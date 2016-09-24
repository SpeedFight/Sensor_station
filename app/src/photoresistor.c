/**
* @file	photoresistor.c
* @Author	SpeedFight (speedfight_2@wp.pl)
* @date	24.09.16
*/

#include "../inc/photoresistor.h"
#include <avr/io.h>

uint8_t brightness;

void init()
{

}

void photoresistor_init_struct(photoresistor_typedef *photoresistor)
{
    photoresistor->brightness=&brightness;
    photoresistor->init=&init;
}
