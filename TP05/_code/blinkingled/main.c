/* LAB 4 Arduino, testing code + Board */
/* Blinking Led example */

#include <avr/io.h>
#include <util/delay.h>

//Red Led on Digital 13 - 1Hz

void init(void)
{
  // DDRB is the configuration register for digital 7 to 18
  // Set Digital 13 to "outputmode"
  DDRB |= 0b00100000;// Digital 13 "outputmode"
}


void change_led_state(){
  // Change digital 13 on->off->on (xor is life!)
  PORTB ^= 0b00100000;
}



int main(void)
{
  init();
  while(1) //infinite loop
    {
      change_led_state();
      _delay_ms(1000);     // 1Hz period
    }
  
  return 0;
}
