#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/lm3s6965.h"
#include "driverlib/timer.h"

#define STACK_SIZE 4096   // Amount of stack space for each thread

void test_func(void)
{
    char str[20];
    scanf("%s", str);
    printf("str is %s", str);
    return ;
}

//Main function of the program, sets up peripherals,
//creates threads, and starts the scheduler
int main(void)
{

        // Set the clocking to run directly from the crystal.
        SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_8MHZ);


        //Enable timer0
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

        //
        // Enable the GPIO pin for the LED0/1 (PF2/3). and PF1  Set the
        // direction as output, and enable the GPIO pin for digital function.
        //
        GPIO_PORTF_DIR_R = 0x0F;
        GPIO_PORTF_DEN_R = 0x0F;

        //Configure timer to run in timeout mode at twice the sysclock
        // frequency
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/2);
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        TimerEnable(TIMER0_BASE, TIMER_A);

        // Enable the peripherals used by this example.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        // Set GPIO A0 and A1 as UART pins.
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        // Configure the UART for 115,200, 8-N-1 operation.
        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
        // Start running coroutines
        // Setup SysTic clock to timeout every 0.5s with interrupt enabled
        IntMasterEnable();
        // Initialize the global thread lock

        printf("aaaaa\n");
        test_func();
        // If scheduler() returns, all coroutines are inactive and we return
        // from main() hence exit() should be called implicitly (according to
        // ANSI C). However, TI's startup_gcc.c code (ResetISR) does not
        // call exit() so we do it manually.
        exit(0);
}


