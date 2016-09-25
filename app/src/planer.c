/**
* @file	planer.c
* @Author	SpeedFight (speedfight_2@wp.pl)
* @date	25.09.16
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../inc/uart.h"
#include "../inc/esp.h"
#include "../inc/thingspeak.h"
#include "../inc/photoresistor.h"
#include "../inc/dht.h"
#include "../inc/planer.h"

#include <string.h>
#include <stdlib.h>

const char ip[]="184.106.153.149";
const char port[]="80";
const char channel_id[]="143012";
const char api_key[]="8TPKDQ7OU004TBD5";


///
static char str_temperature[4];
static char str_humidity[4];

int8_t temp, hum;

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
}time_typedef;

time_typedef time;


#define COMPARE_NUMBER (62500u-1u)
void start_timer()
{
    TCCR1B |= (1<<WGM12); //CTC mode for 16bit timer A
    TCCR1B |= (1<<CS11) | (1<<CS10); //prescaler to 64

    OCR1AL |= COMPARE_NUMBER;
    OCR1AH |= (COMPARE_NUMBER >>8);

    TIFR |= (1<<OCF1A); //enable IRQ
}

void timer()
{

}

ISR (TIMER1_COMPA_vect)
{
    time.seconds++;

    if(time.seconds>59u)
    {
        time.seconds=0;
        time.minutes++;
    }

    if(time.minutes>59u)
    {
        time.minutes=0;
        time.hours++;
    }

    if(time.hours>23u)
    {
        time.hours=0;
        time.days++;
    }
}

uint8_t main_activity()
{
    //init uart
    comm_typedef uart;
    uart_init_struct(&uart);
    uart.init();

    //init photoresistor
    photoresistor_typedef photoresistor;
    photoresistor_init_struct(&photoresistor);
    photoresistor.init();

    //init esp
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

    data_field_typedef temperature=	{.field_no=str_temperature};
    data_field_typedef humidity=	{.field_no=str_humidity};
    data_field_typedef light; //=	{.field_no="0"};

    thingspeak_init_struct(uart.send,
                            &thingspeak,
                            &temperature,
                            &humidity,
                            &light);

    //przed wysłaniem zawsze wyczyść *uart.received_data_pack_flag=0;

    start_timer();

/////debug
	int8_t temp, hum;

while(1)
{
	uart.send("bright: ");
	uart.send(light.field_value);
	uart.send(" temp: ");
	uart.send(temperature.field_value);
	uart.send(" hum: ");
	uart.send(humidity.field_value);
	uart.send("%\n\r");

	dht_gettemperaturehumidity(&temp,&hum);
	itoa (temp, str_temperature, 10);
	itoa (hum, str_humidity, 10);

	for(uint8_t i=0; i<20;i++)
	{
		photoresistor.start_measure();
		_delay_ms(100);
	}

	light.field_value=photoresistor.get_brightness();
	photoresistor.reset_average();

}

/*
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
	*/
}
