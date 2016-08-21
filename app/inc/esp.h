/**
 * @file	esp.h
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	19.08.16
 */

#include <avr/io.h>

typedef struct{
        uint8_t (*connect_to_wifi)(char *ssid, char *password);
        uint8_t (*test_connection)();
        uint8_t (*connect_to_TCP)(char *ip, char *port);
        //uint8_t (*send_data)();
        volatile uint8_t *received_data_pack_flag;

}esp_typedef;

void esp_init_struct(  void (*uart_send_function)(char *),  //pointer to send function
                char * ptr_input_array, //pointer to input data buffer
                esp_typedef *esp);      //pointer to struct
