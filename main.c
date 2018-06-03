/*
 * Soarer_steering_wheel_buttons.c
 *
 * Created: 29.05.2018 17:10:41
 * Author : adxx
 */ 

//DEC 912, 1110010000	Climate On-Off
//DEC 816, 1100110000	Climate ? AC? 
//DEC 672, 1010100000	Climate Up
//DEC 688, 1010110000	Climate Down

//DEC 560, 1000110000	Radio On Off
//DEC 592, 1001010000	AM
//DEC 688, 1010110000	FM
//DEC 624, 1001110000	Radio Up
//DEC 656, 1010010000	Radio Dwn
//DEC 720, 1011010000	Volume Up
//DEC 752, 1011110000	Volume Dwn


#define F_CPU 9500000UL

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
void nec(char ad, char com);
char analiz_button(char ch_1, char ch_2);

void nec_2(char necc)
{
	for (i=0;i<8;i++)
	{
		if ((necc&(1<<i))!=0)
		{
			one;
		}
		else
		{
			nul;
		}
	}
}

void nec(char ad, char com)
{
	ad_in=~ad;
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
	//uint8_t b = 0x4D;
	//uint8_t id = 0b00000100;
	uart_init();
	while (1)
	{
		if(new_press == 1){ //проверяем наличие новой команды от кнопок. 1 - если новое нажатие есть
			new_press = 0;		//сбрасываем флаг нового нажатия, теперь если что придет новое - даст знать
			if(rxbyte = )
			
		}
		//if(uart_recieve(&b) >= 0)	// ?сли ничего не прин¤ли, ничего и не передаем
		//uart_send(b);		// ј если прин¤ли, передаем прин¤тое
		nec(0xB9,0x0A);
		_delay_ms(500);
	}
	return (0);
}

