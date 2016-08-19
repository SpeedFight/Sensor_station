#include <avr/io.h>
#include <util/delay.h>
#include "../inc/uart.h"
#include "../inc/esp.h"

int main(void)
{
	comm_typedef uart;
	uart_init_struct(&uart);
	uart.init();

	while(1)
	{
		//_delay_ms(1000);

		if(*uart.received_data_pack_flag)
		{
			*uart.received_data_pack_flag=0;

			uart.send(uart.received);

		}
	}
}
