#ifndef _thingspeak_h
#define _thingspeak_h

/**
 * @file	thingspeak.h
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	04.09.16
 */


 #include <avr/io.h>

typedef struct{
    const char *field_no;
    char *field_value;
}data_field_typedef;

typedef struct{
     const char *ip;
     const char *port;
     const char *channel_id;
     const char *api_key;

     uint16_t (*post_message_length)();
     void (*send_post)();

 }thingspeak_typedef;

 #define DATA_FIELD_2
 #define DATA_FIELD_3
 #define DATA_FIELD_4


 #ifndef DATA_FIELD_2
 void thingspeak_init_struct(void (*uart_send_function)(char *),
                                thingspeak_typedef *thingspeak_struct,
                                 data_field_typedef *data1);
 #endif


 #ifndef DATA_FIELD_3
 void thingspeak_init_struct(void (*uart_send_function)(char *),
                                thingspeak_typedef *thingspeak_struct,
                                 data_field_typedef *data1,
                                 data_field_typedef *data2);
 #endif

 #ifndef DATA_FIELD_4
 void thingspeak_init_struct(void (*uart_send_function)(char *),
                                thingspeak_typedef *thingspeak_struct,
                                 data_field_typedef *data1,
                                 data_field_typedef *data2,
                                 data_field_typedef *data3);
 #endif

 #ifdef DATA_FIELD_4
 void thingspeak_init_struct(void (*uart_send_function)(char *),
                                thingspeak_typedef *thingspeak_struct,
                                 data_field_typedef *data1,
                                 data_field_typedef *data2,
                                 data_field_typedef *data3,
                                 data_field_typedef *data4);
 #endif

 #endif
