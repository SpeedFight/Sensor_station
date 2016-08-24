/**
 * @file	esp.c
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	19.08.16
 */
#include "../inc/esp.h"
#include <avr/io.h>
#include <util/delay.h>

//#include "stdio.h"

#define DELAY_TIME 750

static void (*send_uart)(char *);    //pointer to send via uart function

static char *input_buff;
static volatile uint8_t *received_data_pack_flag;




static uint8_t log_to_wifi(char *ssid,char *password)
{
        send_uart("AT+CWMODE=1\n\r");
        _delay_ms(DELAY_TIME);
        send_uart("AT+CWJAP=\"");send_uart(ssid);send_uart("\",\"");send_uart(password);send_uart("\"\n\r");
        _delay_ms(DELAY_TIME);

        //sprawdzenie czy sie udało++
        return 1;
}

static uint8_t ping()
{
        send_uart("AT+PING=\"www.google.pl\"\n\r");
        _delay_ms(DELAY_TIME);
        //sprawdzanie czy pozytywne++
        return 1;
}

static uint8_t log_to_TCP(char *ip, char *port)
{
        send_uart("AT+CIPMODE=0\n\r");
        _delay_ms(DELAY_TIME);
        send_uart("AT+CIPMUX=0\n\r");
        _delay_ms(DELAY_TIME);

        send_uart("AT+CIPSTART=\"TCP\",\"");send_uart(ip);send_uart("\",");send_uart(port);send_uart("\n\r");
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

/**
 * @brief
 * @detail
 */
void esp_init_struct(  void (*uart_send_function)(char *),  //pointer to send function
                char * ptr_input_array, //pointer to input data buffer
                esp_typedef *esp)       //pointer to struct
{
        send_uart=uart_send_function;

        input_buff=ptr_input_array;

        esp->connect_to_wifi=&log_to_wifi;
        esp->test_connection=&ping;
        esp->connect_to_TCP=&log_to_TCP;
        esp->received_data_pack_flag=received_data_pack_flag;
}



//AT command list"
/*always end by \n\r */
//char *cwmode1="AT+CWMODE=1\n\r";
//char *cwjap="AT+CWJAP=\"SSID\",\"pass\"\n\r";
//char *ping="AT+PING=\"www.google.pl\"\n\r";

//char *cipmode0="AT+CIPMODE=0\n\r";
//char *cipmux0="AT+CIPMUX=0\n\r";

//char *cipstart="AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\n\r";

static char *cipsend="AT+CIPSEND=73\n\r";

static char *get_data="GET https://api.thingspeak.com/channels/143012/feeds.json?results=1&api_key=xxx";

static char *send_data="GET https://api.thingspeak.com/update?api_key=xxx&field1=0";
