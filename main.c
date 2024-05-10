/*
 * GccApplication1.c
 *
 * Created: 2024/4/22 19:08:30
 * Author : 22539
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"

#define KEY_PORT PORTC
#define KEY_PIN PINC
#define KEY_DDR DDRC

#define DDR DDRB
#define PORT PORTB
#define RS_PIN 0
#define RW_PIN 1
#define EN_PIN 2

typedef struct {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned short year;
} DateTime;

const char keymap[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

void keypad_init(void);
unsigned char getkey(void);
void update_time(DateTime *dt);
void display_time(const DateTime *dt);

int main(void) {
	avr_init();
	lcd_init();
	keypad_init();
	lcd_clr();
	//DDRB = 1;
    while (1) {
		//avr_wait(1000);
		//unsigned char key = getkey();
		
		
		for (int i = 0; i < 2; ++i)
		{
			lcd_pos(i,0);
			for (int j = 0; j < 15; ++j)
			{
				char a = '7';
				lcd_put(a);
			}
		}
		//lcd_puts1("ABC");
		
    }
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