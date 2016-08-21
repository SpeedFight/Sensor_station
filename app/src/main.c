#include <avr/io.h>
#include <util/delay.h>
#include "../inc/uart.h"
#include "../inc/esp.h"

int main(void)
{
	comm_typedef uart;
	uart_init_struct(&uart);
	uart.init();

	esp_typedef esp;
	esp_init_struct(uart.send,uart.received,&esp);

	while(1)
	{
		esp.connect_to_wifi("nazwa","haslo");
		esp.test_connection();
		esp.connect_to_TCP("111.111.111.111","80");
		
		//_delay_ms(1000);

		if(*uart.received_data_pack_flag)
		{
			*uart.received_data_pack_flag=0;

			uart.send(uart.received);

		}
	}
}
