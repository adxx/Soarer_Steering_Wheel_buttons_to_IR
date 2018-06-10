#include "uart13.h"

/*
*	ISR(TIM0_COMPA_vect)
*	���������� ������� �� ��������� � ��������� OCR0A.	��� �������� ��������� � ��������� ������ ���, �����
*	������ ��������� ��������, ����������� � �������� OCR0A. ����� ��� ����������, �������� ������� � ��������
*	TCNT0 ������������� ����������.
*	��� ���������� ������������ ��� �������� ������ �� UART. ���������� txbyte ������������ ��� ��������� �������:
*	������ ���, ����� ���������� ����������, ������� ������ ���������� txbyte ������������ �� ����� TXD ����������,
*	����� ���� ���������� ����� ����������� ���������� ������, � � �������������� ������� ������ ������������
*	���������� �������. ���� ������ ��� �������� ���, � ���������� �������� ����� 0xFFFF �, ��� �����, �� ������
*	TXD ���������� �������������� ������� ���������� �������. ����� �� ����� �������� ������, �� ������ ��������
*	� ������� ��� ����� ��� ��� ��������: 1 ���������, 8 ��� ������ � 1 ��������, ����� 10 (0x0A), � ��������
*	� txbyte ������ ��� �������� ������ �� ��������� �����. ����� ����� ��� ���������� ������ ������������.
*	������������� ������� ���������� ������� void uart_send(uint8_t tb).
*/

ISR(TIM0_COMPA_vect)
{
	if (activity_count !=0) {
		activity_count ++;
		if (activity_count >=12) {
			new_press = 1;				// ������������� ���� ���������� ������� �������	
			activity_count = 0;
		}
	}
	//TXPORT = (TXPORT & ~(1 << TXD)) | ((txbyte & 0x01) << TXD); // ���������� � ��� TXD ������� ��� txbyte
	//txbyte = (txbyte >> 0x01) + 0x8000;							// ������� txbyte ������ �� 1 � ����� 1 � ������� ������ (0x8000)
	//if(txbitcount > 0)											// ���� ���� �������� (������ ��� ������ ����),
	//{
		//txbitcount--;											// �� ��������� ��� �� �������.
	//}
	
}

/*
*	void uart_send(uint8_t tb)
*	������� �������� ����� �� UART. ��������� � �������� ��������� ���� ��� ��������, ������������� �������� ���.
*	���� � ������ ������ ���� �������� �����, �� ��� ���� ���� ��������	���������� ����� ���� ���������� �
*	������� 8 ��� ���������� txbyte ���� ��� ��������, ������� 8 ��� �������� 0xFF, ����� �������� ����������
*	�����, �������� ����� ������� ��������� ��� � ������� �������. ������ ������� ��� = 10.
*/

void uart_send(uint16_t tb)
{
	while(txbitcount);				// ���� ���� ���������� �������� ����������� �����
	txbyte = (tb-0x8001);// << 0x01; // ����� � ������� ������� txbyte ������ ��� �������� � �������� ����� �� 1
	txbitcount = 0x10;				// ������ ������� ���� ������ 16
}


/*
*	ISR(TIM0_COMPB_vect)
*	���������� ������� �� ��������� � ��������� OCR0B. ��� �������� ���������� ���������� ISR(TIM0_COMPA_vect),
*	��, � ������� �� ����, ��� ���������� ����� ���������� �� ���������� ��������� ������� TCNT0. ��� ����������
*	����������� ������ ����� �� ��������� ������, � ��������� ����� ��� ���������� ���������. ����� ��� ���������,
*	�� ��������� ���������� ��������� ����� UART RXD �, ���� ��� � �������, �� ����� ������� � ������� ������
*	���������� ������ rxbyte, ����� �� ��������� �� ������� ������� �������� ��� �, ���� �� ���� �����, �����������
*	�����. ����� �������� ������ ���������� rxbyte, ����� ����������� �� � ������ ���������� ����.
*/

ISR(TIM0_COMPB_vect)
{
	if(RXPORT & (1 << RXD))			// ��������� � ����� ��������� ���� RXD
		rxbyte |= 0x01;				// ���� � 1, �� ����� 1 � ������� ������ rxbyte
	if(--rxbitcount == 0)			// ��������� �� 1 ������� ��� � ��������� �� ���� �� �� �����
	{
		TIMSK0 &= ~(1 << OCIE0B);	// ���� ��, ��������� ���������� �� ��������� OCR0B
		TIFR0 |= (1 << OCF0B);		// ������� ���� ���������� (�����!)
		GIFR |= (1 << INTF0);		// ������� ���� ���������� �� INT0
		GIMSK |= (1 << INT0);		// ��������� ���������� INT0		
	}
	else
	{
		rxbyte <<= 0x01;			// ����� �������� ����� �� 1 rxbyte
	}
}

/*
*	ISR(INT0_vect)
*	���������� INT0. ����������� �� ������� ������ �������� �� ����� INT0, ������������
*	��� ������������ ������ ������ ����������. ���������� ������� ��� ������ 9, ��������
*	���������� ���������� rxbyte. ������ �������� ��� �������� OCR0B, ��������� �������������
*	������������ ���������� ISR(TIM0_COMPB_vect), ��� ������ ����������� �� �������� ������������
*	���� (�� �������). ����� ���� ���������� ISR(TIM0_COMPB_vect) �����������, � ���������� INT0
*	�����������.
*/

ISR(INT0_vect)
{
	prevbyte = rxbyte;
	rxbitcount = 0x0A;						// 10 ��� ������
	rxbyte = 0xFFFE;							// ���������� rxbyte � �������
	if(TCNT0 < (BAUD_DIV / 2))				// ���� ������ �� �������� �� �������� �������� �������
	{
		OCR0B = TCNT0 + (BAUD_DIV / 2);	// �� ���������� ���������� � ������� ������� ������ ��� �������
	}
	else
	{
		OCR0B = TCNT0 - (BAUD_DIV / 2);	// ����� ���������� ���������� � ��� ��������� ������� �������
	}
	GIMSK &= ~(1 << INT0);					// ��������� ���������� �� INT0
	TIFR0 |= (1 << OCF0A) | (1 << OCF0B);	// ������� ���� ���������� INT0
	TIMSK0 |= (1 << OCIE0B);				// ��������� ���������� �� OCR0B
	activity_count = 1;						// ������������� � �������� ��������� �������� ����������, ������ 12 ����� - �� 2 ������ ��� �������
											// - ��� ������� �������� ����� �������� ������. 
	
}


/*
*	int16_t uart_recieve(uint8_t* rb)
*	������� ������ ����� �� UART. ��������� � �������� ��������� �� 8-������ ����������, ��� ����� �����������
*	�������� ����. ���������� �������� ����, ���� ���� ������, ���������� (-1), ���� ��������� ������.
*	���� � ������ ������ ������� ���� �����, ������� ����� ����� ��� ����������. ���� ������� ������� ������,
*	�� ������ ��� ��� ��������� �������� ����, ������ ��� (-1).
*/

int16_t uart_recieve(uint16_t* rb)
{
	if(rxbitcount < 0x0A)	// ���� ������� ��� �� ����� ������ 10
	{
		while(rxbitcount);	// ���� ���� ���������� ������� �����
		*rb = rxbyte;		// ����� �� ������ ��������� �������� ����
		rxbitcount = 0x0B;	// ��������������� �������� �������� ���
		return (*rb);		// ������������
	}
	else
	{
		return (-1);		// ����� ���������� -1 (��������� ������)
	}
}

/*
*	void uart_init()
*	������� ������������� UART. ���������� ���, ������������� �������� ���.
*	�������������� ���������� ���������� � �������� ����������������.
*/

void uart_init()
{
	txbyte = 0xFFFF;		// �������� ������ �� �������� - ��� �������
	rxbyte = 0xFFFF;			// �������� ������ �� ����� - ��� ����
	txbitcount = 0x00;		// �������� �������� ������������ ��� - ���� (������ ���� �� ��������)
	rxbitcount = 0x0A;		// �������� �������� ��� �� ����� - 10 (������� ���������� ������)
	
	TXDDR |= (1 << TXD);		// ������ ����������� ����� �� �������� ��� �����
	RXDDR &= ~(1 << RXD);		// ������ ����������� ����� �� ����� ��� ����
	TXPORT |= (1 << TXD);		// ����� ������� � ����� TXD
	RXPORT |= (1 << RXD);		// ����������� � ������� ���� RXD
	OCR0A = BAUD_DIV;			// ������ �������� �������� OCR0A � ������������ � ���������
	TIMSK0 |= (1 << OCIE0A);	// ��������� ���������� TIM0_COMPA
	TCCR0A |= (1 << WGM01);		// ����� ������� CTC (������� TCNT0 �� ���������� OCR0A)
	TCCR0B |= T_DIV;			// ������ �������� ����� ������� � ������������ � ���������
	MCUCR |= (1 << ISC01);		// ������ ���������� INT0 �� ������� ������ ��������
	GIMSK |= (1 << INT0);		// ��������� ���������� INT0
	sei();						// ��������� ���������� ���������
}

