/* ���������� ����������� ���������� UART ��� ����������������� ATtiny */
/* �����: ������ ���������, 2015 �. */

#ifndef _UART13_H_
#define _UART13_H_ 1

#include <avr/io.h>
#include <avr/interrupt.h>

/*
*	���� ������������� ����� � ���� ������ ������� ����� ��������������
*	��� ���������� � �������� UART.
*/

#define TXPORT PORTB	// ��� ����� ��� ��������
#define RXPORT PINB		// ��� ����� �� �����
#define TXDDR DDRB		// ������� ����������� ����� �� ��������
#define RXDDR DDRB		// ������� ����������� ����� �� �����
#define TXD 3			// ����� ���� ����� ��� ������������� �� ��������
#define RXD 1			// ����� ���� ����� ��� ������������� �� �����

/*
*	���� �������� ���������, ������������ �������� �������� ������ (�������)
*	������ BAUD_DIV �������������� ��������� �������:
*	BAUD_DIV = (CPU_CLOCK / DIV) / BAUD_RATE
*	��� CPU_CLOCK - �������� ������� �����������, BAUD_RATE - �������� �������� UART,
*	� DIV - �������� �������� ������� �������, ���������� � �������� TCCR0B.
*	��������, �������� �� 8, �������� ����� 9600 ���:
*	BAUD_DIV = (9 600 000 / 8) / 9600 = 125 (0x7D).
*/

//#define T_DIV		0x01	// DIV = 1
//#define T_DIV		0x02	// DIV = 8
//#define T_DIV		0x03	// DIV = 64
#define T_DIV		0x04	// DIV = 256
#define BAUD_DIV	0xA0	// �������� = 9600 ���.

/*
*	���� ���� ���������� ���������� ���������� � ������� ��� ������ UART
*/

volatile uint16_t txbyte;
volatile uint16_t rxbyte;
volatile uint16_t prevbyte;
volatile uint8_t txbitcount;
volatile uint8_t rxbitcount;
volatile uint8_t new_press;
volatile uint8_t activity_count;



void uart_init();

int16_t uart_recieve(uint16_t* rb);

#endif /* _UART13_H_ */
