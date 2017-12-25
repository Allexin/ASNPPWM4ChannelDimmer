#ifndef FASTTIMER_H
#define FASTTIMER_H

#include <avr/interrupt.h>
#include <avr/io.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 

#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega88__) 
#define D0_In DDRD &=B11111110
#define D1_In DDRD &=B11111101
#define D2_In DDRD &=B11111011
#define D3_In DDRD &=B11110111
#define D4_In DDRD &=B11101111
#define D5_In DDRD &=B11011111
#define D6_In DDRD &=B10111111
#define D7_In DDRD &=B01111111
 
#define D8_In DDRB &= B11111110
#define D9_In DDRB &= B11111101
#define D10_In DDRB &=B11111011
#define D11_In DDRB &=B11110111
#define D12_In DDRB &=B11101111
#define D13_In DDRB &=B11011111

#define D14_In DDRC &=B11111110
#define D15_In DDRC &=B11111101
#define D16_In DDRC &=B11111011
#define D17_In DDRC &=B11110111
#define D18_In DDRC &=B11101111
#define D19_In DDRC &=B11011111

#define D0_Out DDRD |=B00000001
#define D1_Out DDRD |=B00000010
#define D2_Out DDRD |=B00000100
#define D3_Out DDRD |=B00001000
#define D4_Out DDRD |=B00010000
#define D5_Out DDRD |=B00100000
#define D6_Out DDRD |=B01000000
#define D7_Out DDRD |=B10000000

#define D8_Out DDRB |= B00000001
#define D9_Out DDRB |= B00000010
#define D10_Out DDRB |=B00000100
#define D11_Out DDRB |=B10001000
#define D12_Out DDRB |=B00010000
#define D13_Out DDRB |=B00100000

#define D14_Out DDRC |=B00000001
#define D15_Out DDRC |=B00000010
#define D16_Out DDRC |=B00000100
#define D17_Out DDRC |=B00001000
#define D18_Out DDRC |=B00010000
#define D19_Out DDRC |=B00100000

#define D0_High PORTD |=B00000001
#define D1_High PORTD |=B00000010
#define D2_High PORTD |=B00000100  
#define D3_High PORTD |=B00001000  
#define D4_High PORTD |=B00010000  
#define D5_High PORTD |=B00100000     
#define D6_High PORTD |=B01000000      
#define D7_High PORTD |=B10000000     

#define D8_High PORTB |=B00000001     
#define D9_High PORTB |=B00000010   
#define D10_High PORTB|=B00000100     
#define D11_High PORTB|=B00001000   
#define D12_High PORTB|=B00010000   
#define D13_High PORTB|=B00100000   
  
#define D14_High PORTC |=B00000001   
#define D15_High PORTC |=B00000010    
#define D16_High PORTC |=B00000100   
#define D17_High PORTC |=B00001000      
#define D18_High PORTC |=B00010000     
#define D19_High PORTC |=B00100000   

#define D0_Low PORTD &= B11111110
#define D1_Low PORTD &= B11111101
#define D2_Low PORTD &= B11111011
#define D3_Low PORTD &= B11110111  
#define D4_Low PORTD &= B11101111  
#define D5_Low PORTD &= B11011111 
#define D6_Low PORTD &= B10111111 
#define D7_Low PORTD &= B01111111 

#define D8_Low PORTB &= B11111110 
#define D9_Low PORTB &= B11111101 
#define D10_Low PORTB &=B11111011 
#define D11_Low PORTB &=B11110111 
#define D12_Low PORTB &=B11101111 
#define D13_Low PORTB &=B11011111 

#define D14_Low PORTC &= B11111110  
#define D15_Low PORTC &= B11111101 
#define D16_Low PORTC &= B11111011 
#define D17_Low PORTC &= B11110111
#define D18_Low PORTC &= B11101111
#define D19_Low PORTC &= B11011111 

#define D0_Read (PIND & B00000001)
#define D1_Read ((PIND & B00000010)>>1)
#define D2_Read ((PIND & B00000100)>>2)
#define D3_Read ((PIND & B00001000)>>3)
#define D4_Read ((PIND & B00010000)>>4)
#define D5_Read ((PIND & B00100000)>>5)
#define D6_Read ((PIND & B01000000)>>6)
#define D7_Read ((PIND & B10000000)>>7)

#define D8_Read (PINB & B00000001)
#define D9_Read ((PINB & B00000010)>>1)
#define D10_Read ((PINB & B00000100)>>2)
#define D11_Read ((PINB & B00001000)>>3)
#define D12_Read ((PINB & B00010000)>>4)
#define D13_Read ((PINB & B00100000)>>5)

#define D14_Read (PINC & B00000001)
#define D15_Read ((PINC & B00000010)>>1)
#define D16_Read ((PINC & B00000100)>>2)
#define D17_Read ((PINC & B00001000)>>3)
#define D18_Read ((PINC & B00010000)>>4)
#define D19_Read ((PINC & B00100000)>>5)

//extern void (*func)();  
//void FastTimerStart(void (*isr)(), uint32_t set_us);
//void FastTimerStop(void);
//void FastTimerResume(void);
//void FastTimerRestart(void);

#endif

#endif
