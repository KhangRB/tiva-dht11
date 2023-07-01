

/**
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "driverlib/pin_map.h"

void dht11_read(void);

int main(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    while(1)
    {
        GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3);
        SysCtlDelay(2*SysCtlClockGet()/3);  // wait for 1s
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
        SysCtlDelay(20*SysCtlClockGet()/3000);  // at least 18ms
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);
        SysCtlDelay(44*SysCtlClockGet()/3000000);   // ~40us

        uint8_t count = 7;
        uint8_t i = 0;
        uint8_t bit_received;
        uint8_t dat[5] = {0,0,0,0,0};

        GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);
        while(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3) == GPIO_PIN_3);
        while(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3) != GPIO_PIN_3);
        while(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3) == GPIO_PIN_3);

        for(bit_received = 0; bit_received < 40; ++bit_received)
        {
            while(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3) != GPIO_PIN_3);
            SysCtlDelay(30*SysCtlClockGet()/3000000);
            if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3) == GPIO_PIN_3)
            {
                dat[i] |= (1 << count);
            }
            if(count == 0)
            {
                count = 7;
                i++;
            } else count--;
            while(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3) == GPIO_PIN_3);
        }
        uint8_t checksum = dat[0] + dat[1] + dat[2] + dat[3];
        if (checksum == dat[4]) break;
    }
    return 0;
}




