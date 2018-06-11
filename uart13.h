/* Software UART ATtiny */
/* Copiright: Nagaev Alexander, 2015.*/
/* Modified by adxx for Toyota Soarer MZ20/21 Limited steering wheel buttons */

#ifndef _UART13_H_
#define _UART13_H_ 1

#include <avr/io.h>
#include <avr/interrupt.h>

/*
*	Setup pin as RX
*/
#define RXPORT PINB		// RX port
#define RXDDR DDRB		// RX port direction - input
#define RXD 1			// Port number - 1

/*
*	Ниже задаются константы, определяющие скорость передачи данных (бодрейт)
*	расчет BAUD_DIV осуществляется следующим образом:
*	BAUD_DIV = (CPU_CLOCK / DIV) / BAUD_RATE
*	где CPU_CLOCK - тактовая частота контроллера, BAUD_RATE - желаемая скорость UART,
*	а DIV - значение делителя частоты таймера, задающееся в регистре TCCR0B.
*	Например, делитель на 8, скорость порта 9600 бод:
*	BAUD_DIV = (9 600 000 / 8) / 9600 = 125 (0x7D).
*/

//#define T_DIV		0x01	// DIV = 1
//#define T_DIV		0x02	// DIV = 8
//#define T_DIV		0x03	// DIV = 64
#define T_DIV		0x04	// DIV = 256
#define BAUD_DIV	0xA0	// Скорость = 9600 бод.

/*
*	global variables and functions for UART
*/
volatile uint16_t rxbyte;
volatile uint16_t prevbyte;
volatile uint8_t txbitcount;
volatile uint8_t rxbitcount;
volatile uint8_t new_press;
volatile uint8_t activity_count;



void uart_init();

int16_t uart_recieve(uint16_t* rb);

#endif /* _UART13_H_ */
