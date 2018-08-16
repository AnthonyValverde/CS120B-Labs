#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN:	setting this bit enables analog-to-digital conversion.
	// ADSC:	setting this bit starts the first conversion.
	// ADATE:	setting this bit enables auto-triggering. Since we are
	//			in Free Running Mode, a new conversion will trigger whenever
	//			the previous conversion completes.
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // Output
	DDRD = 0xFF; PORTD = 0x00; // Output
	DDRA = 0x00; PORTA = 0xFF; // Input
	
	ADC_init();
	
	/* Replace with your application code */
	while (1)
	{
		unsigned short x = ~ADC;	// Get the value from the ADC.
		PORTB = (char) x;		// Lower 8 bits.
		PORTD = (char)(x >> 8); // Upper 8 bits.
	}
}
