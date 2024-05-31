/*
 * main.c
 *
 * Created: 2024/5/29 15:21:46
 *  Author: 22539
 */ 
#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"

#define KEY_PORT PORTC
#define KEY_PIN PINC
#define KEY_DDR DDRC

void keypad_init(void);
unsigned char getkey(void);
char wait_key(void);
unsigned char char_to_number(char c);
int get_sample0();
int get_sample1();

const char keymap[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

int main(void) {
	avr_init();
	lcd_init();
	keypad_init();
	lcd_clr();
	
	char buf0[20];
	char buf1[20];
	float result;
	float min = -1;
	float max = -1;
	float sum = 0.00;
	int count = 0;
	char show = 1;
	while(1)
	{
		char key = getkey();
		if (key == 'A')
		{
			show = !show;
		}
			
		if (!show)
		{
			min = -1;
			max = -1;
			sum = 0.00;
			count = 0;
			//int pa0 = get_sample0();
			//int pa1 = get_sample1();
			int get = get_sample0();
			result = 5 * get / 1023.00;
			sprintf(buf0, "in:%.2f av:----",result);
			sprintf(buf1, "ma:---- mi:----");

			lcd_clr();
			lcd_pos(0,0);
			lcd_puts2(buf0);
			lcd_pos(1,0);
			lcd_puts2(buf1);
			avr_wait(500);
		}
		else if (show)
		{int get = get_sample0();
		result = 5 * get / 1023.00;
		if (min == -1)
		{
			min = result;
		}
		else if (min > result)
		{
			min = result;
		}
		if (max == -1)
		{
			max = result;
		}
		else if (max < result)
		{
			max = result;
		}
		sum += result;
		float aver = sum / ++count;
		sprintf(buf0, "in:%.2f av:%.2f",result, aver);
		sprintf(buf1, "ma:%.2f mi:%.2f",max, min);

		lcd_clr();
		lcd_pos(0,0);
		lcd_puts2(buf0);
		lcd_pos(1,0);
		lcd_puts2(buf1);
		avr_wait(500);
		}
	}
}

int get_sample0()
{
	ADMUX = 0b01000000;
	ADCSRA = 0b11000000;
	while (GET_BIT(ADCSRA,6));
	int l = ADCL;
	return (ADCH << 8) + l;
}

int get_sample1()
{
	ADMUX = 0b01000100;
	ADCSRA = 0b11000000;
	while (GET_BIT(ADCSRA,6));
	int l = ADCL;
	return (ADCH << 8) + l;
}


void keypad_init(void)
{
	// out:0123 in: 4567
	KEY_DDR = 0x0F;
}

unsigned char getkey(void)
{
	for (int i = 0; i < 4; ++i)
	{
		KEY_PORT = ~(1<<i);
		for (int j = 0;j < 4; ++j)
		{
			if (!(KEY_PIN & (1<<(j+4))))
			{
				return keymap[i][j];
			}
		}
	}
	return 0;
}

char wait_key(void)
{
	char k = 0;
	while (!k)
	{
		avr_wait(50);
		k = getkey();
	}
	lcd_put(k);
	avr_wait(50);
	return k;
}

unsigned char char_to_number(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
		} else {
		return 0;
	}
}
