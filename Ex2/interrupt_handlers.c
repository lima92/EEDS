#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void GPIO_IRQ_Handler();
uint8_t cnt = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  //Clear interrupt
  
  *TIMER1_IFC = 1;
  cnt += 1;
 
  if (cnt > 106){
    *GPIO_PA_DOUT = (*GPIO_PA_DOUT << 1);
    cnt -= 106;
  }

  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */  
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
  GPIO_IRQ_Handler();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
  GPIO_IRQ_Handler();
}

void GPIO_IRQ_Handler()
{
  *GPIO_IFC = *GPIO_IF;
  uint8_t input = *GPIO_PC_DIN;
  //if(input && 0b00100000){
  *GPIO_PA_DOUT = 0b0101010101010101;
  // }
  
}
