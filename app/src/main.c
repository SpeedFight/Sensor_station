
#include <avr/io.h>
#include <util/delay.h>
#include "../inc/uart.h"




int main(void)
{
	uart_init(&uart);
	uart.init();
	char *message="aiadomosc";
	DDRB |= (1<<PINB0);
	while(1)
	{
		PORTB ^=(1<<PINB0);
		_delay_ms(500);
		uart.send(message);

	}
}
