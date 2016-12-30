#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <ctr_core/ctr_core_pxi.h>

static volatile uint32_t *pxi_base = NULL;

void ctr_core_pxi_change_base(volatile uint32_t *base)
{
	pxi_base = base;
}

volatile uint32_t* ctr_core_pxi_get_base(void)
{
	return pxi_base;
}

bool ctr_core_pxi_send_empty_status(void)
{
	return PXI_CNT(pxi_base) & 0x1;
}

bool ctr_core_pxi_send_full_status(void)
{
	return PXI_CNT(pxi_base) & (1 << 1);
}

bool ctr_core_pxi_get_send_empty_irq(void)
{
	return PXI_CNT(pxi_base) & (1 << 2);
}

void ctr_core_pxi_set_send_empty_irq(bool aState)
{
	uint32_t cnt = PXI_CNT(pxi_base);
	cnt &= ~(1u << 2);
	PXI_CNT(pxi_base) = cnt | ((uint32_t)aState) << 2;
}

void ctr_core_pxi_fifo_send_clear(void)
{
	PXI_CNT(pxi_base) |= 1 << 3;
}

bool ctr_core_pxi_receive_empty_status(void)
{
	return PXI_CNT(pxi_base) & (1 << 8);
}

bool ctr_core_pxi_receive_full_status(void)
{
	return PXI_CNT(pxi_base) & (1 << 9);
}

bool ctr_core_pxi_get_receive_not_empty_irq(void)
{
	return PXI_CNT(pxi_base) & (1 << 10);
}

void ctr_core_pxi_set_receive_not_empty_irq(bool aState)
{
	uint32_t cnt = PXI_CNT(pxi_base);
	cnt &= ~(1u << 10);
	PXI_CNT(pxi_base) = cnt | ((uint32_t)aState) << 10;
}

void ctr_core_pxi_set_enabled(bool aState)
{
	uint32_t cnt = PXI_CNT(pxi_base);
	cnt &= ~(1u << 15);
	PXI_CNT(pxi_base) = cnt | ((uint32_t)aState) << 15;
}

bool ctr_core_pxi_get_enabled(void)
{
	return PXI_CNT(pxi_base) & (1 << 15);
}

void ctr_core_pxi_fifo_ack(void)
{
	PXI_CNT(pxi_base) |= 1 << 14;
}

bool ctr_core_pxi_get_error(void)
{
	return PXI_CNT(pxi_base) & (1 << 14);
}

bool ctr_core_pxi_push(uint32_t aData)
{
	if (!ctr_core_pxi_send_full_status())
	{
		PXI_SEND(pxi_base) = aData;
		return true;
	}
	return false;
}

bool ctr_core_pxi_pop(uint32_t *apData)
{
	if (!ctr_core_pxi_receive_empty_status())
	{
		*apData = PXI_RECV(pxi_base);
		return true;
	}
	return false;
}

