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

#define SP_PORT PORTB

#include <avr/wdt.h>

#define A 440
#define As 466
#define B 494
#define C 523
#define Cs 554
#define D 587
#define Ds 622
#define Ee 659
#define F 698
#define Fs 740
#define G 784
#define Gs 831
#define hA 880
#define hAs 932
#define hB 988
#define hC 1046
#define hCs 1109
#define hD 1175
#define hDs 1244
#define hEe 1318
#define hF 1397
#define hFs 1480
#define hG 1568
#define hGs 1161
#define lA 220
#define lAs 233
#define lB 247
#define lC 262
#define lCs 277
#define lD 294
#define lDs 311
#define lEe 330
#define lF 349
#define lFs 370
#define lG 392
#define lGs 415

int notelist[] = {A, As, B, C, Cs, D, Ds, Ee, F, Fs, G, Gs, hA, hAs, hB, hC, hCs, hD, hDs, hEe, hF, hFs, hG, hGs, lA, lAs, lB, lC, lCs, lD, lDs, lEe, lF, lFs, lG, lGs};

void keypad_init(void);
unsigned char getkey(void);
char wait_key(void);
unsigned char char_to_number(char c);

void Wait(unsigned long len)
{
	volatile unsigned long i; 
	for (i = 0; i < len; ++i)
	{
	}
}

typedef enum
{
	W = 16,
	H = 8,
	Q = 4,
	Ei = 2,
	T = 3,
	U = 1
} Duration;

typedef struct 
{
	int note;
	Duration duration;
} PlayNote;


typedef struct 
{
	PlayNote *song;
	int length;
	int speed;
	char* name;
} songinfo;


void play_note(const PlayNote* note, unsigned long u)
{
	if (note->note == 0)
	{
		unsigned long dura = u * note->duration * 2;
		for (unsigned long i = 0; i < dura; ++i)
		{
			CLR_BIT(SP_PORT,3);
			wdt_reset();
		}
	}
	else
	{
		unsigned long dura = u * note->duration;
		int waitt = 250000 / note->note / 2;
		for (unsigned long i = 0; i < dura/(waitt*2); ++i)
		{
			SET_BIT(SP_PORT,3);
			Wait(waitt);
			CLR_BIT(SP_PORT,3);
			Wait(waitt);
			wdt_reset();
		}
	}
}

char play_song(const PlayNote song[], int length, int speed)
{
	unsigned long u = 15000000 / speed / Q;
	for (int i = 0; i < length; ++i)
	{
		play_note(&song[i],u);
		char key = getkey();
		if (key)
		{
			return key;
		}
	}
}

const char keymap[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};


int main(void) {
	int offset = 0;


	avr_init();
	lcd_init();
	keypad_init();
	lcd_clr();
	DDRB |= (1<<3);
	wdt_enable(9);
	
	char playing = 1;
	float spdoffset = 1;
	char key;
	
	while(1)
	{
		PlayNote bmzy[] = {
			{notelist[7+offset],Q},{notelist[5+offset],Q},{notelist[3+offset],Q},{notelist[2+offset],Q},{notelist[24+offset],T},{notelist[26+offset],T},{notelist[3+offset],Q},{notelist[24+offset],Ei},
			{0,Ei},{notelist[24+offset],Ei},{notelist[7+offset],Q},{notelist[5+offset],Q},{notelist[3+offset],Q},{notelist[3+offset],Q},{notelist[26+offset],T},
			{notelist[24+offset],U},{notelist[24+offset],Ei},{notelist[3+offset],Ei},{notelist[3+offset],Q},{0,Ei},{notelist[26+offset],Ei},{notelist[10+offset],Q},
			{notelist[8+offset],T},{notelist[7+offset],U},{notelist[7+offset],Ei},{notelist[3+offset],Ei},{notelist[3+offset],Ei},{notelist[3+offset],U},{notelist[5+offset],U},{notelist[3+offset],Ei},{notelist[5+offset],Ei},
			{notelist[7+offset],Q},{notelist[3+offset],Ei},{notelist[26+offset],Ei},{notelist[3+offset],Ei},{notelist[26+offset],Ei},{notelist[5+offset],T},{notelist[7+offset],U},{notelist[7+offset],Ei},
			{notelist[5+offset],Ei},{0,Q},{notelist[5+offset],Ei},{notelist[7+offset],Ei},{notelist[7+offset],Ei},{notelist[3+offset],Ei},{notelist[3+offset],Q},{notelist[3+offset],Q}

		};
		
		PlayNote sykxmyas[] = {
			{notelist[7+offset], Ei}, {notelist[8+offset], Ei}, {notelist[8+offset], Ei}, {notelist[8+offset], Ei}, {notelist[7+offset], U}, {notelist[8+offset], U}, {notelist[8+offset], Q}, {0, Ei},
			{notelist[7+offset], Ei}, {notelist[8+offset], Ei}, {notelist[8+offset], Ei}, {notelist[8+offset], Ei}, {notelist[7+offset], U}, {notelist[8+offset], U}, {notelist[7+offset], Q},
			{0, Ei}, {notelist[3+offset], Ei}, {notelist[5+offset],Ei}, {notelist[3+offset], U}, {notelist[5+offset], U}, {notelist[5+offset], U}, {0, Ei},
			{notelist[5+offset], U},{notelist[5+offset], Q}, {notelist[5+offset], T}, {notelist[7+offset], U}, {notelist[7+offset], Q}, {notelist[7+offset], Q}, {0, Q}, {0, Q}
		};
		
		PlayNote secretbase[] =  {
			{notelist[2+offset], Ei}, {notelist[15+offset], Ei}, {notelist[17+offset], U}, {0, U}, {notelist[17+offset], U}, {0,U}, {notelist[21+offset], U}, {0,U}, {notelist[21+offset], U}, {0,U}, {notelist[21+offset], U}, 
			{0,U}, {notelist[21+offset], U}, {0,U}, {notelist[21+offset], T}, {0,U}, {notelist[21+offset], U}, {0,U}, {notelist[21+offset], U}, {0,U}, {notelist[17+offset], U}, {0,U},
			{notelist[17+offset], U}, {0,U},{notelist[17+offset], U}, {0,U},{notelist[17+offset], U}, {0,U}, {notelist[17+offset], T}, {0,U}, {notelist[17+offset], U}, {0,U}, 
			{notelist[17+offset], U}, {0,U}, {notelist[15+offset], U}, {0,U}, {notelist[15+offset], U}, {0,U},{notelist[15+offset], U}, {0,U}, {notelist[15+offset], U}, {0,U}, 
			{notelist[15+offset], T}, {0,U}, {notelist[15+offset], U}, {0,U}, {notelist[10+offset], U}, {0,U}, {notelist[10+offset], U}, {0,U}, {notelist[10+offset], U}, {0,U}, 
			{0, Ei}, {notelist[10+offset], U}, {0,U}, {notelist[15+offset], U}, {0,U}, {notelist[17+offset], U}, {0,U}, {notelist[21+offset], U}, {0,U}, 
			{notelist[21+offset], U}, {0,U}, {notelist[21+offset], U}, {0,U}, {notelist[21+offset], U}, {0,U}, {notelist[21+offset], U}, {0,U}, {notelist[21+offset], U}, 
			{0,U},  {notelist[21+offset], U}, {0,U}, {notelist[22+offset], Ei}, {0,Ei}, {notelist[21+offset], U}, {0,U},{notelist[21+offset], U}, {0,U},
			{notelist[21+offset], U}, {0,U}, {notelist[21+offset], U}, {0,U},{notelist[21+offset], U}, {0,U}, {notelist[17+offset], U}, {0,U}, {notelist[15+offset], U}, 
			{0,U}, {notelist[17+offset], Q}, {notelist[21+offset], U}, {0,U},{notelist[21+offset], U}, {0,U}};
				
		songinfo songlist[3] = {
			{bmzy,47,120,"BMZY"},
			{sykxmyas,30,80,"sykxmyas"},
			{secretbase,94,120,"secret base"}
		};
		
		if (playing)
		{
			lcd_clr();
			lcd_pos(0,0);
			lcd_puts2(songlist[playing-1].name);
			key = play_song(songlist[playing-1].song,songlist[playing-1].length,songlist[playing-1].speed * spdoffset);
			if (key == 'A')
			{
				playing = 0;
				avr_wait(500);
			}
			else if (key =='1')
			{
				playing = 1;
				avr_wait(500);
			}
			else if (key == '2')
			{
				playing = 2;
				avr_wait(500);
			}
			if (key == '3')
			{
				playing = 3;
				avr_wait(500);
			}
			if (key == 'B')
			{
				spdoffset *= 2;
				avr_wait(500);
			}
			if (key == 'C')
			{
				spdoffset /= 2;
				avr_wait(500);
			}
			if (key == '*')
			{
				offset += 1;
			}
			if (key == '#')
			{
				offset -= 1;
			}
		}
		else
		{
			lcd_clr();
			lcd_pos(0,0);
			lcd_puts2("SPOTTED");
			char key = wait_key();
			if (key == 'A')
			{
				playing = 1;
			}
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
