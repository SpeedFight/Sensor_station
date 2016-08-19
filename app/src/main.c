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
		//_delay_ms(1000);

		if(*uart.received_data_pack)
		{
			*uart.received_data_pack=0;

			uart.send(uart.received);

		}
	}
}
