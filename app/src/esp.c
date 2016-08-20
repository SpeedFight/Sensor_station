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
