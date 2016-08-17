/**
 * @file	uart.c
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	13.08.16
 * @brief	Function definition for uart interface
 */

#include "../inc/uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/**
 * @addtogroup uart_definitions
 * @{
 */
#define BAUD_UART 19200u	
#define UBRR_value (F_CPU/16/BAUD_UART-1)
///@}

char uart_receive_data[512];
uint8_t uart_received_data_packet;
uint8_t uart_flag;

/**
 * @brief Inicjalizacja modułu uart
 */
void init(void)
{
	UBRRL =(uint8_t) UBRR_value;		//ustawienie predkosci transmisji
	UBRRH =(uint8_t)(UBRR_value>>8);

	UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);	//aktywowanie TX oraz RX, oraz przerwanie po odebraniu ramki danych
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0) ;	//ramka 8 bitów, 1 bit start i stop
	sei(); //właczenie przerwań
}


/**
 * @brief Wysłanie cstringa uartem
 */
void send(char *message)
{
	uart_flag=1;
	do
	{

		while (!( UCSRA & (1<<UDRE)));	//czekaj aż poprzednie sie wyśle
							//również tylko wtedy mozna pisać do tego
							// bufora
	
		UDR = *message;				//wpakowanie danych do bufora
	
	}while(*(++message));			//jeśli napotkasz koniec cstring to skoncz wysyłanie
						//(symbol '/0')
}

ISR(USART_RXC_vect)
{
	static uint16_t element;
	PORTB ^=(1<<PINB0);

	if(uart_flag)
	{
		uart_flag=0;
		element=0;
	}

	uart_receive_data[element++]=UDR;
	uart_receive_data[element]='\0';
	//send(uart_receive_data);


}

/**
 * @brief Initialize pointers to uart struct
 */
void uart_init(comm_typedef *uart)
{
	uart->init=&init;
	uart->send=&send;
	uart->received=uart_receive_data;
}
