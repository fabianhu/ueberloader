

#include "touchpad.h"
#include "OS/FabOS.h"

uint8_t touchcalbytes[5] = TOUCHCALINIT;


uint8_t touchGetPad(uint8_t pin)
{
	uint8_t cnt,i;
	uint8_t mask = (1<<pin);

	cnt = 0;
	OS_ENTERCRITICAL  // no interrupts allowed here!
	for(i=0;i<TOUCHTIMES;i++)
	{
		TOUCHTOGGLEHIGH;
		while (!(TOUCHPORT.IN & mask))
		{
			cnt++;
		}
		TOUCHTOGGLELOW;
		while (TOUCHPORT.IN & mask)
		{
			cnt++;
		}
	}
	OS_LEAVECRITICAL

	touchcalbytes[pin] = min(touchcalbytes[pin],cnt);


	// every some time, correct the calibration bytes.. even, if a touch is recognized... -> provides self healing..


	return cnt - touchcalbytes[pin];
}


void touch_init(void)
{
	TOUCHCONFIGPORT;


}

/*volatile uint16_t pads[4];
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

		cnt = 0;
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
		pads[3]=cnt;

		asm("nop");

}
*/

