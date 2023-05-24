.global ctr_core_font_data_begin, ctr_core_font_data_end, ctr_core_font_data_size

.section .rodata.default_font

.align 4
ctr_core_font_data_begin:
.word ctr_core_font_begin

.align 4
ctr_core_font_data_end:
.word ctr_core_font_end

.align 4
ctr_core_font_begin:
.incbin "data/font.pcf"
ctr_core_font_end:

.align 4
ctr_core_font_data_size:
.word ctr_core_font_end-ctr_core_font_begin

