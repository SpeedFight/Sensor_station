#include <avr/io.h>
#include <util/delay.h>
#include "../inc/uart.h"

int main(void)
{
	comm_typedef uart;
	uart_init_struct(&uart);
	uart.init();

	while(1)
	{
		//PORTB ^=(1<<PINB0);
		_delay_ms(1000);

		//if((uart.received_data_pack))

		uart.send(uart.received);
		uart.received_data_pack=0;

	}
}
