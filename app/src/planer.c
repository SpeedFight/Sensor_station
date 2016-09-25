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

int8_t no_temperature, no_humidity;

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
}time_typedef;

volatile time_typedef time;


#define COMPARE_NUMBER (62500u-1u)
void start_timer()
{
    TCCR1B |= (1<<WGM12); //CTC mode for 16bit timer A
    TCCR1B |= (1<<CS11) | (1<<CS10); //prescaler to 64

    //OCR1AL |= COMPARE_NUMBER;
    //OCR1AH |= (COMPARE_NUMBER >>8);
    OCR1A = COMPARE_NUMBER;

    TIMSK |= (1<<OCIE1A); //enable IRQ
}


uint8_t wait_minutes(uint8_t minutes)
{
    static uint8_t actual_minutes;
    static uint8_t wait;
    static uint8_t flag;

    if(!(flag))
    {
        flag=1;
        actual_minutes=time.minutes;
    }

    if(!(actual_minutes==time.minutes))
    {
        flag=0;
        wait++;
    }

    if(wait>=minutes)
    {
        flag=0;
        wait=0;
        return 1;
    }

    return 0;
}

uint8_t wait_hours(uint8_t hours)
{
    static uint8_t actual_hour;
    static uint8_t wait;
    static uint8_t flag;

    if(!(flag))
    {
        flag=1;
        actual_hour=time.hours;
    }

    if(!(actual_hour==time.hours))
    {
        flag=0;
        wait++;
    }

    if(wait>=hours)
    {
        flag=0;
        wait=0;
        return 1;
    }

    return 0;
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
	int8_t tmp_temp, tmp_hum;

    photoresistor.start_measure();
    dht_gettemperaturehumidity(&tmp_temp,&tmp_hum);
    no_temperature=(no_temperature+tmp_temp)/2u;
    no_humidity=(no_humidity+tmp_hum)/2u;

    itoa (no_temperature, str_temperature, 10);
    itoa (no_humidity, str_humidity, 10);
	light.field_value=photoresistor.get_brightness();
	photoresistor.reset_average();

    while(1)
    {
        if(wait_minutes(1))
        {
            uart.send("bright: ");
            uart.send(light.field_value);
            uart.send(" temp: ");
            uart.send(temperature.field_value);
            uart.send(" hum: ");
            uart.send(humidity.field_value);
            uart.send("%\n\r");
        }
    }


	while(1)
	{

		while (!(esp.reset_until_ready()))
        {

		}

		while(1)
		{

			if(esp.test_ap())
            {

			}

			if(esp.test_internet())
            {

			}

			if(esp.fnct_send_to_TCP(thingspeak.send_post,
									thingspeak.post_message_length(),
									"+IPD,2:",ip,port))
            {

			}

		}
	}
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
