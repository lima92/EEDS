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
void setTimer(uint16_t time);

Song *song;
short next;
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
  // toneCnt += 1;
  // bpmCnt += 1;
  

  //check if tone is next
  if (next == 0){
    *DAC0_CH0DATA ^= volume;
    *DAC0_CH1DATA ^= volume;
    toneCnt = tone;
    bpmCnt -= tone;
  }

  //check if bpm is next
  else if (next == 1){
    if(beatsLeft == 1){ //Set up next tone if on last
      beatsLeft = song->note[noteNr++];
      tone = (song->note[noteNr++]>>1);
      if(noteNr > song->length){
	stop();
      }
      next = 3; //Enter a short pause before next tone
    }else{
      beatsLeft--;
    }
    bpmCnt = bpm;
    *GPIO_PA_DOUT ^= 0xFFFF; //Blink LED with beat
  }

  if(next == 3){
    setTimer(10000); //300*140/14000 sec pause
    next=0;
  }else{
    if(toneCnt<bpmCnt){
      next = 0;
      setTimer(toneCnt);
    }else{
      next = 1;
      setTimer(bpmCnt);
    }
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
  tone = (song->note[1]>>1);
  noteNr = 2;
  bpm = 6000000/song->bpm;
  bpmCnt = bpm;
  next=0;
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
  setTimer(1);
}

void setTimer(uint16_t time){
  *TIMER1_TOP = time * 140;
}

