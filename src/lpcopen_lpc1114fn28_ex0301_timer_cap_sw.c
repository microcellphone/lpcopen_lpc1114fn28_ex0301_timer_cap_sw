/*
===============================================================================
 Name        : lpcopen_lpc1114fn28_ex0301_timer_cap_sw.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "led.h"
#include "sw.h"

// TODO: insert other definitions and declarations here

#define TICKRATE_HZ (1000) // Timer frequency in Hz
#define TICKRATE_MS (1000/TICKRATE_HZ)

static volatile uint32_t g_swPressCount = 0;

void TIMER16_0_IRQHandler(void)
{
	if (Chip_TIMER_CapturePending(LPC_TIMER16_0, CAP0)) {
       Chip_TIMER_ClearCapture(LPC_TIMER16_0, CAP0);
    }

	if (Chip_TIMER_CapturePending(LPC_TIMER16_0, CAP1)) {
       Chip_TIMER_ClearCapture(LPC_TIMER16_0, CAP1);
    }
}

void TIMER16_1_IRQHandler(void)
{
	if (Chip_TIMER_CapturePending(LPC_TIMER16_1, CAP0)) {
       Chip_TIMER_ClearCapture(LPC_TIMER16_1, CAP0);
    }

	if (Chip_TIMER_CapturePending(LPC_TIMER16_1, CAP1)) {
       Chip_TIMER_ClearCapture(LPC_TIMER16_1, CAP1);
    }
}

void TIMER32_0_IRQHandler(void)
{
	if (Chip_TIMER_CapturePending(LPC_TIMER32_0, CAP0)) {
    	g_swPressCount++;
       Chip_TIMER_ClearCapture(LPC_TIMER32_0, CAP0);
    }

	if (Chip_TIMER_CapturePending(LPC_TIMER32_0, CAP1)) {
       Chip_TIMER_ClearCapture(LPC_TIMER32_0, CAP1);
    }
}

void TIMER32_1_IRQHandler(void)
{
	if (Chip_TIMER_CapturePending(LPC_TIMER32_1, CAP0)) {
       Chip_TIMER_ClearCapture(LPC_TIMER32_1, CAP0);
    }

	if (Chip_TIMER_CapturePending(LPC_TIMER32_1, CAP1)) {
       Chip_TIMER_ClearCapture(LPC_TIMER32_1, CAP1);
    }
}

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here
    bool led;

    SysTick_Config(SystemCoreClock/1000 - 1);
    IOCON_Config_Request();
    LED1_Config_Request();

    /* Set up the GPIO pin for the capture input */
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC2 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN);

    TIMER_Capture_Request(LPC_TIMER32_0, CAP0, TICKRATE_HZ);

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        if (g_swPressCount > 0) {
        	led ^= 1;
        	LED_Set_Request(LED1, led);
        	g_swPressCount--;
        	Delay(1000);
         }
        i++ ;
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
