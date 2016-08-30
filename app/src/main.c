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

	esp_typedef esp;
	esp_init_struct(uart.send,
			uart.set_input_buffer_pointer_to_beginning,
			uart.received,
			uart.received_data_pack_flag,
			&esp);

	DDRD |=(1<<PIN6);
	//PORTD &=~(1<<PIN6);
	PORTD |=(1<<PIN6);

	//przed wysłaniem zawsze wyczyść *uart.received_data_pack_flag=0;

	/*
	do {
		while(!(*uart.received_data_pack_flag));


	}while(strstr(uart.received,"GOT IP"));     //WIFI GOT IP
	PORTD &=~(1<<PIN6);

	_delay_ms(1000);
	PORTD |=(1<<PIN6);
	*/

PORTD &=~(1<<PIN6);
	_delay_ms(1000);
	while(1)
	{
		if(esp.test_ap())
		{
			PORTD &=~(1<<PIN6);
			_delay_ms(500);

			PORTD |=(1<<PIN6);
		}
		/*
		//esp.connect_to_wifi("nazwa","haslo");
		//esp.test_connection();
		//esp.connect_to_TCP("111.111.111.111","80");

		//_delay_ms(1000);
		//uart.send(uart.received);
		_delay_ms(500);
		uart.send("AT+CWJAP?\r\n");
		uart.set_input_buffer_pointer_to_beginning();
		if(*uart.received_data_pack_flag)
		{


			if(strstr(uart.received,"+CWJAP"))
			{

				PORTD &=~(1<<PIN6);
				_delay_ms(500);

				PORTD |=(1<<PIN6);
			}
			//uart.send(uart.received);
		}
		*/
	}
}
