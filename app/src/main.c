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
			uart.set_null_to_buff_beginning,
			uart.received,
			uart.received_data_pack_flag,
			&esp);

	DDRD |=(1<<PIN6);
	PORTD &=~(1<<PIN6);
	_delay_ms(100);
	PORTD |=(1<<PIN6);

	//przed wysłaniem zawsze wyczyść *uart.received_data_pack_flag=0;

	uint8_t sprawdz=0;
	while(1)
	{
















		
/*
		//uart.send("Komenda_czekam_na_odp!");

		if(sprawdz==0)
		{
			if(*uart.received_data_pack_flag)
			{
				_delay_us(100);
				for(uint16_t try=0;try<(5*10000);try++)
				{
					if(strstr(uart.received,"ready"))
					{
						PORTD &=~(1<<PIN6);
						_delay_ms(2000);
						PORTD |=(1<<PIN6);

						uart.set_input_buffer_pointer_to_beginning();
						uart.set_null_to_buff_beginning();
						*uart.received_data_pack_flag=0;
						sprawdz=1;
						//break;
					}
				}
			}
		}

		if(sprawdz)
		{
			uart.send("AT\r\n");

			for(uint16_t try=0;try<(5*10000);try++)
			{
				_delay_us(100);
				if(strstr(uart.received,"OK"))
				{
					if(*uart.received_data_pack_flag)
					{
						PORTD &=~(1<<PIN6);
						_delay_ms(2000);
						PORTD |=(1<<PIN6);

						uart.set_input_buffer_pointer_to_beginning();
						uart.set_null_to_buff_beginning();
						//*uart.received_data_pack_flag=0;
						//sprawdz=1;
						//break;
					}
				}
			}
		}


		_delay_ms(1000);
		//uart.send(uart.received);
*/
	}
}
