#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

enum SM_STATES {SM_INCREMENT, SM_DECREMENT, SM_ZERO} SM_STATE;
volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
unsigned short lightup = 0;
unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned short cnt = 0;

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
void SM_TICK()
{
	switch(SM_STATE)
	{
		case SM_ZERO:
		if (!button1 && !button2)
		{ 
			SM_STATE = SM_ZERO;
		}
		else if (button1 && !button2)
		{
			SM_STATE = SM_INCREMENT;
		}
		else if (!button1 && button2)
		{
			SM_STATE = SM_DECREMENT;
		}
		else if (button1 && button2)
		{
			lightup = 0;
			SM_STATE = SM_ZERO;
		}
		break;
		
		case SM_INCREMENT:
		if ((lightup < 9) && (cnt <= 0))
		{
			lightup = lightup + 1;
		}
		
		if (!button1 && !button2)
		{
			SM_STATE = SM_ZERO;
				cnt = 0;
		}
		else if (button1 && !button2)
		{
			cnt = cnt +1;
			if (cnt >= 10)
			{
			cnt = 0;
			}
			SM_STATE = SM_INCREMENT;
			
			
		}
		else if (!button1 && button2)
		{
			SM_STATE = SM_DECREMENT;
				cnt = 0;
		}
		else if (button1 && button2)
		{
			lightup = 0;
			SM_STATE = SM_ZERO;
				cnt = 0;
		}
		break;
		
		case SM_DECREMENT:
		if ((lightup > 0) && (cnt <= 0))
		{
			lightup = lightup - 1;
		}

		if (!button1 && !button2)
		{
			SM_STATE = SM_ZERO;
				cnt = 0;
		}
		else if (button1 == 1 && button2 == 0)
		{
			SM_STATE = SM_INCREMENT;
				cnt = 0;
		}
		else if (button1 == 0 && button2 == 1)
		{
			SM_STATE = SM_DECREMENT;
			cnt = cnt + 1;
			if (cnt >= 10)
			{
					cnt = 0;
				}
			}
	
		else if (button1 == 1 && button2 == 1)
		{
			lightup = 0;
			SM_STATE = SM_ZERO;
				cnt = 0;
		}
		break;
	}
}
void main()
{	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(150);
	TimerOn();
	LCD_init();
	SM_STATE = SM_ZERO;
	lightup = 0;
	button1 = !(PINA & 0x01);
	button2 = !(PINA & 0x02);
	
	while(1) {
		button1 = !(PINA & 0x01);
		button2 = !(PINA & 0x02);
		SM_TICK();
		LCD_Cursor(1);
		LCD_WriteData('0' + lightup);
		while (!TimerFlag);
		TimerFlag = 0;

	}
}
