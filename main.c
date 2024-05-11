/*
 * GccApplication1.c
 *
 * Created: 2024/4/22 19:08:30
 * Author : 22539
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"

#define KEY_PORT PORTC
#define KEY_PIN PINC
#define KEY_DDR DDRC


typedef struct {
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char month;
	unsigned char day;
	unsigned int year;
} DateTime;

const char keymap[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

void keypad_init(void);
unsigned char getkey(void);
void set_time(DateTime *dt);
void display_time(const DateTime *dt);
void display_timeAP(const DateTime *dt);
void tick(DateTime *time);
char wait_key(void);
unsigned char char_to_number(char c);
char check_input(DateTime* dt);

int main(void) {
	avr_init();
	lcd_init();
	keypad_init();
	lcd_clr();
	DateTime dt = {12,59,50,2,28,2001};
	char ap = 0;
	char mode = 0;
	
    while (1) {
		
		avr_wait(120);
		unsigned char key = getkey();
		if (key == 'C')
		{
			ap = 1;
		}
		else if (key == 'D')
		{
			ap = 0;
		}
		else if (key == 'A')
		{
			mode = 1;
		}
	
				
		if (!mode)
		{
			tick(&dt);
			if (ap)
			{
				display_timeAP(&dt);
			}
			else
			{
				display_time(&dt);
			}
		}
		else if (mode)
		{
			set_time(&dt);
			if (!check_input(&dt))
			{
				DateTime dtt = {12,59,50,2,28,2001};
				dt = dtt;
			}
				
			
			mode = 0;
		}
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

void display_time(const DateTime *dt) {
	char buffer[16];
	lcd_clr();
	sprintf(buffer, "%02d/%02d/%02d", dt->month, dt->day, dt->year);
	lcd_pos(0, 0);
	lcd_puts2(buffer);
	sprintf(buffer, "%02d:%02d:%02d", dt->hour, dt->minute, dt->second);
	lcd_pos(1, 0);
	lcd_puts2(buffer);
}

int is_leap_year(unsigned short year) {
	return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}
void tick(DateTime *time) {
	time->second++;
	if (time->second >= 60) {
		time->second = 0;
		time->minute++;
		if (time->minute >= 60) {
			time->minute = 0;
			time->hour++;
			if (time->hour >= 24) {
				time->hour = 0;
				time->day++;
				int days_in_month;
				switch (time->month) {
					case 2:
					days_in_month = is_leap_year(time->year) ? 29 : 28;
					break;
					case 4:
					case 6:
					case 9:
					case 11:
					days_in_month = 30;
					break;
					default:
					days_in_month = 31;
				}
				if (time->day > days_in_month) {
					time->day = 1;
					time->month++;
					if (time->month > 12) {
						time->month = 1;
						time->year++;
					}
				}
			}
		}
	}
}

void display_timeAP(const DateTime *dt) {
	char buffer[20];
	lcd_clr();
	sprintf(buffer, "%02d/%02d/%02d", dt->month, dt->day, dt->year);
	lcd_pos(0, 0);
	lcd_puts2(buffer);
	int hour = dt->hour;
	char *am_pm = "AM";
	if (hour >= 12) {
		am_pm = "PM";
		if (hour > 12)
			hour -= 12;
	}
	if (hour == 0) {
		hour = 12;
	}
	sprintf(buffer, "%02d:%02d:%02d %s", hour, dt->minute, dt->second, am_pm);
	lcd_pos(1, 0);
	lcd_puts2(buffer);
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

void set_time(DateTime *dt) {
	int row, col, num;
	lcd_clr();
	lcd_pos(0, 0);
	//lcd_puts2("Setting Time:");
	for (row = 0; row < 2; row++) {
		for (col = 0; col < 3; col++) {
			lcd_clr();
			switch (row * 3 + col) {
				case 0: // Year
				lcd_pos(0, 0);
				lcd_puts2("Enter year:");
				lcd_pos(1,0);
				num = char_to_number(wait_key());
				num = num * 10 + char_to_number(wait_key());
				num = num * 10 + char_to_number(wait_key());
				num = num * 10 + char_to_number(wait_key());
				dt->year = num;
				break;
				case 1: // Month
				lcd_pos(0, 0);
				lcd_puts2("Enter mouth:");
				lcd_pos(1,0);
				num = char_to_number(wait_key());
				num = num * 10 + char_to_number(wait_key());
				dt->month = num;
				break;
				case 2: // Day
				lcd_pos(0, 0);
				lcd_puts2("Enter day:");
				lcd_pos(1,0);
				num = char_to_number(wait_key());
				num = num * 10 + char_to_number(wait_key());
				dt->day = num;
				break;
				case 3: // Hour
				lcd_pos(0, 0);
				lcd_puts2("Enter hour:");
				lcd_pos(1,0);
				num = char_to_number(wait_key());
				num = num * 10 + char_to_number(wait_key());
				dt->hour = num;
				break;
				case 4: // Minute
				lcd_pos(0, 0);
				lcd_puts2("Enter minute:");
				lcd_pos(1,0);
				num = char_to_number(wait_key());
				num = num * 10 + char_to_number(wait_key());
				dt->minute = num;
				break;
				case 5: // Second
				lcd_pos(0, 0);
				lcd_puts2("Enter second:");
				lcd_pos(1,0);
				num = char_to_number(wait_key());
				num = num * 10 + char_to_number(wait_key());
				dt->second = num;
				break;
			}
		}
	}
}


int is_date_valid(unsigned char day, unsigned char month, unsigned int year) {
	if (month < 1 || month > 12) return 0;

	if (day < 1) return 0;
	if (month == 2) {
		if (is_leap_year(year)) {
			if (day > 29) return 0;
			} else {
			if (day > 28) return 0;
		}
		} else if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day > 30) return 0;
		} else {
		if (day > 31) return 0;
	}

	return 1;
}

int is_time_valid(unsigned char hour, unsigned char minute, unsigned char second) {
	if (hour > 23) return 0;
	if (minute > 59) return 0;
	if (second > 59) return 0;
	return 1;
}

char check_input(DateTime* dt) {
	if (is_date_valid(dt->day, dt->month, dt->year) && is_time_valid(dt->hour, dt->minute, dt->second)) {
		return 1;
		} else {
		lcd_clr();
		lcd_pos(0,0);
		lcd_puts2("Invalid!");
		avr_wait(200);
		return 0;
	}
}