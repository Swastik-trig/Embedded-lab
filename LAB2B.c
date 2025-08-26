#include <stdint.h>
#include "tm4c123gh6pm.h"

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;               // Enable clock to GPIOF
    while ((SYSCTL_PRGPIO_R & 0x20) == 0);   // Wait until ready

    GPIO_PORTF_LOCK_R = 0x4C4F434B;          // Unlock commit register
    GPIO_PORTF_CR_R |= 0x1F;                 // Allow changes to PF0–PF4
    GPIO_PORTF_DIR_R |= 0x0E;                // PF1, PF2, PF3 = output (LEDs)
    GPIO_PORTF_DEN_R |= 0x1F;                // Enable digital for PF0–PF4
    GPIO_PORTF_PUR_R |= 0x11;                // Enable pull-ups on PF0, PF4 (switches)
}

void delayMs(int n) {
    volatile int i, j;
    for(i = 0; i < n; i++)
        for(j = 0; j < 3180; j++);  // Approx 1 ms delay
}

int main(void) {
    PortF_Init();

    int state = 0;
    int colors[8] = {0x00, 0x02, 0x04, 0x08, 0x0A, 0x06, 0x0C, 0x0E};
    // OFF, Red, Blue, Green, Yellow, Pink, Cyan, White

    while(1) {
        if((GPIO_PORTF_DATA_R & 0x01) == 0) {   // SW2 pressed (PF0 low)
            delayMs(20);  // debounce delay

            while((GPIO_PORTF_DATA_R & 0x01) == 0); // wait for release
            delayMs(20);  // debounce delay

            // Next color
            state = (state + 1) % 8;

            GPIO_PORTF_DATA_R = colors[state];
        }
    }
}
