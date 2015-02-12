#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define DATA_SPACE ((volatile uint32_t*) (0x20000000))


/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  //Clear interrupt
  
  *TIMER1_IFC = 1;
  *DATA_SPACE += 1;
 
  if (*DATA_SPACE > 106){
    *GPIO_PA_DOUT = (*GPIO_PA_DOUT << 1);
    *DATA_SPACE -= 106;
  }
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */  
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  //  *TIMER1_IFC = 1;
  //*GPIO_PA_DOUT ^= 0xff00;
    /* TODO handle button pressed event, remember to clear pending interrupt */
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
}
