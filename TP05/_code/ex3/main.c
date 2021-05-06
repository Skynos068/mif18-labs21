// MIF18 - advanced systems
// Arduino Lab #1 - Main File for students, to be completed
// This is a C++ file.

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

// Functionality 
//Blinks the led one cycle over two.
//Now ports can be accessed by digitalWrite/digitalRead

// declaration of global variables
int led = 13;    // Synchronized with the shield led
boolean led_on;  // it is C++ you have access to boolean vars.


void setup() {               
  //Setup for LED on pin 13 
  pinMode(led, OUTPUT);
  // TODO setup for button
  // global state variables
  led_on=true;
}

void change_state_led(){
  if (led_on){
    digitalWrite(led, LOW);
  } else
    digitalWrite(led, HIGH);
  led_on = !led_on;
}

int main(void)
{
  setup();
  while(1)
    { 
      change_state_led();
      digitalWrite(a,LOW);
      _delay_ms(1000);
      
    }
  return 0;
}
