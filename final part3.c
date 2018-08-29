#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

enum SM_STATES {SM_INCREMENT, SM_DECREMENT, SM_ZERO} SM_STATE; //buttons
enum SM_STATES2 {SM2_INCREMENT, SM2_DECREMENT, SM2_ZERO} SM_STATE2; //buttons
enum SM_STATES3 {SM_UPLEFT, SM_UPRIGHT, SM_DOWNLEFT, SM_DOWNRIGHT, SM_SRIGHT, SM_SLEFT} SM_STATE3;// ball direction states
enum SM_STATES4 {SM_POS1, SM_POS2, SM_POS3, SM_POS4, SM_POS5, SM_POS6} SM_STATE4; //paddle 1 states
enum SM_STATES5 {SM2_POS1, SM2_POS2, SM2_POS3, SM2_POS4, SM2_POS5, SM2_POS6} SM_STATE5; //paddles 2 states
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
unsigned char column = 0x00;
unsigned char padrow1 = 0x00;
unsigned char padrow2 = 0x00;
unsigned short cnt = 0;
unsigned short cnt1 = 0;
unsigned short player1p = 0;
unsigned short player2p = 0;
unsigned short i;
unsigned short direction = 0;
unsigned char column_val1 = 0x01; // sets the pattern displayed on columns
unsigned char column_sel1 = 0x7F; // grounds column to display pattern
unsigned char column_val2 = 0x01; // sets the pattern displayed on columns
unsigned char column_sel2 = 0x7F; // grounds column to display pattern
unsigned char column_val3 = 0x01; // sets the pattern displayed on columns
unsigned char column_sel3 = 0x7F; // grounds column to display pattern
int arr[8] = {0x01, 0x02,0x04,0x08,0x10,0x20,0x40,0x80};

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
// ====================
// SM1: DEMO LED matrix
// ====================
enum SM1_States {sm1_display};

int SM1_Tick(int state) {

	// === Local Variables ===
	static unsigned char column_val = 0x01; // sets the pattern displayed on columns
	static unsigned char column_sel = 0xFE; // grounds column to display pattern
	
	// === Transitions ===
	switch (state) {
		case sm1_display:    break;
		default:   	        state = sm1_display;
		break;
	}
	
	// === Actions ===
	switch (state) {
		case sm1_display:   // If illuminated LED in bottom right corner
		if (column_sel == 0x7F && column_val == 0x80) {
			column_sel = 0xFE; // display far left column
			column_val = 0x01; // pattern illuminates top row
		}
		// else if far right column was last to display (grounded)
		else if (column_val == 0x80) {
			column_val = 0x01; // resets display column to far left column
			column_sel = ~column_sel;
			column_sel = column_sel << 1; // shift down illuminated LED one row
			column_sel = ~column_sel;
		}
		// else Shift displayed column one to the right
		else {
			column_val = (column_val << 1);
		}
		break;
		default:   	        break;
	}
	
	PORTB = column_val; // PORTA displays column pattern
	PORTA = column_sel; // PORTB selects column to display pattern

	return state;
};
void physics ()
{
	switch(SM_STATE3)
	{
		case SM_SRIGHT:
		column_sel3 = !(column_sel3);
		column_sel3 = column_sel3 >> 1 ;
		column_sel3 = !(column_sel3);
		break;
		case SM_SLEFT:
		column_sel3 = !(column_sel3);
		column_sel3 = column_sel3 << 1 ;
		column_sel3 = !(column_sel3);
		break;
		case SM_DOWNRIGHT:
		column_sel3 = !(column_sel3);
		column_sel3 = column_sel3 >> 1 ;
		column_sel3 = !(column_sel3);
		column_val3 = column_val3 >> 1;
		break;
		case SM_DOWNLEFT:
		column_sel3 = !(column_sel3);
		column_sel3 = column_sel3 << 1 ;
		column_sel3 = !(column_sel3);
		column_val3 = column_val3 << 1;
		break;
		case SM_UPRIGHT:
		column_sel3 = !(column_sel3);
		column_sel3 = column_sel3 >> 1 ;
		column_sel3 = !(column_sel3);
		column_val3 = column_val3 >> 1;
		break;
		case SM_UPLEFT:
		column_sel3 = !(column_sel3);
		column_sel3 = column_sel3 << 1 ;
		column_sel3 = !(column_sel3);
		column_val3 = column_val3 << 1;
		break;
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
			if (cnt >= 5)
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
			if (cnt >= 5)
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
			if (cnt1 >= 5)
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
			if (cnt1 >= 5)
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
{	for(unsigned short i = 0;  i <= 150; ++i)
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
	}
}
int paddle1(SM_STATE4)
{

		if(lightup == 1)
		{
			SM_STATE4 = SM_POS1;
		}
		else if (lightup == 2)
		{
			SM_STATE4 = SM_POS2;
		}
		else if (lightup == 3)
		{
			SM_STATE4 = SM_POS3;
		}
		else if (lightup == 4)
		{
			SM_STATE4 = SM_POS4;
		}
		else if (lightup == 5)
		{
			SM_STATE4 = SM_POS5;
		}
		else 
		{
		SM_STATE4 = SM_POS6;
	}
	switch (SM_STATE4)
	{
	case SM_POS1:
		column_sel1 = 0xF8;
	break;
	case SM_POS2:

		column_sel1 = 0xF1;
	break;
	case SM_POS3:
		column_sel1 = 0xE3;
	break;
	case SM_POS4:
	
		column_sel1 = 0xC7;
	break;
	case SM_POS5:
		column_sel1 = 0x8F;
	break;
		
		case SM_POS6:
		column_sel1 = 0x1F;
		
	break;
	}

	return SM_STATE4;
}
int paddle2(SM_STATE5)
{
		if(lightup1 == 1)
		{
			SM_STATE5 = SM2_POS1;
		}
		else if (lightup1 == 2)
		{
			SM_STATE5 = SM2_POS2;
		}
		else if (lightup1 == 3)
		{
			SM_STATE5 = SM2_POS3;
		}
		else if (lightup1 == 4)
		{
			SM_STATE5 = SM2_POS4;
		}
		else if (lightup1 == 5)
		{
			SM_STATE5 = SM2_POS5;
		}
		else {
		SM_STATE5 = SM2_POS6;
		}
	switch (SM_STATE5)
	{
	case SM2_POS1:
		column_sel2 = 0xF8;
		column_val2 = 0x80;
		PORTB = column_val2;
		PORTA = column_sel2;


	break;
	case SM2_POS2:
	for(i = 0; i < 3; ++i)
	{
		column_sel2 = 0xF1;
		column_val2 = 0x80;
		PORTB = column_val2;
		PORTA = column_sel2;

	}
	break;
	case SM2_POS3:
	
		column_sel2 = 0xE3;
		column_val2 = 0x80;
		PORTB = column_val2;
		PORTA = column_sel2;

	
	break;
	case SM2_POS4:

		column_sel2 = 0xC7;
		column_val2 = 0x80;
		PORTB = column_val2;
		PORTA = column_sel2;
	
	break;
	case SM2_POS5:
		
			column_sel2 = 0x8F;
			column_val2 = 0x80;
			PORTB = column_val2;
			PORTA = column_sel2;

		break;
		case SM2_POS6:
		
			column_sel2 = 0x1F;
			column_val2 = 0x80;
			PORTB = column_val2;
			PORTA = column_sel2;

		
		break;
	}
	return SM_STATE5;
	}
void checkrows () {
	
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
	SM_STATE4 = SM_POS3;
	SM_STATE5 = SM2_POS3;
	//SM_STATE3 - SM_SRIGHT;
	lightup = 3;
	lightup1 = 3;
	direction = 1;
	intromessage();
	while(1) {
		//button input to move paddles
		button1 = !(PIND & 0x01);
		button2 = !(PIND & 0x02);
		button3 = !(PIND & 0x04);
		button4 = !(PIND & 0x08);
		//LED MATRIX GAMEPLAY
		SM_TICK(); // determines lightup value
		SM_TICK2(); // determines lightup1 value
		paddle1(SM_STATE4);
		paddle2(SM_STATE5);
		//SM1_Tick(1);
		//display points
		LCD_DisplayString(1, "Player 1: ");
		LCD_Cursor(11);
		LCD_WriteData('0' + player1p);
		LCD_DisplayString(17, "Player 2: ");
		LCD_Cursor(28);
		LCD_WriteData('0' + player2p);
		while (!TimerFlag);
		TimerFlag = 0;

	}
}
