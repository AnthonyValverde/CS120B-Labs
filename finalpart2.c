void SM_TICK()
{
	switch(SM_STATE)
	{
		case SM_ZERO:
		if ((button1 == 0x00) && (button2 == 0x00))
		{
			SM_STATE = SM_ZERO;
		}
		else if ((button1 == 0x01) && (button2 == 0x00))
		{
			SM_STATE = SM_INCREMENT;
		}
		else if ((button1 == 0x00) && (button2 == 0x01))
		{
			SM_STATE = SM_DECREMENT;
		}
		else if ((button1 == 0x01) && (button2 == 0x01))
		{
			SM_STATE = SM_ZERO;
		}
		break;
		
		case SM_INCREMENT:
		if ((lightup < 6) && (cnt <= 0))
		{
			lightup = lightup + 1;
		}
		
		if ((button1 == 0x00) && (button2 == 0x00))
		{
			SM_STATE = SM_ZERO;
			cnt = 0;
		}
		else if ((button1 == 0x01) && (button2 == 0x00))
		{
			cnt = cnt +1;
			if (cnt >= 5)
			{
				cnt = 0;
			}
			SM_STATE = SM_INCREMENT;
			
			
		}
		else if ((button1 == 0x00) && (button2 == 0x01))
		{
			SM_STATE = SM_DECREMENT;
			cnt = 0;
		}
		else if ((button1 == 0x01) && (button2 == 0x01))
		{
			SM_STATE = SM_ZERO;
			cnt = 0;
		}
		break;
		
		case SM_DECREMENT:
		if ((lightup > 1) && (cnt <= 0))
		{
			lightup = lightup - 1;
		}

		if ((button1 == 0x00) && (button2 == 0x00))
		{
			SM_STATE = SM_ZERO;
			cnt = 0;
		}
		else if ((button1 == 0x01) && (button2 == 0x00))
		{
			SM_STATE = SM_INCREMENT;
			cnt = 0;
		}
		else if ((button1 == 0x00) && (button2 == 0x01))
		{
			SM_STATE = SM_DECREMENT;
			cnt = cnt + 1;
			if (cnt >= 5)
			{
				cnt = 0;
			}
		}
		
		else if ((button1 == 0x01) && (button2 == 0x01))
		{
			SM_STATE = SM_ZERO;
			cnt = 0;
		}
		break;
	}
}

void SM_TICK2()
{
	switch(SM_STATE2)
	{
		case SM2_ZERO:
		if ((button3 == 0x00) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_ZERO;
		}
		else if ((button3 == 0x01) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_INCREMENT;
		}
		else if ((button3 == 0x00) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_DECREMENT;
		}
		else if ((button3 == 0x01) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_ZERO;
		}
		break;
		
		case SM2_INCREMENT:
		if ((lightup1 < 6) && (cnt1 = 0))
		{
			lightup1 = lightup1 + 1;
		}
		
		if ((button3 == 0x00) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_ZERO;
			cnt1 = 0;
		}
		else if ((button3 == 0x01) && (button4 == 0x00))
		{
			cnt1 = cnt1 +1;
			if (cnt1 >= 5)
			{
				cnt1 = 0;
			}
			SM_STATE2 = SM2_INCREMENT;
			
			
		}
		else if ((button3 == 0x00) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_DECREMENT;
			cnt1= 0;
		}
		else if ((button3 == 0x01) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_ZERO;
			cnt1 = 0;
		}
		break;
		
		case SM2_DECREMENT:
		if ((lightup1 > 1) && (cnt1 <= 0))
		{
			lightup1 = lightup1 - 1;
		}

		if ((button3 == 0x00) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_ZERO;
			cnt1 = 0;
		}
		else if ((button3 == 0x01) && (button4 == 0x00))
		{
			SM_STATE2 = SM2_INCREMENT;
			cnt1 = 0;
		}
		else if ((button3 == 0x00) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_DECREMENT;
			cnt1 = cnt1 + 1;
			if (cnt1 >= 5)
			{
				cnt1 = 0;
			}
		}
		
		else if ((button3 == 0x01) && (button4 == 0x01))
		{
			SM_STATE2 = SM2_ZERO;
			cnt1 = 0;
		}
		break;
	}
}
void intromessage()
{	for(unsigned short i = 0;  i <= 25; ++i)
	{
		LCD_DisplayString(1, "Welcome Ping    Pong Playa");
		while (!TimerFlag);
		TimerFlag = 0;
	}
	while((button1 == 0x00) && (button2 == 0x00) && (button3 == 0x00) && (button4 == 0x00))
	{	button1 = !(PIND & 0x01);
		button2 = !(PIND & 0x02);
		button3 = !(PIND & 0x04);
		button4 = !(PIND & 0x08);
		LCD_DisplayString(1, "Press any key");
	}
}
void paddles()
{
	if (lightup == 1)
	{
		
	}
	else if (lightup == 2)
	{
		
	}
	else if (lightup == 3)
	{
		
	}
	else if (lightup == 4)
	{
		
	}
	else if (lightup == 5)
	{
		
	}
	else
	{
		
	}
}
void paddles2()
{
	if (lightup1 == 1)
	{
		
	}
	else if (lightup1 == 2)
	{
		
	}
	else if (lightup1 == 3)
	{
		
	}
	else if (lightup1 == 4)
	{
		
	}
	else if (lightup1 == 5)
	{
		
	}
	else
	{
		
	}
}
void main()
{	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xF0; PORTD = 0x0F;
	TimerSet(150);
	TimerOn();
	LCD_init();
	SM_STATE = SM_ZERO;
	SM_STATE2 = SM2_ZERO;
	lightup = 3;
	lightup1 = 3;
	intromessage();
	while(1) {
		//button input to move paddles
		button1 = !(PIND & 0x01);
		button2 = !(PIND & 0x02);
		button3 = !(PIND & 0x04);
		button4 = !(PIND & 0x08);
		SM_TICK(); // paddle1
		SM_TICK2(); // paddle2
		SM1_Tick(1);
		paddles();
		paddles2();
		//display points
		LCD_DisplayString(1, "Player 1: ");
		LCD_Cursor(11);
		LCD_WriteData('0' + player1p);
		LCD_DisplayString(17, "Player 2: ");
		LCD_Cursor(28);
		LCD_WriteData('0' + player2p);
		while (!TimerFlag);
		TimerFlag = 0;

	}
}
