// MIF18 Lab, 2020-21, Laure Gonnord

#include <avr/io.h>			// for the input/output register
#include <avr/interrupt.h> 	
#include <util/delay.h>  	// for the _delay_ms

int int_counter = 0;
volatile int second = 0;

void init_led_red(void)
{
  // TODO : init, red led is on analog 0
}

void init_led_yellow(void)
{
  // TODO : init, yellow led on analog 1
}


void init_timer(void)
{
  TCCR2B = 0b00000100; // Clock / 256 soit 16 micro-s et WGM22 = 0
  TIMSK2 = 0b00000001; // Interruption locale autorisée par TOIE2
}


void task_led_red(void)
{
  // TODO : call to init for the red led
  // then blink, then wait (delay)
}


ISR(TIMER2_OVF_vect)
{
  int_counter += 1;
  if (int_counter == 1000) { // around each second, blink the led
    second+=1;
    int_counter = 0;
    // TODO : blink the yellow led
  } 
}



int main(void)
{
  // TODO : init yellow led + timer.
  sei(); // this is mandatory
  
  while(1)
    {
      // TODO : which task(s) ?
    }
  
  return 0;
}
