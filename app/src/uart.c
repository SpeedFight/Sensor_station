/**
 * @file	uart.c
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	13.08.16
 * @brief	Function definition for uart interface
 */

#include "../inc/uart.h"
#include <avr/io.h>
#include <util/delay.h>

/**
 * @addtogroup uart_definitions
 * @{
 */
#define BAUD_UART  300
#define UBRR_value (F_CPU/16/BAUD_UART-1)
///@}

/**
 * @brief Inicjalizacja modułu uart
 */
void init(void)
{
	UBRRL =(uint8_t) UBRR_value;		//ustawienie predkosci transmisji
	UBRRH =(uint8_t)(UBRR_value>>8);

	UCSRB = (1<<RXEN) | (1<<TXEN);			//aktywowanie TX oraz RX
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0) ;	//ramka 8 bitów, 1 bit start i stop
}


/**
 * @brief Wysłanie cstringa uartem
 */
void send(char *message)
{
	do
	{

		while (!( UCSRA & (1<<UDRE)));	//czekaj aż poprzednie sie wyśle
							//również tylko wtedy mozna pisać do tego
							// bufora
	
		UDR = *message;				//wpakowanie danych do bufora
	
	}while(*(++message));			//jeśli napotkasz koniec cstring to skoncz wysyłanie
						//(symbol '/0')
}
