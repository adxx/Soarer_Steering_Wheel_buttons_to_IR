/*
 * Soarer_steering_wheel_buttons.c
 *
 * Created: 29.05.2018 17:10:41
 * Author : adxx
 */ 

//DEC 912, 1110010000	Climate On-Off	=> 64623, 1111 1100 0110 1111 
//DEC 816, 1100110000	Climate ? AC?	=> 64719, 1111 1100 1100 1111
//DEC 672, 1010100000	Climate Up		=> 64863, 1111 1101 0101 1111
//DEC 688, 1010110000	Climate Down	=> 64847, 1111 1101 0100 1111

//DEC 560, 1000110000	Radio On Off	=> 64975, 1111 1101 1100 1111
//DEC 592, 1001010000	AM				=> 64943, 1111 1101 1010 1111
//DEC 688, 1010110000	FM				=> 64847
//DEC 720, 1011010000	Radio Up		=> 64815
//DEC 752, 1011110000	Radio Dwn		=> 64783
//DEC 624, 1001110000	Volume Up		=> 64911
//DEC 656, 1010010000	Volume Dwn		=> 64879


#define F_CPU 6300000UL //корректируем кривой генератор китайской тиньки

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart13.h"

#define on PORTB|=(1<<0)
#define off PORTB&=~(1<<0)
#define st PORTB|=(1<<0);_delay_us(8950);PORTB&=~(1<<0);_delay_us(4450)
#define one PORTB|=(1<<0);_delay_us(560);PORTB&=~(1<<0);_delay_us(1625)
#define nul PORTB|=(1<<0);_delay_us(560);PORTB&=~(1<<0);_delay_us(550)

unsigned char i, ad_in, k, but, com_in, n;
volatile char cha_1, cha_2;
void nec_2(char necc);
void nec(char ad, char ad_in, char com);


void nec_2(char necc)
{
	for (i=8;i>0;i--)
	{
		if ((necc&(1<<(i-1)))!=0)
		{
			one;
		}
		else
		{
			nul;
		}
	}
}

void nec(char ad, char ad_in, char com)
{
	//ad_in=~ad;
	com_in=~com;
	st;
	nec_2(ad);
	nec_2(ad_in);
	nec_2(com);
	nec_2(com_in);
	one;
	_delay_ms(250);
}

int main(void)
{
	activity_count = 0;
	OSCCAL = 0x7E; //корректируем кривой генератор китайской тиньки ч.2
	DDRB = (1 << DDB0) | (1 << DDB2);
	uart_init();
	while (1)
	{
		if (new_press == 1){
			if(rxbyte == prevbyte) {
				if(rxbyte == 64975) {		//Power button 
					nec(0x61,0x4e,0x90);
					PORTB ^= 1 << PINB2;
				}
				if(rxbyte == 64943) {		//AM - lets make it "source"
					nec(0x61,0x4e,0x50);
					PORTB ^= 1 << PINB2;
				}
				if(rxbyte == 64847) {		//FM - lets make it "band"
					nec(0x61,0x4e,0x61);
					PORTB ^= 1 << PINB2;
				}
				if(rxbyte == 64783) {		//Step dwn
					nec(0x61,0x4e,0xC8);
					PORTB ^= 1 << PINB2;
				}
				if(rxbyte == 64815) {		//Step up
					nec(0x61,0x4e,0x48);
					PORTB ^= 1 << PINB2;
				}
				if(rxbyte == 64879) {		//Volume Down
					nec(0x61,0x4e,0xA8);
					PORTB ^= 1 << PINB2;
				}
				if(rxbyte == 64911) {		//Volume Up
					nec(0x61,0x4e,0x28);
					PORTB ^= 1 << PINB2;
				}
				
				
				
			}
			new_press = 0;
		}
	}
	return (0);
}

