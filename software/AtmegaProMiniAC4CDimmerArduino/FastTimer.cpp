
/*
#include "FastTimer.h"
#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)

#define DIV_0    TCCR1B = (1 << CS10)
#define DIV_8    TCCR1B = (1 << CS11)
#define DIV_64   TCCR1B = ((1 << CS11) | (1 << CS10))
#define DIV_256  TCCR1B = (1 << CS12)
#define DIV_1024 TCCR1B = ((1 << CS12) | (1 << CS10))

void (*func)();
volatile uint16_t dub_tcnt1;
 
void FastTimerStart(void (*isr)(), uint32_t set_us)
{ 
  TIMSK1 &= ~(1<<TOIE1);
  func = *isr;
  TCCR1A = 0;
  TCCR1B = 0;
  if(set_us > 5 && set_us < 4096){
    set_us = 65584 - (set_us << 4);
    DIV_0;
  }
  else
    if(set_us > 4095 && set_us < 32768){ 
      set_us = 65542 - (set_us << 1);
      DIV_8;
    } 
    else
      if(set_us > 32767 && set_us < 262144){ 
        set_us = 65536 - (set_us >> 2);
        DIV_64;
      }
      else
        if(set_us > 262143 && set_us < 1048576){
          set_us = 65536 - (set_us >> 4);
          DIV_256;
        }
        else
          if(set_us > 1048575 && set_us < 4194304){
            set_us = 65536 - (set_us >> 6);
            DIV_1024;
          }
          else{
            TCCR1B = 1;
          }
  
  dub_tcnt1 = set_us;
  TCNT1 = 0;
  TIMSK1 |= (1 << TOIE1);
  sei();   
}

void FastTimerStop(void)
{
  TIMSK1 &= ~(1<<TOIE1);
}

void FastTimerResume(void)
{
  TIMSK1 |= (1<<TOIE1);
}

void FastTimerRestart(void)
{
  TCNT1 = dub_tcnt1;
  TIMSK1 |= (1<<TOIE1);
}

ISR(TIMER1_OVF_vect) 
{
  //TCNT1 = dub_tcnt1;    
  (*func)();
}
#endif*/
