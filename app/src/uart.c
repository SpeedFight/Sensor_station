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
#define BAUD_UART 300	
#define UBRR_value (F_CPU/16/BAUD_UART-1)

#define TIMER_COMPARE_VALUE	(255-252) //(1024/1e6)*10 = 0.05s
					 //(clk_div/F_CPU)*mulipy_by
///@}

char uart_receive_data[256];
uint8_t uart_received_data_packet;
uint8_t uart_flag;
uint16_t element;
uint8_t uart_data_pack_received;

/**
 * @brief Inicjalizacja modułu uart
 */
void init(void)
{
	DDRB |= (1<<PINB0); //ustawienie pinu pod diode
	DDRD |= (1<<PIND7);

	UBRRL =(uint8_t) UBRR_value;		//ustawienie predkosci transmisji
	UBRRH =(uint8_t)(UBRR_value>>8);

	UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);	//aktywowanie TX oraz RX, oraz przerwanie po odebraniu ramki danych
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0) ;	//ramka 8 bitów, 1 bit start i stop


	//8bit timer0 configuration
	TCCR0 |=(1<<CS02) | (0<<CS01) | (1<<CS00); //clock/1024 prescaler

	sei(); //enale IRQ
	TCNT0 = (uint8_t)0; //Timer0 counter register value
}

/**
 * @brief Wysłanie cstringa uartem
 */
void send(char *message)
{
	//if(!(uart_data_pack_received))
	{
			PORTB &= ~(1<<PINB0);	
			uart_flag=1;
		do
		{

			while (!( UCSRA & (1<<UDRE)));	//czekaj aż poprzednie sie wyśle
								//również tylko wtedy mozna pisać do tego
								// bufora
		
			UDR = *message;				//wpakowanie danych do bufora
		
		}while(*(++message));			//jeśli napotkasz koniec cstring to skoncz wysyłanie
							//(symbol '/0')
		
		PORTB |= (1<<PINB0);
	}
}

/**
 * @brief Receive uart data byte interrupt routine
 * @detals 
 */
ISR(USART_RXC_vect)
{
	PORTD &=~(1<<PIND7);
	uart_data_pack_received=0;


	uart_receive_data[element++]=UDR;

	PORTD |=(1<<PIND7);

	TIFR |=(0<<TOV0);	//clear overflow flag
	TIMSK |=(1<<TOIE0); //enable timer0 overflow IRQ
	TCNT0 = (uint8_t)TIMER_COMPARE_VALUE; //Timer0 counter register value
	

}

/**
 * @brief timer irq routine
 * @detils Interrupt after last receive byte from uart, set
 *		last iput array element as \0
 */
ISR(TIMER0_OVF_vect)
{
	uart_receive_data[element]='\0';
	uart_data_pack_received=1;
	element=0;
	TIMSK &=~(1<<TOIE0); //disable timer0 overflow IRQ
	TCNT0 = (uint8_t)0; //Timer0 counter register value
	//PORTB ^=(1<<PINB0);

}
/**
 * @brief Initialize pointers to uart struct
 */
void uart_init_struct(comm_typedef *uart)
{
	uart->init=&init;
	uart->send=&send;
	uart->received=uart_receive_data;
	uart->received_data_pack=&uart_data_pack_received;
}
