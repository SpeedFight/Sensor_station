#include <avr/io.h>
#include <util/delay.h>
#include "../inc/uart.h"
#include "../inc/esp.h"
#include "../inc/thingspeak.h"
#include "../inc/photoresistor.h"

#include <string.h>

const char ip[]="184.106.153.149";
const char port[]="80";
const char channel_id[]="143012";
const char api_key[]="8TPKDQ7OU004TBD5";

int main(void)
{
	comm_typedef uart;
	uart_init_struct(&uart);
	uart.init();

	photoresistor_typedef photoresistor;
	photoresistor_init_struct(&photoresistor);
	photoresistor.init();


	esp_typedef esp;
	esp_init_struct(uart.send,
			uart.set_input_buffer_pointer_to_beginning,
			uart.set_null_to_buff_beginning,
			uart.received,
			uart.received_data_pack_flag,
			&esp);

	thingspeak_typedef thingspeak={
		.ip=ip,
		.port=port,
		.channel_id=channel_id,
		.api_key=api_key
	};

	data_field_typedef temperature=	{.field_no="1"};
	data_field_typedef humidity=	{.field_no="2"};
	data_field_typedef pressure=	{.field_no="3"};
	data_field_typedef light=		{.field_no="4"};

	thingspeak_init_struct(uart.send,
							&thingspeak,
							&temperature,
							&humidity,
							&pressure,
							&light);

	//przed wysłaniem zawsze wyczyść *uart.received_data_pack_flag=0;
	temperature.field_value="1";
	humidity.field_value="2";
	pressure.field_value="3";
	light.field_value="4";


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

			if(esp.fnct_send_to_TCP(thingspeak.send_post,
									thingspeak.post_message_length(),
									"+IPD,2:",ip,port)){
				PORTD &=~(1<<PIN6);
				_delay_ms(2000);
				PORTD |=(1<<PIN6);
			}

		}
	}
}
