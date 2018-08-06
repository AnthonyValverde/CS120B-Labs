

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
while(1)
	{
		// 1) Read input
		tmpA = PINA & 0x01;
		// 2) Perform computation
		// if PA0/PA1 is 1/0, set
		if (tmpA == 0x01) {
			tmpA = PINA & 0x02;
			if (tmpA == 0x02) {
			tmpB = (tmpB & 0xFC) | 0x01;
		
		}
		else {
			tmpB = (tmpB & 0xFC);
		}
		}
		else {
			tmpB = (tmpB & 0xFC);
		}	
PORTB = tmpB;	
	}
	return 0;
}

