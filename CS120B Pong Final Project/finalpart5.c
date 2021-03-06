include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

enum SM_STATES {SM_INCREMENT, SM_DECREMENT, SM_ZERO} SM_STATE; //buttons
enum SM_STATES2 {SM2_INCREMENT, SM2_DECREMENT, SM2_ZERO} SM_STATE2; //buttons
enum SM_STATES3 {SM_UPLEFT, SM_UPRIGHT, SM_DOWNLEFT, SM_DOWNRIGHT, SM_SRIGHT, SM_SLEFT, SM_SCORE1, SM_SCORE2} SM_STATE3;// ball direction states
volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
unsigned short lightup = 0;
unsigned short lightup1 = 0;
unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned char button3 = 0x00;
unsigned char button4 = 0x00;
unsigned short cnt = 0;
unsigned short cnt1 = 0;
unsigned short winperiod = 0;
unsigned long lcdperiod = 0;
unsigned short ballperiod = 0;
unsigned short player1p = 0;
unsigned short player2p = 0;
unsigned short i;
unsigned short j=0;
unsigned short ballyval = 0;
unsigned short ballxval = 0;

unsigned char column_sel1 = 0x00; // grounds column to display pattern

unsigned char column_sel2 = 0x00; // grounds column to display pattern

unsigned char column_sel3 = 0xF7; // grounds column to display pattern
int arr[8] = {0x01, 0x02,0x04,0x08,0x10, 0x20, 0x40,0x80};
int arr2[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
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
void physics ()
{
	ballperiod = ballperiod + 1;
	if (ballperiod >= 160)
	{
		ballperiod = 0;
		switch(SM_STATE3)
		{
			case SM_SRIGHT:
			if ((ballyval == lightup1 ) && (ballxval == 6))
			{
				SM_STATE3 = SM_UPLEFT;
			}
			else if ((ballyval == lightup1 + 2) && (ballxval == 6))
			{
				SM_STATE3 = SM_DOWNLEFT;
			}
			else if ((ballyval == lightup1 + 1) && (ballxval == 6))
			{
				SM_STATE3 = SM_SLEFT;
			}
			else if ((abs(ballyval - lightup1) >= 3 )  && (ballxval == 7))
			{
				SM_STATE3 = SM_SCORE1;
			}
			else {

				ballxval = ballxval + 1;
			}

			break;

			case SM_SLEFT:
			if ((ballyval == lightup) && (ballxval == 1))
			{
				SM_STATE3 = SM_UPRIGHT;
			}
			else if ((ballyval == lightup + 2)  && (ballxval == 1))
			{
				SM_STATE3 = SM_DOWNRIGHT;
			}
			else if ((ballyval == lightup + 1)  && (ballxval == 1))
			{
				SM_STATE3 = SM_SRIGHT;
			}
			else if ((abs(ballyval - lightup) >= 3)  && (ballxval == 1))
			{
				SM_STATE3 = SM_SCORE2;
			}
			else {

				ballxval = ballxval - 1;
			}
			break;

			case SM_DOWNRIGHT:
			if ((ballyval == lightup1 + 2)  && (ballxval == 6))
			{
				SM_STATE3 = SM_DOWNLEFT;
			}
			else if ((ballyval == lightup1)  && (ballxval == 6))
			{
				SM_STATE3 = SM_UPLEFT;
			}

			else if ((ballyval == lightup1 + 1)  && (ballxval == 6))
			{
				SM_STATE3 = SM_SLEFT;
			}
			else if ((abs(ballyval - lightup1) >= 2 )  && (ballxval == 6))
			{
				SM_STATE3 = SM_SCORE1;
			}
			else if ((ballyval == 8)  && (ballxval >=2) && (ballxval <= 7))
			{
				SM_STATE3 = SM_UPRIGHT;
			}
			else
			{
				column_sel3 = ~column_sel3;
				column_sel3 = column_sel3 << 1 ;
				column_sel3 = ~column_sel3;
				ballxval = ballxval + 1;
				ballyval = ballyval + 1;
			}

			break;

			case SM_DOWNLEFT:
			if ((ballyval == lightup + 2) && (ballxval == 1))
			{
				SM_STATE3 = SM_DOWNRIGHT;
			}
			else if ((ballyval == lightup) && (ballxval == 1))
			{
				SM_STATE3 = SM_UPRIGHT;
			}

			else if ((ballyval == lightup + 1)  && (ballxval == 1))
			{
				SM_STATE3 = SM_SRIGHT;
			}
			else if ((abs(ballyval - lightup) >= 2 ) && (ballxval == 1))
			{
				SM_STATE3 = SM_SCORE2;
			}
			else if ((ballyval == 8)  && (ballxval >=2) && (ballxval <= 7))
			{
				SM_STATE3 = SM_UPLEFT;
			}
			else
			{
				column_sel3 = ~column_sel3;
				column_sel3 = column_sel3 << 1 ;
				column_sel3 = ~column_sel3;
				ballxval = ballxval - 1;
				ballyval = ballyval + 1;
			}
			break;

			case SM_UPRIGHT:
			if ((ballyval == lightup1 + 2)  && (ballxval == 6))
			{
				SM_STATE3 = SM_DOWNLEFT;
			}
			else if ((ballyval == lightup1)  && (ballxval == 6))
			{
				SM_STATE3 = SM_UPLEFT;
			}

			else if ((ballyval == lightup1 + 1)  && (ballxval == 6))
			{
				SM_STATE3 = SM_SLEFT;
			}
			else if ((abs(ballyval - lightup1) >= 2 )  && (ballxval == 6))
			{
				SM_STATE3 = SM_SCORE1;
			}
			else if ((ballyval == 1)  && (ballxval >=2) && (ballxval <= 7))
			{
				SM_STATE3 = SM_DOWNRIGHT;
			}
			else
			{
				column_sel3 = ~column_sel3;
				column_sel3 = column_sel3 >> 1 ;
				column_sel3 = ~column_sel3;
				ballxval = ballxval + 1;
				ballyval = ballyval - 1;
			}
			break;

			case SM_UPLEFT:
			if ((ballyval == lightup + 2)  && (ballxval == 1))
			{
				SM_STATE3 = SM_DOWNRIGHT;
			}
			else if ((ballyval == lightup)  && (ballxval == 1))
			{
				SM_STATE3 = SM_UPRIGHT;
			}

			else if ((ballyval == lightup + 1)  && (ballxval == 1))
			{
				SM_STATE3 = SM_SRIGHT;
			}
			else if ((abs(ballyval - lightup) >= 2 )  && (ballxval == 1))
			{
				SM_STATE3= SM_SCORE2;
			}
			else if ((ballyval <= 1)  && (ballxval >= 2) && (ballxval <= 7))
			{
				SM_STATE3 = SM_DOWNLEFT;
			}
			else
			{
				column_sel3 = ~column_sel3;
				column_sel3 = column_sel3 >> 1 ;
				column_sel3 = ~column_sel3;
				ballxval = ballxval - 1;
				ballyval = ballyval - 1;
			}
			break;

			case SM_SCORE1:
			player1p = player1p + 1;
			ballxval = 4;
			ballyval = 4;
			column_sel3 = 0xF7;
			SM_STATE3 = SM_SLEFT;
			break;

			case SM_SCORE2:
			player2p = player2p + 1;
			ballxval = 4;
			ballyval = 4;
			column_sel3 = 0xF7;
			SM_STATE3 = SM_SRIGHT;

			break;
		}
	}
	}
void SM_TICK()
{
	switch(SM_STATE)
	{
		case SM_ZERO:
		if ((button1 == 0x00) && (button2 == 0x00))
		{
			SM_STATE = SM_ZERO;
		}
		else if ((button1 == 0x01) && (button2 == 0x00))
		{
			SM_STATE = SM_INCREMENT;
		}
		else if ((button1 == 0x00) && (button2 == 0x01))
		{
			SM_STATE = SM_DECREMENT;
		}
		else if ((button1 == 0x01) && (button2 == 0x01))
		{
			SM_STATE = SM_ZERO;
		}
		break;

		case SM_INCREMENT:
		if ((lightup < 6) && (cnt <= 0))
		{
			lightup = lightup + 1;
		}

		if ((button1 == 0x00) && (button2 == 0x00))
		{
			SM_STATE = SM_ZERO;
			cnt = 0;
		}
		else if ((button1 == 0x01) && (button2 == 0x00))
		{
			cnt = cnt +1;
			if (cnt >= 200)
			{
				cnt = 0;
			}
			SM_STATE = SM_INCREMENT;


		}
		else if ((button1 == 0x00) && (button2 == 0x01))
		{
			SM_STATE = SM_DECREMENT;
			cnt = 0;
		}
		else if ((button1 == 0x01) && (button2 == 0x01))
		{
			SM_STATE = SM_ZERO;
			cnt = 0;
		}
		break;

		case SM_DECREMENT:
		if ((lightup > 1) && (cnt <= 0))
		{
			lightup = lightup - 1;
		}

		if ((button1 == 0x00) && (button2 == 0x00))
		{
			SM_STATE = SM_ZERO;
			cnt = 0;
		}
		else if ((button1 == 0x01) && (button2 == 0x00))
		{
			SM_STATE = SM_INCREMENT;
			cnt = 0;
		}
		else if ((button1 == 0x00) && (button2 == 0x01))
		{
			SM_STATE = SM_DECREMENT;
			cnt = cnt + 1;
			if (cnt >= 200)
			{
				cnt = 0;
			}
		}

		else if ((button1 == 0x01) && (button2 == 0x01))
		{
			SM_STATE = SM_ZERO;
			cnt = 0;
		}
		break;
	}
}

void SM_TICK2()
{
	switch(SM_STATE2)
	{
		case SM2_ZERO:
		if ((button3 == 0x00) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_ZERO;
		}
		else if ((button3 == 0x01) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_INCREMENT;
		}
		else if ((button3 == 0x00) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_DECREMENT;
		}
		else if ((button3 == 0x01) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_ZERO;
		}
		break;

		case SM2_INCREMENT:
		if ((lightup1 < 6) && (cnt1 == 0))
		{
			lightup1 = lightup1 + 1;
		}

		if ((button3 == 0x00) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_ZERO;
			cnt1 = 0;
		}
		else if ((button3 == 0x01) && (button4 == 0x00))
		{
			cnt1 = cnt1 +1;
			if (cnt1 >= 200)
			{
				cnt1 = 0;
			}
			SM_STATE2 = SM2_INCREMENT;


		}
		else if ((button3 == 0x00) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_DECREMENT;
			cnt1= 0;
		}
		else if ((button3 == 0x01) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_ZERO;
			cnt1 = 0;
		}
		break;

		case SM2_DECREMENT:
		if ((lightup1 > 1) && (cnt1 <= 0))
		{
			lightup1 = lightup1 - 1;
		}

		if ((button3 == 0x00) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_ZERO;
			cnt1 = 0;
		}
		else if ((button3 == 0x01) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_INCREMENT;
			cnt1 = 0;
		}
		else if ((button3 == 0x00) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_DECREMENT;
			cnt1 = cnt1 + 1;
			if (cnt1 >= 200)
			{
				cnt1 = 0;
			}
		}

		else if ((button3 == 0x01) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_ZERO;
			cnt1 = 0;
		}
		break;
	}
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
void paddle1()
{
	///paddle1period = paddle2period + 1;
	//if (paddle1period >= 200)
	//{
	//	paddle1period = 0;
		if (lightup == 1)
		{
			column_sel1 = 0xF8;
		}
		else if (lightup == 2)
		{
			column_sel1 = 0xF1;
		}
		else if (lightup == 3)
		{
			column_sel1 = 0xE3;
		}
		else if (lightup == 4)
		{
			column_sel1 = 0xC7;
		}
		else if (lightup == 5)
		{
			column_sel1 = 0x8F;
		}
		else
		{
			column_sel1 = 0x1F;
		}
//	}
}
void paddle2()
{
//	paddle2period = paddle2period + 1;
	//if (paddle2period >= 200)
//	{
	//	paddle2period = 0;
		if (lightup1 == 1)
		{
			column_sel2 = 0xF8;
		}
		else if (lightup1 == 2)
		{
			column_sel2 = 0xF1;
		}
		else if (lightup1 == 3)
		{
			column_sel2 = 0xE3;
		}
		else if (lightup1 == 4)
		{
			column_sel2 = 0xC7;
		}
		else if (lightup1 == 5)
		{
			column_sel2 = 0x8F;
		}
		else
		{
			column_sel2 = 0x1F;
		}
	//}
}
void printtoports()
{
	if (j == 0)
	{
		PORTB = arr[j];
		PORTA = column_sel1;
	}
	else if ((j >= 1) && (j < 7))
	{
		if ( j == ballxval)
		{
			PORTB = arr[j];
			PORTA  = column_sel3;
		}
		else {
			  PORTB = arr[j];
			  PORTA  = 0xFF;
		}
	}
	else if (j == 7)
	{	PORTB = arr[j];
		PORTA = column_sel2;
		j = 0;
		return;
	} 
	j = j+ 1;
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
void main()
{	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xF0; PORTD = 0x0F;
	TimerSet(1);
	TimerOn();
	LCD_init();
	SM_STATE = SM_ZERO;
	SM_STATE2 = SM2_ZERO;
	SM_STATE3 = SM_SRIGHT;
	lightup = 3;
	lightup1 = 3;
	ballyval = 4;
	ballxval = 4;
	intromessage();
	while(1) {
		//button input to move paddles

		//LED MATRIX GAMEPLAY
		button1 = !(PIND & 0x01);
			button2 = !(PIND & 0x02);
			button3 = !(PIND & 0x04);
			button4 = !(PIND & 0x08);
		SM_TICK(); // determines lightup value
		SM_TICK2(); // determines lightup1 value
		paddle1();
		paddle2();
		printtoports();
		physics();
		//display points
		displayfunction();
		while (!TimerFlag);
		TimerFlag = 0;

	}
}
