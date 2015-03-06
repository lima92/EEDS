#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "buttons.h"
#include "songs.h"

void GPIO_IRQ_Handler();
void PlaySong();
void stop();
void start();
void enableDAC();
void disableDAC();
void stopTimer();
void startTimer();

Song *song;
uint16_t toneCnt = 0;
uint16_t bpmCnt = 0;
uint16_t tone;
uint16_t volume = 0b000000100000;
uint16_t noteNr;
uint16_t bpm;
uint16_t pause;
uint16_t beatsLeft;


/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  //Clear interrupt

  *TIMER1_IFC = 1;
  toneCnt += 1;
  bpmCnt += 1;



  if (toneCnt == tone/2){
    *DAC0_CH0DATA ^= volume;
    *DAC0_CH1DATA ^= volume;
  }else if (toneCnt == tone){
    *DAC0_CH0DATA ^= volume;
    *DAC0_CH1DATA ^= volume;
    toneCnt = 0;
  }
  if (bpmCnt == bpm){
    if(beatsLeft == 1){
      beatsLeft = song->note[noteNr++];
      tone = song->note[noteNr++];
      if(noteNr > song->length){
	stop();
      }
    }else{
      beatsLeft--;
    }
    toneCnt = 0;
    bpmCnt = 0;
    *GPIO_PA_DOUT ^= 0xFFFF;
  }else if (beatsLeft==1 && bpmCnt == pause){
    tone=10000;
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
  uint16_t input = *GPIO_PC_DIN;
  switch(input){
  case(sw1):
    song = &LISA;
    PlaySong();
    break;
  case(sw2):
    song = &STAR_WARS;
    PlaySong();
    break;
  case(sw3):
    song = &MARIO_COIN;
    PlaySong();
    break;
  case(sw4):
    song = &MARIO_1_UP;
    PlaySong();
    break;
  case(sw5):
    tone=170;
    break;
  case(sw6):
    volume = (volume<<1);
    break;
  case(sw7):
    stop();
    break;
  case(sw8):
    volume = (volume>>1);
    if(volume == 0)
      volume = 1;
    break;
  }
  
  
}

void PlaySong(){
  beatsLeft = song->note[0];
  tone = song->note[1];
  noteNr = 2;
  bpm = 6000000/song->bpm;
  pause = bpm - 300;
  toneCnt=0;
  bpmCnt=0;
  *SCR &= 0xFFFFFFFB;
  start();
}

void stop(){
  stopTimer();
  disableDAC();
}

void start(){
  startTimer();
  enableDAC();
}


