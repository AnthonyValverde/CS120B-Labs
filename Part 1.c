#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,
	// initialize to 0s
	unsigned char amtFuel = 0x00; // intermediate variable used for port updates
	unsigned char outputLights = 0x00;
	unsigned char lowfuel =0x00;
	while(1)
	{
		// 1) Read Inputs and assign to variables
		amtFuel = PINA & 0x0F; // Mask PINA to only get the bit you are interested in
		if(amtFuel == 0x01 || amtFuel == 0x02)
		{
			outputLights = 0x20;
		}
		else if(amtFuel == 0x03 || amtFuel == 0x04)
		{
			outputLights = 0x03;
		}
		else if(amtFuel == 0x05 || amtFuel == 0x06)
		{
			outputLights = 0x38;
		}
		else if(amtFuel == 0x07 || amtFuel == 0x08 || amtFuel == 0x09)
		{
			outputLights = 0x3C;
		}
		else if(amtFuel == 0x0A || amtFuel == 0x0B || amtFuel == 0x0C)
		{
			outputLights = 0x3E;
		}
		else if(amtFuel == 0x0D || amtFuel == 0x0E || amtFuel == 0x0F)
		{
			outputLights = 0x3F;
		}
		PORTC = outputLights;
		if(amtFuel <= 0x04)
		{
			lowfuel = 0x40;
			PORTC = PORTC | lowfuel;
		}
		
	}

}