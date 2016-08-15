
#include <avr/io.h>
#include <util/delay.h>
#include "../inc/uart.h"
int main(void)
{
	init();
	char *message="aiadomosc";
	DDRB |= (1<<PINB0);
	while(1)
	{
		PORTB ^=(1<<PINB0);
		_delay_ms(500);
		send(message);

	}
}
