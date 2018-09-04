#include <avr/io.h>
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,
	// initialize to 0s
	unsigned char spaceFree = 0x04; // Temporary variable to hold the value of B
	unsigned char spaceUsed = 0x00; // Temporary variable to hold the value of A
	while(1)
	{
		// 1) Read input
		spaceUsed = PINA & 0x0F;
		spaceFree = 4;
		// 2) Perform computation
		unsigned char tmp = 0x01;
		for(unsigned char i = 0; i < 4; i++)
		{
			if((spaceUsed & tmp) != 0)
			spaceFree = spaceFree - 0x01;
			tmp = tmp << 1;
		}
		if(spaceFree == 0)
			spaceFree = spaceFree | 0x80;
		PORTC = spaceFree;
			
	}
	return 0;
}
