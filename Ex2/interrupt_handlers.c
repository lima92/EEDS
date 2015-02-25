#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "buttons.h"
#include "songs.h"

void GPIO_IRQ_Handler();
Song song;
uint16_t toneCnt = 0;
uint16_t bpmCnt = 0;
uint32_t tone = 44000;
uint16_t volume = 0b000000100000;
uint16_t noteNr;
unit8_t bpm;
uint8_t beatsLeft;
 

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  //Clear interrupt
  
  *TIMER1_IFC = 1;
  tonecnt += 1;
  bpmcnt += 1;

  if (toneCnt == tone]/2){
     *DAC0_CH0DATA ^= volume;
     *DAC0_CH1DATA ^= volume;
  }
  if (toneCnt == tone){
     *DAC0_CH0DATA ^= volume;
     *DAC0_CH1DATA ^= volume;
     toneCnt -= tone;
  }
  if (bpmCnt == song.bpm){
    if(beatsLeft==0){
      beatsLeft = song.note[noteNr++];
      tone = song.note[noteNr++];
    }else{
      beatsLeft--;
    }
  }
  

    
  /* if ( cnt % 20 == 0){
  *DAC0_CH0DATA ^= 0b000000100000;
  *DAC0_CH1DATA ^= 0b000000100000;
  }*/
 
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
  uint16_t input = *GPIO_PC_DIN;
  switch(input){
  case(sw1):
    song = LISA;
    tone=255;
    break;
  case(sw2):
    tone=227;
    break;
  case(sw3):
    tone=202;
    break;
  case(sw4):
    tone=191;
    break;
  case(sw5):
    tone=170;
    break;
  case(sw6):
    tone=152;
    break;
  case(sw7):
    tone=143;
    break;
  case(sw8):
    tone=128;
    break;
  }
  noteNr = 1;
  cnt=0;
  
}
