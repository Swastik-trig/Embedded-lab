#include <stdint.h>
#include "tm4c123gh6pm.h"

#define GPIO_PORTF_DATA_R   (*((volatile uint32_t *)0x400253FC))

// LEDs
#define RED_LED   (1U << 1)   // PF1
#define BLUE_LED  (1U << 2)   // PF2
// Switches
#define SW2       (1U << 0)   // PF0
#define SW1       (1U << 4)   // PF4
int main(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;               // Enable clock to GPIOF
        while ((SYSCTL_PRGPIO_R & 0x20) == 0);   // Wait until ready

        GPIO_PORTF_LOCK_R = 0x4C4F434B;
           GPIO_PORTF_CR_R |= (RED_LED | BLUE_LED | SW1 | SW2);


           GPIO_PORTF_DIR_R |= (RED_LED | BLUE_LED); // PF1, PF2 = output (LEDs)
           GPIO_PORTF_DIR_R &= ~(SW1 | SW2);         // PF0, PF4 = input (switches)
           GPIO_PORTF_DEN_R |= (RED_LED | BLUE_LED | SW1 | SW2);
           GPIO_PORTF_PUR_R |= (SW1 | SW2);

           while(1) {
               // SW1 pressed  turn ON Red LED
               if((GPIO_PORTF_DATA_R & SW1) == 0) {
                   GPIO_PORTF_DATA_R |= RED_LED;
               }
               if((GPIO_PORTF_DATA_R & SW2) == 0) {
                   GPIO_PORTF_DATA_R |= BLUE_LED;
               } else {
                   GPIO_PORTF_DATA_R= 0x00;
               }
           }
       }

