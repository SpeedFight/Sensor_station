/**
* @file	photoresistor.c
* @Author	SpeedFight (speedfight_2@wp.pl)
* @date	24.09.16
*/

#include "../inc/photoresistor.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t no_brightness;


#define ADC_PORT    7u  //adc7

//lowest voltage on voltage divider (photoresistor) ~1.2V
//((1.2*1023)/3.3)=372
#define ADC_LOW  372u

#define ADC_MAX (1023u-372u)

static void init()
{
    ADMUX |=(1<<REFS0) //AVCC with external capacitor at AREF pin
            |ADC_PORT; //set adc port to use //default: adc7

    ADCSRA |=   (1<<ADPS2) | (1<<ADPS0) //prescaler to 32
                |(1<<ADEN)  //enable adc
                |(1<<ADIE); ///enable interrupt

}

static void start_measure()
{
    ADCSRA |=(1<<ADSC); //start conversion
}

static void reset_average()
{
    no_brightness = 0;
}

static char *get_string_brightness()
{
    static char str_brightness[4];
    itoa (no_brightness, str_brightness, 10);
    return  str_brightness;
}

ISR(ADC_vect)
{
    uint16_t adc_tmp=0;

    adc_tmp = ADCL;
    adc_tmp |=ADCH<<8;

    //average measures
    if(!no_brightness)
        no_brightness = ((adc_tmp - ADC_LOW)*100/ADC_MAX);
    else
        no_brightness = ((uint16_t)no_brightness +((adc_tmp - ADC_LOW)*100/ADC_MAX))/2;

}

void photoresistor_init_struct(photoresistor_typedef *photoresistor)
{
    photoresistor->init=&init;
    photoresistor->reset_average=&reset_average;
    photoresistor->start_measure=&start_measure;
    photoresistor->get_brightness=&get_string_brightness;
}
