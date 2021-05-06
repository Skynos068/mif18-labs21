// MIF18 Lab, 2019-20, Laure Gonnord
// Test the LCD device and lib.
	
#include <util/delay.h>  	// for the _delay_ms
#include "lib_lcd.h"

void init_leds(void)
{
  // Init, red led is on analog 0 = PORTC.1
  // Yellow on analog1
  DDRC |= 0b00000011; 
}


void init_lcd(void)
{
  lcd_init_4d();
  uint8_t message_lcd[15]= "@@Hello LCD!@@";
  // First clean+ wait
  lcd_write_instruction_4d(lcd_Clear);
  _delay_ms(100);
  lcd_write_string_4d(message_lcd);
}

int main(void)
{
  init_leds();
  PORTC = 0b00000011 ; // ON
  init_lcd();
  while(1)
    {
      // do nothing ! 
      _delay_ms(200);
    }
  
  return 0;
}
