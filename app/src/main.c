#include <avr/io.h>
#include <util/delay.h>
#include "../inc/uart.h"
#include "../inc/esp.h"

#include <string.h>

char message[]="GET https://api.thingspeak.com/update?api_key=8TPKDQ7OU004TBD5&field1=9";
char ip[]="184.106.153.149";
char port[]="80";

int main(void)
{
	comm_typedef uart;
	uart_init_struct(&uart);
	uart.init();

	esp_typedef esp;
	esp_init_struct(uart.send,
			uart.set_input_buffer_pointer_to_beginning,
			uart.set_null_to_buff_beginning,
			uart.received,
			uart.received_data_pack_flag,
			&esp);

	DDRD |=(1<<PIN6);
	PORTD &=~(1<<PIN6);
	_delay_ms(100);
	PORTD |=(1<<PIN6);

	//przed wysłaniem zawsze wyczyść *uart.received_data_pack_flag=0;

	while(1)
	{

		while (!(esp.reset_until_ready())){
			PORTD &=~(1<<PIN6);
			_delay_ms(1500);
		}
		PORTD |=(1<<PIN6);
		_delay_ms(1500);
		while(1)
		{

			if(esp.test_ap()){
				PORTD &=~(1<<PIN6);
				_delay_ms(2000);
				PORTD |=(1<<PIN6);
			}

			if(esp.test_internet()){
				PORTD &=~(1<<PIN6);
				_delay_ms(2000);
				PORTD |=(1<<PIN6);
			}

			if(esp.send_to_TCP(message,"+IPD,2:",ip,port)){
				PORTD &=~(1<<PIN6);
				_delay_ms(2000);
				PORTD |=(1<<PIN6);
			}
		}

		//_delay_ms(1000);
		//uart.send(uart.received);
	}
}
