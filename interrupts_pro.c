#include <lpc21xx.h>
#include "interrupts_pro.h"
#include "types.h"
#include "interrupts_defines_pro.h"

void Enable_EINT1(void)
{
 PINSEL0|=0x000000C0;
 VICIntEnable=1<<EINT0_VIC_CHNO;
 VICVectCntl1=(1<<5)|EINT0_VIC_CHNO;
 VICVectAddr1=(u32)eint0_isr;
 EXTMODE=1<<0;
}
/*void Enable_UART1(void)
{
	PINSEL0|=0x00000000;
	U1IER = 0x03;
	VICIntEnable=1<<7;
	VICVectCntl0=(1<<5)|7;
	VICVectAddr0=(u32)UART1_isr;
} */

