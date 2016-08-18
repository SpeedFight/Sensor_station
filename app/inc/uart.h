#ifndef _uart_h
#define _uart_h

#include <avr/io.h>
typedef struct 
{
	void (*init)();
	void (*send)(char *message);
	char *received;
	uint8_t *received_data_pack;
}comm_typedef;


void uart_init_struct(comm_typedef *uart);

#endif
