/*
 * Oscilloscope.cpp
 *
 * Created: 2/7/2013 10:33:09 PM
 * Author: Ajarax
 */


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "lib/ADC.h"
#include "lib/USART.h"

volatile unsigned char usartValue;
volatile uint32_t adcSample;
volatile uint8_t samplesCount;

int main(void) {
    DDRB |= (1 << PINB0) | (1 << PINB1);
    PORTB |= (0 << PINB0) | (0 << PINB1);
    sei();
    USARTInit(true);
    // Indicate start-up sequence
    for (int i = 0; i < 3; i++) {
        PORTB ^= (1 << PINB0);
        _delay_ms(50);
        PORTB ^= (1 << PINB0);
        _delay_ms(50);
    }
    while (1) {
        // Endless cycle of violence..
    }
}

ISR(USART_RXC_vect) {
    usartValue = UDR;
    switch (usartValue) {
        case 'R':
            DisableADC();
            samplesCount = 0;
            adcSample = 0;
            ADCInit(0);
            break;
        case 'S':
            DisableADC();
            break;
        default:
            PORTB ^= (1 << PINB0);
            break;
    }
}

ISR(ADC_vect) {
    if (samplesCount == 16) {
        int result = (int) (adcSample / 16);
        char adcLow = result;
        char adcHigh = (result >> 8);
        USARTSendByte(adcLow);
        USARTSendByte(adcHigh);
        adcSample = 0;
        samplesCount = 0;
    }
    adcSample += ADC;
    samplesCount++;
    //    char res [10];
    //    itoa(ADC, res, 10);
    //    USARTSendString(res);
    ADCSRA |= 1 << ADSC;
}