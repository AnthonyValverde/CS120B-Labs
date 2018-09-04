#include <avr/io.h>
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs,
	// initialize to 0s
	unsigned char door = 0x00; // Temporary variable to hold the value of B
	unsigned char light = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of A
	while(1)
	{
		// 1) Read input
		door = PINA & 0x01;
		light = PINA & 0x02;
		// 2) Perform computation
		// if PA0 is 1, set PB1PB0=01, else =10
		if (door == 0x01 && light != 0x02) { // True if PA0 is 1
			tmpB = (tmpB & 0xFC) | 0x01; // Sets tmpB to bbbbbb01
			// (clear rightmost 2 bits, then set to 01)
		}
		else {
			tmpB = (tmpB & 0xFC) | 0x00; // Sets tmpB to bbbbbb10
			// (clear rightmost 2 bits, then set to 10)
		}
		// 3) Write output
		PORTB = tmpB;
	}
	return 0;
}
