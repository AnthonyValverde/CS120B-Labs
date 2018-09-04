#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port B's 8 pins as outputs,
	DDRD = 0xFF; PORTD = 0x00; // Configure port B's 8 pins as outputs,
	// initialize to 0s
	unsigned char weightA = 0x04; // Temporary variable to hold the value of B
	unsigned char weightB = 0x00; // Temporary variable to hold the value of A
	unsigned char weightC = 0x00; // Temporary variable to hold the value of A
	unsigned char output = 0x00;
	unsigned char amtWeight = 0;
	while(1)
	{
		// 1) Read input
		weightA = PINA & 0xFF;
		weightB = PINB & 0xFF;
		weightC = PINC & 0xFF;
		output = 0;
		// 2) Perform computation
		if(weightA + weightB + weightC > 140)
			output = output | 0x01;
		if(abs(weightA - weightC) > 80)
			output = output | 0x02;
		
		amtWeight = weightA + weightB + weightC;
		
		amtWeight = amtWeight & 0xFC;
		
		output = output | amtWeight;
		PORTD = output;
		
	}
	return 0;
}
