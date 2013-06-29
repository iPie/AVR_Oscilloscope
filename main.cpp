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
volatile uint8_t adcHigh;
volatile uint8_t adcLow;

void runStartUpSequence() {
    for (int i = 0; i < 3; i++) {
        PORTB ^= (1 << PINB0);
        _delay_ms(50);
        PORTB ^= (1 << PINB0);
        _delay_ms(50);
    }
}

int main(void) {
    DDRB |= (1 << PINB0) | (1 << PINB1);
    PORTB |= (0 << PINB0) | (0 << PINB1);
    runStartUpSequence();
    sei();
    USARTInit(true);
    while (1) {
        // Endless cycle of violence..
        cli();
        USARTSendByte(adcLow);
        USARTSendByte(adcHigh);
        sei();
    }
}

ISR(USART_RXC_vect) {
    usartValue = UDR;
    switch (usartValue) {
        case 'R':
            DisableADC();
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
    adcHigh = ADCH;
    adcLow = ADCL;
    ADCSRA |= 1 << ADSC;
}