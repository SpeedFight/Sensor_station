/**
 * @file	esp.h
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	19.08.16
 */

#include <avr/io.h>

typedef struct{
        //uint8_t (*connect_to_wifi)(char *ssid, char *password);
        uint8_t (*test_internet)();
        uint8_t (*test_ap)();
        uint8_t (*reset_until_ready)();
        uint8_t (*send_to_TCP)(char *message,char *specific_answer,char *ip, char *port);
        volatile uint8_t *received_data_pack_flag;

}esp_typedef;

void esp_init_struct(  void (*uart_send_function)(char *),  //pointer to send function
                void (*set_beggining_input_buffer)(),
                void (*set_null_to_buff_beginning)(),
                char * ptr_input_array, //pointer to input data buffer
                volatile uint8_t *received_data_flag,
                esp_typedef *esp);       //pointer to struct
