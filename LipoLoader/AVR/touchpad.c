

#include "OS/FabOS.h"

volatile uint16_t pads[4];
void checkTouchpad(void)
{
	uint8_t i;
	uint8_t cnt;

	cnt = 0;
		for(i=0;i<3;i++)
		{
			PORTE.PIN1CTRL = PORT_OPC_PULLUP_gc;
			while (!(PORTE.IN & (1<<0)))
			{
				cnt++;
			}
			PORTE.PIN1CTRL = PORT_OPC_PULLDOWN_gc;
			while (PORTE.IN & (1<<0))
			{
				cnt++;
			}
		}
		PORTE.PIN1CTRL = PORT_OPC_TOTEM_gc;
		pads[0]=cnt;

		cnt = 0;
		for(i=0;i<3;i++)
		{
			PORTE.PIN0CTRL = PORT_OPC_PULLUP_gc;
			while (!(PORTE.IN & (1<<1)))
			{
				cnt++;
			}
			PORTE.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
			while (PORTE.IN & (1<<1))
			{
				cnt++;
			}
		}
		PORTE.PIN0CTRL = PORT_OPC_TOTEM_gc;
		pads[1]=cnt;

	/*	cnt = 0;
		for(i=0;i<3;i++)
		{
			PORTE.PIN2CTRL = PORT_OPC_PULLUP_gc;
			while (!(PORTE.IN & (1<<2)))
			{
				cnt++;
			}
			PORTE.PIN2CTRL = PORT_OPC_PULLDOWN_gc;
			while (PORTE.IN & (1<<2))
			{
				cnt++;
			}
		}
		pads[2]=cnt;

		cnt = 0;
		for(i=0;i<3;i++)
		{
			PORTE.PIN3CTRL = PORT_OPC_PULLUP_gc;
			while (!(PORTE.IN & (1<<3)))
			{
				cnt++;
			}
			PORTE.PIN3CTRL = PORT_OPC_PULLDOWN_gc;
			while (PORTE.IN & (1<<3))
			{
				cnt++;
			}
		}
		pads[3]=cnt;*/

		asm("nop");

}
