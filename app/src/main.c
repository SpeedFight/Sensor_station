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

	//esp_typedef esp;
	//esp_init_struct(uart.send,uart.received,&esp);

	DDRD |=(1<<PIN6);
	//PORTD &=~(1<<PIN6);
	PORTD |=(1<<PIN6);

	//przed wysłaniem zawsze wyczyść *uart.received_data_pack_flag=0;

	while(1)
	{
		//esp.connect_to_wifi("nazwa","haslo");
		//esp.test_connection();
		//esp.connect_to_TCP("111.111.111.111","80");

		//_delay_ms(1000);
		//uart.send(uart.received);
		if(*uart.received_data_pack_flag)
		{
			uart.send("Przyszło: ");
			uart.send(uart.received);
			uart.send("\n\r\n");


			PORTD &=~(1<<PIN6);

			if((strstr(uart.received,"OK")))
			{
				_delay_ms(500);
				uart.send("Przyjąłem: ");
				uart.send(uart.received);
			}
			PORTD |=(1<<PIN6);


			uart.send("**********************************\n\r\n\r");
			*uart.received_data_pack_flag=0;
		}
	}
}
