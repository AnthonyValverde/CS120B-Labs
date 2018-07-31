/*
 * avalv003_lab1_part1.c
 *
 * Created: 7/31/2018 2:13:34 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
DDRA = 0x00; PORTA = 0x00; // Configure port A's 8 pins as inputs
DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
while(1)
{
    // 1) Read input
    tmpA = PINA & 0x02;
	// if Port A = 00, pb0 = 0
    if (tmpA == 0x00) {
        tmpB = 0x00;
	}
    else if (tmpA == 0x01) {
		tmpB = 0x01;
}
	else if (tmpA = 0x10)  {
		tmpB = 0x00;
    }
    else {
        tmpB = (tmpB & 0xFC) | 0x02; // Sets tmpB to bbbbbb10
        // (clear rightmost 2 bits, then set to 10)
    }
    // 3) Write output
    PORTB = tmpB;
}
return 0;
    while (1) 
    {
    }
}

