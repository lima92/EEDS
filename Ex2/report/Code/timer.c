#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer */
void setupTimer(uint16_t period)
{
  //  uint32_t clock = *CMU_HFPERCLKEN0;
  *CMU_HFPERCLKEN0 |= (1 << 6);

  //14MHz clock frq. divided by 4800 samples per second
  *TIMER1_TOP = period;
  *TIMER1_IEN = 1;
 
  *TIMER1_CMD = 1;

  //Prescale clock
  *CMU_HFPERCLKDIV |= 0;
  
  /*
    TODO enable and set up the timer
    
    1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
    2. Write the period to register TIMER1_TOP
    3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
    4. Start the timer by writing 1 to TIMER1_CMD
    
    This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked.
  */  
}

void stopTimer(){
  *TIMER1_IFC = 1;
  *TIMER1_IEN = 0;
  *TIMER1_CMD = 0b10;
  *SCR |= 0b100;
  *GPIO_PA_DOUT ^= 0xFF00;
}

void startTimer(){
  *TIMER1_IEN = 1;
  *TIMER1_CMD = 0b1;
}


