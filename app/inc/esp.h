/**
 * @file	esp.h
 * @Author	SpeedFight (speedfight_2@wp.pl)
 * @date	19.08.16
 */

typedef struct{
        void (*connect_to_wifi)(char *ssid, char *password);
        void (*test_connection)();
        void (*connect_to_TCP)(char *ip, char *port);
        //void (*send_data)();

}esp_typedef;

void esp_init(void (*uart_send_function)(char *));
