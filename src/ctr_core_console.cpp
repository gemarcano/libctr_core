#include <ctr_core/ctr_core_console.h>

#include <sys/iosupport.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

//FIXME Freetype interactions are wonky. I'm not handling the cases where metrics can be negative due to changes in text orientation

#include <ctr_core/ctr_core_freetype.h>
#include <ctr_core/ctr_core_screen.h>
#include <ctr_core/ctr_core_surface.h>

static ssize_t ctr_core_console_write_r(struct _reent *r, void *fd, const char *ptr, std::size_t len);

static const devoptab_t tab =
{
	"console",
	0,
	NULL,
	NULL,
	ctr_core_console_write_r,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static ctr_core_console console;

int ctr_core_console_initialize(ctr_core_surface *surface)
{
	devoptab_list[STD_OUT] = &tab;
	devoptab_list[STD_ERR] = &tab;
	setvbuf(stdout, NULL , _IONBF, 0);
	setvbuf(stderr, NULL , _IONBF, 0);

	memset(&console, 0, sizeof(console));
	console.font_pt = 14;
	console.width = ctr_core_surface_get_width(surface);
	console.height = ctr_core_surface_get_height(surface);
	console.default_fg = console.fg = 0xFFFFFF;
	console.default_bg = console.bg = 0x000000;

	console.surface = surface;
	ctr_core_circular_buffer_initialize(&console.buffer, 17 * 1024u);

	return 0;
}

short ctr_core_console_get_char_width(char c)
{
	FTC_SBit bit = ctr_core_freetype_prepare_character(c);
	return bit->xadvance;
}

unsigned int ctr_core_console_get_char_height(void)
{//FIXME currently char height == line height... will this always be the case?
	FT_Face face = ctr_core_freetype_get_face();
	//This should always be positive...
	return (unsigned int)(face->size->metrics.height >> 6);
}

static void pixel_set(void *buffer, std::uint32_t pixel, std::size_t pixel_size, std::size_t count)
{
	for (std::size_t i = 0; i < count; ++i)
	{
		for (std::size_t j = 0; j < pixel_size; ++j)
			((char*)buffer)[i*pixel_size + j] = pixel >> (j*CHAR_BIT);
	}
}

static void draw_shift_up(void)
{
	// Buffer is bottom to top, left to right
	auto& surface = *reinterpret_cast<ctr_core::generic_surface*>(console.surface);
	auto &screen = surface.get_screen();

	std::size_t offset_to_next_column = 0;
	//For my own sanity, I am defining that the framebuffer pixels are within is
	//one large array object. This allows for pointer arithmetic.
	unsigned char *data = reinterpret_cast<unsigned char*>(surface.get_pixel(0,0).get_buffer());
	if (surface.width() > 1)
	{
		std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(data);
		std::uintptr_t end = reinterpret_cast<std::uintptr_t>(surface.get_pixel(0,1).get_buffer());
		offset_to_next_column = end - begin; 
	}
	unsigned int line_height = ctr_core_console_get_char_height();
	std::size_t copy_col = (surface.height() - line_height) * screen.pixel_size();
	std::size_t line_pixels = line_height * screen.pixel_size();

	for (std::size_t x = 0; x < surface.width(); ++x)
	{
		memmove(data, data + line_pixels, copy_col);
		memset(data + copy_col, 0, line_pixels);
		data += offset_to_next_column;
	}
}

void ctr_core_console_draw(char c)
{
	//This console implementation assumes horizontal layout, always... at least for now
	//This means the xadvance should always be positive
	unsigned int cwidth = (unsigned int)ctr_core_console_get_char_width(c);
	unsigned int cheight = ctr_core_console_get_char_height();

	FTC_SBit bit = ctr_core_freetype_prepare_character(c);

	if (c == '\n' || (console.xpos + cwidth) > console.width)
	{
		console.xpos = 0;
		console.ypos += cheight;
	}

	if (console.ypos + cheight >= console.height)
	{
		draw_shift_up();
		console.ypos -= cheight;
	}

	if (!(c == '\r' || c == '\n'))
	{
		//Can I assume bit->top is always positive for horizontal layouts?
		std::size_t off = (ctr_core_console_get_char_height() - (unsigned int)(bit->top));
		ctr_core_freetype_draw(console.surface, console.xpos, console.ypos + off, c, console.fg, console.bg);
		console.xpos += cwidth;
	}

	if (ctr_core_circular_buffer_count(&console.buffer) == ctr_core_circular_buffer_size(&console.buffer))
	{
		ctr_core_circular_buffer_pop_front(&console.buffer, NULL);
	}
	ctr_core_circular_buffer_push_back(&console.buffer, c);
}

void ctr_core_console_clear(void)
{
	console.xpos = console.ypos = 0;
	ctr_core_surface_clear(console.surface, console.bg);
}

typedef enum
{
	CSI_INITIAL, CSI_PARAMETER, CSI_NEW_PARAMETER, CSI_DONE, CSI_ERR
} csi_state;

static inline bool iscommand(char c)
{
	return NULL != strchr("ABCDEFGHJKSTfmsu", c);
}

typedef struct
{
	const char *params[2]; //Not supporting more than 2 parameters, rest are ignored
	unsigned char command;

	std::size_t bytes_read;
} csi_data;

static csi_state do_csi_initial(csi_data *data, const char *str, std::size_t i)
{
	memset(data, 0, sizeof(*data));
	if (isdigit(str[i]))
	{
		data->params[0] = str+i;
		return CSI_PARAMETER;
	}
	else if (iscommand(str[i]))
	{
		data->command = str[i];
		return CSI_DONE;
	}

	return CSI_ERR;
}

static csi_state do_csi_parameter(csi_data *data, const char *str, std::size_t i)
{
	if (str[i] == ';')
	{
		return CSI_NEW_PARAMETER;
	}
	else if (isdigit(str[i]))
	{
		return CSI_PARAMETER;
	}
	else if (iscommand(str[i]))
	{
		data->command = str[i];
		return CSI_DONE;
	}

	return CSI_ERR;
}

static csi_state do_csi_new_parameter(csi_data *data, const char *str, std::size_t i)
{
	if (isdigit(str[i]))
	{
		if (data->params[1] == NULL)
		{
			data->params[1] = str + i;
		}
		return CSI_PARAMETER;
	}
	return CSI_ERR;
}

static csi_state do_csi_done(csi_data *data, const char *str, std::size_t i)
{
	return CSI_ERR;
}

typedef csi_state (*csi_func)(csi_data *data, const char *str, std::size_t i);

static const csi_func state_table[4] =
{
	do_csi_initial, do_csi_parameter, do_csi_new_parameter, do_csi_done
};

static csi_state csi_run(csi_state state, csi_data *data, const char *str, std::size_t i)
{
	return state_table[state](data, str, i);
}

static inline std::size_t extract_param(const char* param, std::size_t def)
{
	return (param) ?
		(std::size_t)strtol(param, NULL, 10) :
		def;
}

static std::uint32_t colors[8] = {
	0x00, 0xAA0000, 0x00AA00, 0x808000, 0x0000AA, 0xAA00AA, 0x00AAAA, 0xAAAAAA
};

static void csi_sgm(const csi_data *data)
{
	std::size_t param1 = extract_param(data->params[0], 0);
	std::size_t param2 = extract_param(data->params[1], 0);

	switch (param1)
	{
		case 0:
		{
			console.fg = console.default_fg;
			console.bg = console.default_bg;
		}
		break;

		case 7:
		{
			if (!console.negative)
			{
				std::uint32_t tmp = console.fg;
				console.fg = console.bg;
				console.bg = tmp;

				console.negative = true;
			}
		}
		break;

		case 27:
			if (console.negative)
			{
				std::uint32_t tmp = console.fg;
				console.fg = console.bg;
				console.bg = tmp;

				console.negative = false;
			}
		break;

		case 30: case 31: case 32: case 33: case 34: case 35: case 36: case 37:
			console.fg = colors[param1 - 30];
		break;

		case 40: case 41: case 42: case 43: case 44: case 45: case 46: case 47:
			console.bg = colors[param1 - 40];
		break;

		default:
		break;
	}
}

static void execute_command(const csi_data *data)
{
	switch (data->command)
	{
		case 'A':
		{
			std::uint16_t cheight = (std::size_t)ctr_core_console_get_char_height();
			std::size_t param = extract_param(data->params[0], 1);

			std::size_t pos = console.ypos > (param*cheight) ? console.ypos-(param*cheight) : 0;
			//console_adjust_cursor(console.xpos, pos);

		}
		break;
		case 'B':
		{
			std::uint16_t cheight = ctr_core_console_get_char_height();
			std::size_t param = extract_param(data->params[0], 1);

			std::size_t pos = console.ypos + param * cheight;
			if (pos > console.height) pos = console.height - cheight;
			//console_adjust_cursor(console.xpos, pos);
		}
		break;
		case 'C':
		{
			//The problem here is that cwidth may not be constant
			unsigned int cwidth = (unsigned int)ctr_core_console_get_char_width('T'); //uh,FIXME, I need to keep track of the text contents...
			std::size_t param = extract_param(data->params[0], 1);

			std::size_t pos = console.xpos + param*cwidth;
			if (pos > console.width) pos = console.width - cwidth;
			//console_adjust_cursor(pos, console.ypos);
		}
		break;

		case 'D':
		{
			unsigned int cwidth = (unsigned int)ctr_core_console_get_char_width('T'); //uh,FIXME, I need to keep track of the text contents...
			std::size_t param = extract_param(data->params[0], 1);

			std::size_t pos = console.xpos > (param*cwidth) ? console.xpos-(param*cwidth) : 0;
			//console_adjust_cursor(pos, console.ypos);
		}
		break;

		case 'E':
		{
			std::uint16_t cheight = ctr_core_console_get_char_height();
			std::size_t param = extract_param(data->params[0], 1);

			std::size_t pos = console.ypos + param*cheight;
			if (pos > console.height) pos = console.height - cheight;
			//console_adjust_cursor(0, pos);
		}
		break;

		case 'F':
		{
			std::uint16_t cheight = ctr_core_console_get_char_height();
			std::size_t param = extract_param(data->params[0], 1);

			std::size_t pos = console.ypos > param*cheight ? console.ypos - param*cheight : 0;
			//console_adjust_cursor(0, pos);
		}
		break;

		case 'G':
		{
			unsigned int cwidth = (unsigned int)ctr_core_console_get_char_width('T'); //uh,FIXME, I need to keep track of the text contents...
			std::size_t param = extract_param(data->params[0], 1);
			param -= 1; //Is this indexed 0 or 1 (currently assuming 1)

			std::size_t pos = param * cwidth < console.width  ? param*cwidth : console.width;
			//console_adjust_cursor(pos, 0);

		}
		break;

		case 'f':
		case 'H':
		{
			std::size_t param1 = extract_param(data->params[0], 1);
			std::size_t param2 = extract_param(data->params[1], 1);
			unsigned int cwidth = (unsigned int)ctr_core_console_get_char_width('T'); //uh,FIXME, I need to keep track of the text contents...
			std::uint16_t cheight = ctr_core_console_get_char_height();

			std::size_t posx = param1*cwidth < console.width ? param1*cwidth : console.width;
			std::size_t posy = param2*cheight < console.height ? param2*cheight : console.height;
			//console_adjust_cursor(posx, posy);
		}
		break;

		case 'J':
		{
			std::size_t param = extract_param(data->params[0], 0);

			if (param == 0)
			{
				//TODO
			}
			else if (param == 1)
			{
				//TODO
			}
			else if (param == 2)
			{
				ctr_core_console_clear();
			}
		}
		break;

		case 'K':
		{
			std::size_t param = extract_param(data->params[0], 0);
			//TODO
		}
		break;

		case 'S':
		case 'T':
			break;

		case 'm':
			csi_sgm(data);
			break;

		case 's':
			console.saved_xpos = console.xpos;
			console.saved_ypos = console.ypos;
			break;

		case 'u':
			console.xpos = console.saved_xpos;
			console.ypos = console.saved_ypos;
			break;

		default:
			break;
	}
}

static std::size_t process_csi(const char *str, std::size_t len)
{
	csi_state state = CSI_INITIAL;
	csi_data data = {0};
	std::size_t i;
	for (i = 1;
		state != CSI_DONE &&
		state != CSI_ERR &&
		i < len; ++i)
	{
		state = csi_run(state, &data, str, i);
	}

	if (state == CSI_DONE)
	{
		execute_command(&data);
	}

	return i;
}

static ssize_t ctr_core_console_write_r(struct _reent *r, void *fd, const char *ptr, std::size_t len)
{
	for (std::size_t i = 0; i < len; ++i)
	{
		if (ptr[i] == 0x1B)
		{
			if (i+1 != len && ptr[i+1] == '[')
			{
				++i;
				i += process_csi(ptr+i, len-i) - 1;
			}
			//else, just ignore the ESC character
		}
		else
		{
			ctr_core_console_draw(ptr[i]);
		}
	}
	return (ssize_t)len;
}

