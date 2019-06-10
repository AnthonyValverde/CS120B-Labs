#include<avr/io.h>
#include <avr/interrupt.h>
#include "io.c"


void displayfunction()
{
	lcdperiod = lcdperiod + 1;
	if(lcdperiod == 3000000)
	{
		LCD_DisplayString(1, "Player 1: ");
		LCD_Cursor(12);
		LCD_WriteData('0' + player1p);
		LCD_DisplayString(17, "Player 2: ");
		LCD_Cursor(28);
		LCD_WriteData('0' + player2p);
		lcdperiod = 0;
	}
}
