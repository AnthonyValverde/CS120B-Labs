#include<avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

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

void intromessage()
{	for(unsigned short i = 0;  i <= 30; ++i)
	{
		LCD_DisplayString(1, "Welcome Ping    Pong Playa!!!");
		while (!TimerFlag);
		TimerFlag = 0;
	}
	while((button1 == 0x00) && (button2 == 0x00) && (button3 == 0x00) && (button4 == 0x00))
	{	button1 = !(PIND & 0x01);
		button2 = !(PIND & 0x02);
		button3 = !(PIND & 0x04);
		button4 = !(PIND & 0x08);
		LCD_DisplayString(1, "Press any key");
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

void displayfunction()
{
	lcdperiod = lcdperiod + 1;
	if(lcdperiod == 3000000)
	{
		LCD_DisplayString(1, "Player 1: ");
		LCD_Cursor(12);
		LCD_WriteData('0' + player1p);
		LCD_DisplayString(17, "Player 2: ");
		LCD_Cursor(28);
		LCD_WriteData('0' + player2p);
		lcdperiod = 0;
	}
}
