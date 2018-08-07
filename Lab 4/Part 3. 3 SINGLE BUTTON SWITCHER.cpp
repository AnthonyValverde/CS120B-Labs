#include <avr/io.h>

enum SM_STATES {SM_FIRST, SM_SECOND, SM_CALC} SM_STATE;

unsigned char button = 0x00;
unsigned char lightup = 0x00;
unsigned char count = 0x00;

void SM_Tick()
{
	// Transition Tree
	switch(SM_STATE)
	{
		case SM_CALC:
		button = ~PINA & 0x01;
		count = count % 2;
		if ((button == 0x00) && (count == 0x00))
		{
			SM_STATE = SM_FIRST;
		}
		else if ((button == 0x01) && (count == 0))
		{	
			count = count +1;
			SM_STATE =  SM_SECOND;
		}
		else if ((button == 0x00) &&(count ==1))
		{
			SM_STATE = SM_SECOND;
		}
		else if ((button == 0x01) && (count == 1))
		{
			count = count +1;
			SM_STATE = SM_FIRST;
		}
		else
		{
			SM_STATE = SM_CALC;
		}
		break;
		
		case SM_FIRST:
		lightup = 0xA8;
		PORTC = lightup;
		SM_STATE = SM_CALC;
		break;
	
		case  SM_SECOND:
		lightup = 0x54;
		PORTC = lightup;
		SM_STATE = SM_CALC;
			break;
		
		
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	// Init.
	lightup = 0x3F;
	PORTC = lightup;
	count = 0x00;
	SM_STATE = SM_CALC;
	
	/* Replace with your application code */
	while (1)
	{
	
		SM_Tick();
	}
}