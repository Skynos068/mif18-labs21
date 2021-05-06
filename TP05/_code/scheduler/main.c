// Round Robin Scheduler, V1, with interruptions.
// Julien Forget, Thomas Vantroys, Laure Gonnord, Thierry Excoffier

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lib_lcd.h"

// Reminder : to write on the serial port:
// screen /dev/ttyACM0 9600 in a new terminal
// screen -a k to quit.

#define BAUDRATE	103 // UBRR value for 9600

#define NOT_STARTED	0
#define RUNNING		1

// Only for scheduler V2
#define SAVE_CONTEXT()						\
  asm volatile (						\
		"push	r0				\n\t"	\
		"in		r0, __SREG__	\n\t"		\
		"cli					\n\t"	\
		"push	r0				\n\t"	\
		"push	r1				\n\t"	\
		"push	r2				\n\t"	\
		"push	r3				\n\t"	\
		"push	r4				\n\t"	\
		"push	r5				\n\t"	\
		"push	r6				\n\t"	\
		"push	r7				\n\t"	\
		"push	r8				\n\t"	\
		"push	r9				\n\t"	\
		"push	r10				\n\t"	\
		"push	r11				\n\t"	\
		"push	r12				\n\t"	\
		"push	r13				\n\t"	\
		"push	r14				\n\t"	\
		"push	r15				\n\t"	\
		"push	r16				\n\t"	\
		"push	r17				\n\t"	\
		"push	r18				\n\t"	\
		"push	r19				\n\t"	\
		"push	r20				\n\t"	\
		"push	r21				\n\t"	\
		"push	r22				\n\t"	\
		"push	r23				\n\t"	\
		"push	r24				\n\t"	\
		"push	r25				\n\t"	\
		"push	r26				\n\t"	\
		"push	r27				\n\t"	\
		"push	r28				\n\t"	\
		"push	r29				\n\t"	\
		"push	r30				\n\t"	\
		"push	r31				\n\t"	\
		 );


#define RESTORE_CONTEXT()					\
  asm volatile (						\
		"pop	r31				\n\t"	\
		"pop	r30				\n\t"	\
		"pop	r29				\n\t"	\
		"pop	r28				\n\t"	\
		"pop	r27				\n\t"	\
		"pop	r26				\n\t"	\
		"pop	r25				\n\t"	\
		"pop	r24				\n\t"	\
		"pop	r23				\n\t"	\
		"pop	r22				\n\t"	\
		"pop	r21				\n\t"	\
		"pop	r20				\n\t"	\
		"pop	r19				\n\t"	\
		"pop	r18				\n\t"	\
		"pop	r17				\n\t"	\
		"pop	r16				\n\t"	\
		"pop	r15				\n\t"	\
		"pop	r14				\n\t"	\
		"pop	r13				\n\t"	\
		"pop	r12				\n\t"	\
		"pop	r11				\n\t"	\
		"pop	r10				\n\t"	\
		"pop	r9				\n\t"	\
		"pop	r8				\n\t"	\
		"pop	r7				\n\t"	\
		"pop	r6				\n\t"	\
		"pop	r5				\n\t"	\
		"pop	r4				\n\t"	\
		"pop	r3				\n\t"	\
		"pop	r2				\n\t"	\
		"pop	r1				\n\t"	\
		"pop	r0				\n\t"	\
		"out 	__SREG__, r0	\n\t"			\
		"pop	r0				\n\t"	\
		 );

// TASKS

//http://www.avrbeginners.net/architecture/timers/timers.html
void init_timer()
{
  TCCR2B = 0b00000100; // Clock / 256 soit 16 micro-s et WGM22 = 0
  TIMSK2 = 0b00000001; // Interruption locale autorisée par TOIE2
}

void init_led_red(void)
{
  // TODO : red led on analog 0
}

void init_led_yellow(void)
{
  // TODO : yellow led on analog 1
}

void init_serial(void)
{
  // THIS IS GIVEN
  /* ACHTUNG : we suppose UBRR value < 0xff */
  /* Not true in all case */
  uint8_t baudrate = BAUDRATE;
  /* Set baud rate */
  UBRR0H = 0;
  UBRR0L = baudrate;

  /* Enable transmitter */
  UCSR0B = (1<<TXEN0);

  /* Set frame format */
  UCSR0C = 0x06;
}


void send_serial(unsigned char c)
{
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
}


void init_task_lcd(){
  lcd_init_4d();
}


// NOW TASKS : infinite loops
void task_serial(void)
{
  // TODO : write a message on the serial port, redo, ... Do not forget to init
}

void task_led(void)
{
  // TODO : init, then blink red led (infinite loop)
}


void task_lcd(void) 
{
  // TODO : init, and send a message (infinite loop)
}


/* SCHEDULER */
typedef struct task_s {
  volatile uint8_t state;
  void (*start)(void);//code for the task
  volatile uint16_t stack_pointer;// only use in V2
} task_t;

// The third element is not useful for V1
static task_t tasks[] = {
  {NOT_STARTED, task_led, 0x500},
  {NOT_STARTED, task_serial, 0x700},
  {NOT_STARTED, task_lcd, 0x600}
};

#define NB_TASK (sizeof(tasks)/sizeof(tasks[0]))

int int_counter = 0;
volatile int second = 0;

// The isr interruption implements the scheduling activity
ISR(TIMER2_OVF_vect)
{
  static int current_task = 0 ;
  PORTC ^= 2 ; // Yellow led blinks to show its activity

  int_counter += 1;
  if (int_counter == 20) { // around each 20ms, schedule a new task
    second+=1;
    int_counter = 0;

    // TODO : implement the scheduler.
  }
}

int main(void)
{
  // Nothing to do.
  init_led_yellow();// the yellow led blinks to show the sheduler activity.
  init_timer() ;
  sei() ;
  while(1) // waits the first task, and then not useful any more.
    {
    }

  return 0;
}


