#include "gpio_buttons.h"
#include "user_interface.h"
#include "c_types.h"
#include <gpio.h>
#include <ets_sys.h>
#include <esp82xxutil.h>

void gpio_pin_intr_state_set(uint32 i, GPIO_INT_TYPE intr_state);

volatile uint8_t LastGPIOState;

#define BTN_R 0x01
#define BTN_D 0x02
#define BTN_L 0x04
#define BTN_U 0x08
#define BTN_SELECT 0x10
#define BTN_START  0x20
#define BTN_B 0x40
#define BTN_A 0x80

#define GPIO_START  (1 << 0x00)
#define GPIO_SELECT (1 << 0x02)

#define GPIO_GRP1 (1 << 0x04)
#define GPIO_GRP2 (1 << 0x05)
#define GPIO_GRP3 (1 << 0x15)

// OLD BADGES
// Right  = GPIO 0
// Down   = GPIO 2
// Left   = GPIO 12
// Up     = GPIO 13
// Select = GPIO 14
// Start  = GPIO 15
// B      = GPIO 4
// A      = GPIO 5
// NEW BADGES
// Start = GPIO 0
// Select = GPIO 2
// 4, 5, 16 = Button Readout
// 15 replaces 16 on prototype borads
static const uint8_t GPID[] = { 0, 2, 12, 13, 14, 15, 4, 5 };
static const uint8_t Func[] = { FUNC_GPIO0, FUNC_GPIO2, FUNC_GPIO12, FUNC_GPIO13, FUNC_GPIO14, FUNC_GPIO15, FUNC_GPIO4, FUNC_GPIO5 };
static const int  Periphs[] = { PERIPHS_IO_MUX_GPIO0_U, PERIPHS_IO_MUX_GPIO2_U, PERIPHS_IO_MUX_MTDI_U, PERIPHS_IO_MUX_MTCK_U, PERIPHS_IO_MUX_MTMS_U, PERIPHS_IO_MUX_MTDO_U, PERIPHS_IO_MUX_GPIO4_U, PERIPHS_IO_MUX_GPIO5_U };

void interupt_test( void * v )
{
	int i;

	uint8_t stat = GetButtons();

	for( i = 0; i < 8; i++ )
	{
		int mask = 1<<i;
		if( (stat & mask) != (LastGPIOState & mask) )
		{
			HandleButtonEvent( stat, i, (stat & mask)?1:0 );
		}
	}
	LastGPIOState = stat;

	//clear interrupt status
	uint32  gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
}


void ICACHE_FLASH_ATTR SetupGPIO()
{

	//int i;
	ETS_GPIO_INTR_DISABLE(); // Disable gpio interrupts
	ETS_GPIO_INTR_ATTACH(interupt_test, 0); // GPIO12 interrupt handler

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
	PIN_DIR_INPUT |= GPIO_START;
	PIN_OUT_SET &= GPIO_START;
	gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_START), 3);
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(GPIO_START));

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	PIN_DIR_INPUT |= GPIO_SELECT;
	PIN_OUT_SET &= GPIO_SELECT;
	gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_SELECT), 3);
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(GPIO_START));

	//for( i = 0; i < 8; i++ )
	//{
	//	PIN_FUNC_SELECT(Periphs[i], Func[i]);
	//	PIN_DIR_INPUT = 1<<GPID[i];
	//	gpio_pin_intr_state_set(GPIO_ID_PIN(GPID[i]), 3); // Interrupt on any GPIO12 edge
	//	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(GPID[i])); // Clear GPIO12 status
	//}
	ETS_GPIO_INTR_ENABLE(); // Disable gpio interrupts*/
	LastGPIOState = GetButtons();
 	printf( "Setup GPIO Complete\n" );
}

/*
 * set 4 low
 * A = 5
 * Left = 16/15
 *
 * set 5 low
 * B = 4
 * Up = 16/15
 *
 * set 16/15 low
 * Right = 4
 * Down = 5
 */

uint8_t GetButtons()
{
	uint8_t ret = 0;

	ret |= (PIN_IN & GPIO_START) ? BTN_START : 0;
	ret |= (PIN_IN & GPIO_SELECT) ? BTN_SELECT : 0;

	/*PIN_DIR_OUTPUT |= GPIO_GRP1;
	PIN_OUT_SET &= ~GPIO_GRP1;

	ret |= (PIN_IN & GPIO_GRP2) ? BTN_A : 0;
	ret |= (PIN_IN & GPIO_GRP3) ? BTN_L : 0;

	PIN_DIR_INPUT |= GPIO_GRP1;
	PIN_OUT |= GPIO_GRP1;

	PIN_DIR_OUTPUT |= GPIO_GRP2;
	PIN_OUT_SET &= ~GPIO_GRP2;

	ret |= (PIN_IN & GPIO_GRP1) ? BTN_B : 0;
	ret |= (PIN_IN & GPIO_GRP3) ? BTN_U : 0;

	PIN_DIR_INPUT |= GPIO_GRP2;

	PIN_DIR_OUTPUT |= GPIO_GRP3;
	PIN_OUT_SET &= ~GPIO_GRP3;

	ret |= (PIN_IN & GPIO_GRP1) ? BTN_R : 0;
	ret |= (PIN_IN & GPIO_GRP2) ? BTN_D : 0;

	PIN_DIR_INPUT |= GPIO_GRP3;*/

	ret &= ~32;
	//ret ^= ~32; //GPIO15's logic is inverted.  Don't flip it but flip everything else.
	return ret;
}



