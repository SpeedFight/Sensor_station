/**
 * @file	esp.c
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	19.08.16
 */
#include "../inc/esp.h"

#include "stdio.h"

/**
 * @brief
 * @detail
 */
static void send_esp(      void (*send_function)(char *),  /*function pointer*/
                char *message)                  /*message (cstring)*/
{
        send_function(message);
}

//AT command list
/*always end by \n\r */
char *cwmode1="AT+CWMODE=1\n\r";
char *cwjap="AT+CWJAP=\"SSID\",\"pass\"\n\r";
char *ping="AT+PING=\"www.google.pl\"\n\r";

char *cipmode0="AT+CIPMODE=0\n\r";
char *cipmux0="AT+CIPMUX=0\n\r";

char *cipstart="AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\n\r";

char *cipsend="AT+CIPSEND=73\n\r";

char *get_data="GET https://api.thingspeak.com/channels/143012/feeds.json?results=1&api_key=xxx";

char *send_data="GET https://api.thingspeak.com/update?api_key=xxx&field1=0";
