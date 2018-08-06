#include <avr/io.h>

enum SM_STATES{SM_WAIT_FOR_BUTTON, SM_WAIT_FOR_INC_FALL, SM_WAIT_FOR_DEC_FALL, SM_WAIT_BOTH_FALL} SM_STATE;

unsigned char A = 0x00;
unsigned char C = 0x00;

void SM_Tick()
{
	// Transition Tree
	switch(SM_STATE)
	{
		case SM_WAIT_BOTH_FALL:
		if(A == 0x00)
		SM_STATE = SM_WAIT_FOR_BUTTON;
		else
		SM_STATE = SM_WAIT_BOTH_FALL;
		break;
		
		case SM_WAIT_FOR_BUTTON:
		if(A == 0x01)
		{
			if(C < 9)
			C++;
			SM_STATE = SM_WAIT_FOR_INC_FALL;
		}
		else if(A == 0x02)
		{
			if(C > 0)
			C--;
			SM_STATE = SM_WAIT_FOR_DEC_FALL;
		}
		break;
		
		case SM_WAIT_FOR_INC_FALL:
		if(A == 0x00 || A == 0x02)
		SM_STATE = SM_WAIT_FOR_BUTTON;
		else if(A == 0x03)
		{
			C = 0;
			SM_STATE = SM_WAIT_BOTH_FALL;
		}
		break;
		
		case SM_WAIT_FOR_DEC_FALL:
		if(A == 0x00 || A == 0x01)
		SM_STATE = SM_WAIT_FOR_BUTTON;
		else if(A == 0x03)
		{
			C = 0;
			SM_STATE = SM_WAIT_BOTH_FALL;
		}
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	// Init.
	SM_STATE = SM_WAIT_FOR_BUTTON;
	C = 0x07;
	PORTC = C;
	
	/* Replace with your application code */
	while (1)
	{
		A = PINA & 0x03;
		SM_Tick();
		PORTC = C;
	}
}