// MIF18 - advanced systems
// Arduino Lab #1 - Main File for students, to be completed
// This is a C file.

#include <avr/io.h>
#include <util/delay.h>

//Red Led on Digital 13 - 1Hz
//Button on Digital 10. Be careful while pressing the button.

int button_state=1; // button state

void init(void)
{
  // DDRB is the configuration register for digital 8 to 13
  // Set Digital 13 to "outputmode"
  DDRB |= 0b00100000;// Digital 13 "outputmode"

  // change Digital 10 to "inputmode"
  // do nothing because DDRB.2 already set to 0!
  
}


void change_led_state(){
  // Change digital 13 on->off->on (xor is life!)
  PORTB ^= 0b00100000;
}

int get_button_state(){
  // use PINB + boolean calculus to get the right state of button
  return 1;
}



int main(void)
{
  init();
  while(1) //infinite loop
    {
      button_state = get_button_state();
      if (button_state == 1)
	change_led_state();
      _delay_ms(1000);     // 1Hz period
    }
  
  return 0;
}
