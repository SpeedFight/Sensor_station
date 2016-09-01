#include <avr/io.h>
#include <util/delay.h>
#include "../inc/uart.h"
#include "../inc/esp.h"

#include <string.h>

int main(void)
{
	comm_typedef uart;
	uart_init_struct(&uart);
	uart.init();
/*
	esp_typedef esp;
	esp_init_struct(uart.send,
			uart.set_input_buffer_pointer_to_beginning,
			uart.received,
			uart.received_data_pack_flag,
			&esp);
*/
	DDRD |=(1<<PIN6);
	PORTD &=~(1<<PIN6);
	_delay_ms(2000);
	PORTD |=(1<<PIN6);

	//przed wysłaniem zawsze wyczyść *uart.received_data_pack_flag=0;

	PORTD &=~(1<<PIN6);
	_delay_ms(1000);
	PORTD |=(1<<PIN6);
	while(1)
	{


		if(*uart.received_data_pack_flag)
		{
			if(strstr(uart.received,"OK"))
			{
				PORTD &=~(1<<PIN6);
				_delay_ms(500);
				PORTD |=(1<<PIN6);

				uart.send(uart.received);
				uart.set_input_buffer_pointer_to_beginning();
				uart.set_null_to_buff_beginning();
			}
		}
		_delay_ms(500);
		uart.send(uart.received);

	}
}
