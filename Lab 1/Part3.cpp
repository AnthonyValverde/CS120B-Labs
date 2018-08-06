#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	unsigned char tmpC = 0x00;
	unsigned char tmpA = 0x00;
while(1)
	{	unsigned char cntavail = 0x04; 
		// check if port 1 is full
		tmpA = (tmpA & 0x00) | (PINA & 0x01);
		if (tmpA == 0x01) {
		cntavail = cntavail - 0x01;			
		}
		// check if port 2 is full
		tmpA = (tmpA & 0x00) | (PINA & 0x02);
		if (tmpA == 0x02) {
		cntavail = cntavail - 0x01;			
		}
		// check if port 3 is full
		tmpA = (tmpA & 0x00) | (PINA & 0x04);
		if (tmpA == 0x04) {
		cntavail = cntavail - 0x01;			
		}
		// check if port 4 is full
		tmpA = (tmpA & 0x00) | (PINA & 0x08);
		if (tmpA == 0x08) {
		cntavail = cntavail - 0x01;			
		}
			
PORTC = cntavail;	
	if (cntavail == 0)
	{
	tmpC = 0x80;	
	PORTC = tmpC | PORTC;
	}
	}
	return 0;
}


