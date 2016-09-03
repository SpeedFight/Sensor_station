/**
 * @file	esp.c
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	19.08.16
 */
#include "../inc/esp.h"
#include <avr/io.h>

#include <util/delay.h>

//#include "stdio.h"
#include <string.h>
#include <stdlib.h>

#define DELAY_TIME 750
#define MAX_TRY 15
#define MAX_TRY_TIME 13UL //seconds

static void (*send_uart)(char *);    //pointer to send via uart function

static char *input_buff;
static volatile uint8_t *received_data_pack_flag;
static void (*p_set_input_buffer_pointer_to_beginning)();
static void (*p_set_null_to_buff_beginning)();


#define PIN_RESET	5
#define PORT_RESET	D

#define _RESET_PIN(a)	PIN   ## a
#define _RESET_PORT(a)	PORT     ## a
#define _RESET_DDR(a)	DDR	  ## a

#define RESET_PIN(a) 	_RESET_PIN(a)
#define RESET_PORT(a)	_RESET_PORT(a)
#define RESET_DDR(a) 	_RESET_DDR(a)

#define RESET_OFF	RESET_PORT(PORT_RESET) &= ~(1<<RESET_PIN(PIN_RESET));
#define RESET_ON	RESET_PORT(PORT_RESET) |= (1<<RESET_PIN(PIN_RESET));


static void reset()
{
        RESET_ON;
        _delay_ms(1000);
        RESET_OFF;
        RESET_ON;
}
/*
static uint8_t esp_accept_comand(char *ok_string) //nope
{
    _delay_ms(DELAY_TIME);
    p_set_input_buffer_pointer_to_beginning();
    if(*received_data_pack_flag)
    {
        if(strstr(input_buff,ok_string))//OK
            return 1;
    }
    return 0;
}*/


static uint8_t check_input_buff(char *check)
{
    if(strstr(input_buff,check))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void clear_input_buff()
{
    p_set_input_buffer_pointer_to_beginning();
    p_set_null_to_buff_beginning();
    *received_data_pack_flag=0;
}

static uint8_t check_input_buff_and_clear(char *check)
{
    if(strstr(input_buff,check))
    {
        clear_input_buff();
        return 1;
    }
    else
        return 0;
}

static uint8_t esp_accept_comand(char *string_to_send,char *ok_string, uint8_t wait_time)
{
    for(uint8_t try=0;try<MAX_TRY;try++)
    {
        clear_input_buff();
        send_uart(string_to_send);

        for(uint32_t try_time=0;try_time<((uint32_t)wait_time*10000);try_time++)
        {
            _delay_us(100);
            if(*received_data_pack_flag)
            {
                try_time=0;
                for(;try_time<((uint32_t)wait_time*10000);try_time++)
                {
                    _delay_us(100);

                    if(check_input_buff_and_clear(ok_string))
                        return 1;

                    //here and more if's
                }
            }
        }
        //if(!(try%4))
            //reset();

        //_delay_ms(1500);
    }
    return 0;
}


static uint8_t _esp_accept_comand(char *ok_string) //nope
{
    for(uint32_t try_time=0;try_time<(2*10000);try_time++)
    {
        _delay_us(100);
        if(*received_data_pack_flag)
        {
            try_time=0;
            for(;try_time<(2*10000);try_time++)
            {
                _delay_us(100);

                if(check_input_buff_and_clear(ok_string))
                {
                    return 1;
                }

                //here and more if's
            }
        }
    }
    return 0;
}

//static uint8_t reset_until_ready()
static uint8_t reset_until_ready()
{
    reset();
    for(uint32_t try_time=0;try_time<(1UL*10000);try_time++)
    {
        _delay_us(100);
        if(*received_data_pack_flag)
        {
            //try_time=0;
            for(;try_time<(1UL*10000);try_time++)
            {
                _delay_us(100);

                if(check_input_buff("ready"))
                {
                    //return 1;
                    for(;try_time<(5UL*10000);try_time++)
                    {
                        _delay_us(100);
                        if(check_input_buff_and_clear("WIFI"))
                        {
                            //PORTD &=~(1<<PIN6);
                            return 1;
                        }
                    }
                }

                //here and more if's
            }
        }
        try_time=0;
        reset();

    }
    return 0;
}



static uint8_t ping()
{

    return (esp_accept_comand("AT+PING=\"www.google.pl\"\r\n","OK",13));
    /*
    for(uint8_t try=0;try<MAX_TRY;try++)
    {
        clear_input_buff();
        send_uart("AT+PING=\"www.google.pl\"\r\n");

        for(uint32_t try_time=0;try_time<(MAX_TRY_TIME*10000);try_time++)
        {
            _delay_us(100);
            if(*received_data_pack_flag)
            {
                try_time=0;
                for(;try_time<(MAX_TRY_TIME*10000);try_time++)
                {
                    _delay_us(100);

                    if(check_input_buff_and_clear("OK"))
                        return 1;

                    //here and more if's
                }
            }
        }

    }
    return 0;
    */
}
/*
static uint8_t ping()
{
    send_uart("AT+PING=\"www.google.pl\"\r\n");
    send_uart(input_buff);
    return 1;
}

*/
static uint8_t check_connection(void)
{
    for(uint8_t try=0;try<MAX_TRY;try++)
    {
        clear_input_buff();
        send_uart("AT+CWJAP?\r\n");

        for(uint32_t try_time=0;try_time<(2UL*10000);try_time++)
        {
            _delay_us(100);
            if(*received_data_pack_flag)
            {
                try_time=0;
                for(;try_time<(2UL*10000);try_time++)
                {
                    _delay_us(100);

                    if(check_input_buff_and_clear("CWJAP:\""))
                    {
                        return 1;
                    }

                    if(check_input_buff_and_clear("busy"))
                        ;//try_time--;

                    if(check_input_buff_and_clear("No AP"))
                        ;//try_time--;

                    /*
                    if(strstr(input_buff,"CWJAP:1"))//connection timeout
                    ;

                    if(strstr(input_buff,"CWJAP:2"))//wrong password
                    ;

                    if(strstr(input_buff,"CWJAP:3"))//can not found target ap
                    ;

                    if(strstr(input_buff,"CWJAP:4"))//connect fail
                    ;
                    */

                    //here and more if's
                }
            }
        }
        //if(!(try%4))
            //reset();

        //_delay_ms(1500);
    }
    return 0;
}

static uint8_t log_to_wifi(char *ssid,char *password) //poprawić
{
        send_uart("AT+CWMODE=1\r\n");
        _delay_us(150);
        send_uart("AT+CWJAP=\"");send_uart(ssid);send_uart("\",\"");send_uart(password);send_uart("\"\r\n");
        _delay_ms(DELAY_TIME);

        //sprawdzenie czy sie udało++
        return 1;
}


static uint8_t log_to_TCP(char *ip, char *port)
{
    /*
    for(uint8_t i=0;i<MAX_TRY;i++)
    {
        send_uart("AT+CIPMODE=0\r\n");
        if(esp_accept_comand("OK"))
            i=MAX_TRY;
        else if(i>MAX_TRY-1)
            return 0;
    }
    for(uint8_t i=0;i<MAX_TRY;i++)
    {
        send_uart("AT+CIPMUX=0\r\n");
        if(esp_accept_comand("OK"))
        i=MAX_TRY;
        else if(i>MAX_TRY-1)
        return 0;
    }
    */
    esp_accept_comand("AT+CIPMODE=0\r\n","OK",2);
    esp_accept_comand("AT+CIPMUX=0\r\n","OK",2);


    for(uint8_t try=0;try<MAX_TRY;try++)
    {

        send_uart("AT+CIPSTART=\"TCP\",\"");send_uart(ip);send_uart("\",");send_uart(port);send_uart("\r\n");
        _delay_us(150);
        p_set_input_buffer_pointer_to_beginning();
        if(*received_data_pack_flag)
        {
            if(strstr(input_buff,"ALREADY"))//ALREADY CONNECTED
                return 1;

            if(strstr(input_buff,"CLOSED"))
                return 0;

            if(strstr(input_buff,"OK"))
                return 1;
        }

    return 1;
    }
}

/*
static uint8_t send(char *data,uint16_t *data_size,char *ip, char *port)
{
    if(!(*data))
        return 0;

    char tmp_send_size[4];
    itoa(*data_size, tmp_send_size,10);

    log_to_TCP(ip,port);

    for(uint8_t i=0;i<MAX_TRY;i++)
    {
        send_uart("AT+CIPSEND=");send_uart(tmp_send_size);send_uart("\r\n");
        if(esp_accept_comand("OK"))
            i=MAX_TRY;
        else if(i>MAX_TRY-1)
            return 0;
    }

    send_uart(data);
    if(esp_accept_comand("OK"))
        return 1;


    return 0;
}
*/

/**
 * @brief
 * @detail
 */
 void esp_init_struct(  void (*uart_send_function)(char *),  //pointer to send function
                 void (*set_beggining_input_buffer)(),
                 void (*set_null_to_buff_beginning)(),
                 char * ptr_input_array, //pointer to input data buffer
                 volatile uint8_t *received_data_flag,
                 esp_typedef *esp)       //pointer to struct
{
    RESET_DDR(PORT_RESET) |=(1<<RESET_PIN(PIN_RESET));

        send_uart=uart_send_function;

        received_data_pack_flag=received_data_flag;

        input_buff=ptr_input_array;

        p_set_input_buffer_pointer_to_beginning=set_beggining_input_buffer; //set function pointer

        p_set_null_to_buff_beginning=set_null_to_buff_beginning;

        esp->test_internet=&ping;
        esp->test_ap=&check_connection;
        esp->reset_until_ready=&reset_until_ready;
        //esp->connect_to_wifi=&log_to_wifi;
        //esp->connect_to_TCP=&log_to_TCP;
        //esp->received_data_pack_flag=received_data_pack_flag;
}



//AT command list"
/*always end by \r\n */
//char *cwmode1="AT+CWMODE=1\r\n";
//char *cwjap="AT+CWJAP=\"SSID\",\"pass\"\r\n";
//char *ping="AT+PING=\"www.google.pl\"\r\n";

//char *cipmode0="AT+CIPMODE=0\r\n";
//char *cipmux0="AT+CIPMUX=0\r\n";

//char *cipstart="AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n";

//static char *cipsend="AT+CIPSEND=73\r\n";

//static char *get_data="GET https://api.thingspeak.com/channels/143012/feeds.json?results=1&api_key=xxx";

//static char *send_data="GET https://api.thingspeak.com/update?api_key=xxx&field1=0";
