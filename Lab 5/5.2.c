#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.
unsigned char lightup = 0x00;
unsigned char button = 0x00;
unsigned char count = 0x00;
// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

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
	_avr_timer_M = 30;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum SM_STATES {SM_FIRST, SM_SECOND, SM_THIRD, SM_PAUSE}SM_STATE,SM_STATE_SAVE;
void SM_TICK()
{
	switch(SM_STATE)
	{
		case SM_PAUSE:
		while (count !=0)
		{
			button = ~PINA & 0x01;
			if (button == 0x01)
			{
				count = 0;
				SM_STATE = SM_STATE_SAVE;
			}
			else
			{
				SM_STATE = SM_PAUSE;
			}
			
			break;
			
			case SM_FIRST:
			lightup =0x10;
			PORTA = lightup;
			button = ~PINA & 0x01;
			if (button == 0x01)
			{
				SM_STATE_SAVE = SM_STATE;
				count = 1;
				SM_STATE  = SM_PAUSE;
			}
			else
			{
				SM_STATE = SM_SECOND;
			}
			break;
			
			case SM_SECOND:
			lightup = 0x04;
			PORTC = lightup;
			button = ~PINA & 0x01;
			if (button == 0x01)
			{
				SM_STATE_SAVE = SM_STATE;
				count = 1;
				SM_STATE  = SM_PAUSE;
			}
			else
			{
				SM_STATE = SM_THIRD;
			}
			break;
			
			case SM_THIRD:
			lightup = 0x01;
			PORTC = lightup;
			button = ~PINA & 0x01;
			if (button == 0x01)
			{
				SM_STATE_SAVE = SM_STATE;
				count = 1;
				SM_STATE  = SM_PAUSE;
			}
			else
			{
				SM_STATE = SM_FIRST;
			}
			break;
		}
	}
}
int main(void)
{	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; // Set port B to output
	PORTC = 0x00; // Init port B to 0s
	TimerSet(1000);
	TimerOn();
	//init
	lightup = 0x10;
	PORTC = lightup;
	SM_STATE = SM_THIRD;
	while(1) {
		// User code (i.e. synchSM calls)
		// Toggle PORTB; Temporary, bad programming style
		PORTC = lightup;
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
		SM_TICK();
	}
}


