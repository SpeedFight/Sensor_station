#ifndef _thingspeak_h
#define _thingspeak_h

/**
 * @file	thingspeak.h
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	04.09.16
 */


 #include <avr/io.h>

typedef struct{
    char *field_no;
    char *field_value;
}data_field_typedef;

typedef struct{
     char *ip;
     char *port;
     char *channel_id;
     char *api_key;

     uint16_t (*post_message_length)();
     void (*send_post)();

 }thingspeak_typedef;

 #endif
