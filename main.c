#include <stdint.h>
#include "tm4c123gh6pm.h"

// LED masks
#define RED_LED    (1U << 1)  // PF1
#define BLUE_LED   (1U << 2)  // PF2
#define GREEN_LED  (1U << 3)  // PF3

// Switch mask
#define SW2        (1U << 0)  // PF0

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;               // Enable clock to GPIOF
    while ((SYSCTL_PRGPIO_R & 0x20) == 0);   // Wait until ready

    GPIO_PORTF_LOCK_R = 0x4C4F434B;          // Unlock PF0
    GPIO_PORTF_CR_R |= 0x1F;                 // Allow changes to PF0–PF4
    GPIO_PORTF_DIR_R |= (RED_LED | BLUE_LED | GREEN_LED); // PF1–3 = outputs
    GPIO_PORTF_DIR_R &= ~SW2;                // PF0 = input
    GPIO_PORTF_DEN_R |= 0x1F;                // Digital enable PF0–PF4
    GPIO_PORTF_PUR_R |= SW2;                 // Pull-up on PF0
}

void delayMs(int n) {
    volatile int i, j;
    for(i = 0; i < n; i++)
        for(j = 0; j < 3180; j++);  // ~1 ms
}

int main(void) {
    PortF_Init();

    int state = 0;
    // OFF, Red, Blue, Green, Yellow, Pink, Cyan, White
    int colors[8] = {
        0x00,
        RED_LED,
        BLUE_LED,
        GREEN_LED,
        RED_LED | GREEN_LED,   // Yellow
        RED_LED | BLUE_LED,    // Pink
        BLUE_LED | GREEN_LED,  // Cyan
        RED_LED | BLUE_LED | GREEN_LED // White
    };

    while(1) {
        if((GPIO_PORTF_DATA_R & SW2) == 0) {   // SW2 pressed
            delayMs(20);  // debounce

            while((GPIO_PORTF_DATA_R & SW2) == 0); // wait for release
            delayMs(20);  // debounce

            // Next color
            state = (state + 1) % 8;

            // Clear LED bits first, then set new color (RMW)
            GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~(RED_LED | BLUE_LED | GREEN_LED)) | colors[state];
        }
    }
}
