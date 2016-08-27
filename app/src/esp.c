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

#define DELAY_TIME 750
#define MAX_TRY 10

static void (*send_uart)(char *);    //pointer to send via uart function

static char *input_buff;
static volatile uint8_t *received_data_pack_flag;
static void (*p_set_input_buffer_pointer_to_beginning)();


#define RESET_PIN	5
#define RESET_PORT	D

#define __RESET(a)	PIN	## a
#define __RESET(a)	PORT	## a
#define __RESET(a)	DDR	## a

#define RESET_PIN(a) 	_RESET_PIN(a)
#define RESET_PORT(a)	_RESET_PORT(a)
#define RESET_DDR(a) 	_RESET_DDR(a)

#define RESET_ON	RESET_PORT(TX_PORT) &= ~(1<<RESET_PIN(TX_PIN));
#define RESET_OFF	RESET_PORT(TX_PORT) |= (1<<RESET_PIN(TX_PIN));

static void reset()
{
        RESET_ON;
        _delay_ms(500);
        RESET_OFF;
        _delay_ms(500);
}

static uint8_t log_to_wifi(char *ssid,char *password)
{
        send_uart("AT+CWMODE=1\r\n");
        _delay_ms(DELAY_TIME);
        send_uart("AT+CWJAP=\"");send_uart(ssid);send_uart("\",\"");send_uart(password);send_uart("\"\r\n");
        _delay_ms(DELAY_TIME);

        //sprawdzenie czy sie udało++
        return 1;
}

static uint8_t ping()
{
        send_uart("AT+PING=\"www.google.pl\"\r\n");
        _delay_ms(DELAY_TIME);
        //sprawdzanie czy pozytywne++
        return 1;
}

static uint8_t log_to_TCP(char *ip, char *port)
{
        send_uart("AT+CIPMODE=0\r\n");
        _delay_ms(DELAY_TIME);
        send_uart("AT+CIPMUX=0\r\n");
        _delay_ms(DELAY_TIME);

        send_uart("AT+CIPSTART=\"TCP\",\"");send_uart(ip);send_uart("\",");send_uart(port);send_uart("\r\n");
        _delay_ms(DELAY_TIME);

        //sprawdzenie czy sie udało++
        return 1;
}

static uint8_t send()
{
        //log to TCP++
        //format data++
        //send_data++
        //logout from TCP++
        return 1;
}

static uint8_t check_connection(void)
{
        for(uint8_t try=0;try<MAX_TRY;try++)
        {

                send_uart("AT+CWJAP?\r\n");
                _delay_us(100);
                p_set_input_buffer_pointer_to_beginning();
                //_delay_ms(1000);
                //send_uart(input_buff);
                //send_uart("\r\n");
                if(*received_data_pack_flag)
                {
                        if(strstr(input_buff,"+CWJAP"))
                        {
                                return 1;
                        }
                }
                _delay_ms(1000);
        }

        return 0;
}




/**
 * @brief
 * @detail
 */
void esp_init_struct(  void (*uart_send_function)(char *),  //pointer to send function
                void (*set_beggining_input_buffer)(),
                char *ptr_input_array, //pointer to input data buffer
                volatile uint8_t *received_data_flag,
                esp_typedef *esp)       //pointer to struct
{
        send_uart=uart_send_function;

        received_data_pack_flag=received_data_flag;

        input_buff=ptr_input_array;

        p_set_input_buffer_pointer_to_beginning=set_beggining_input_buffer; //set function pointer

        esp->test_internet=&ping;
        esp->test_ap=&check_connection;
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
