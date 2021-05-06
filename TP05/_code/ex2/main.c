// MIF18 - advanced systems
// Arduino Lab #1 - Main File for students, to be completed
// This is a C file.

// Functionality : 2 Leds to blink at the same frequency

#include <avr/io.h>
#include <util/delay.h>

//Red Led on Digital 13 
//Other Led on digital 12

void init(void)
{
  // DDRB is the configuration register for digital 8 to 13
  // TODO : Set Digital 12 and 13 to "outputmode"
}



int main(void)
{
  init();
  while(1) //infinite loop
    {
      // TODO
      _delay_ms(1000);     // 1Hz period
    }
  
  return 0;
}
