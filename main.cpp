/*
 * main.cpp
 *
 * Created: 2/7/2013 10:33:09 PM
 * Author: Ajarax
 */

#include <avr/io.h>
#include <util/delay.h>
#include "lib/ADC.h"
#include "lib/USART.h"

volatile unsigned char usartValue;
volatile uint8_t adcLow;
volatile uint8_t adcHigh;

int main(void) {
    DDRB |= (1 << PINB0);
    PORTB |= (0 << PINB0);
    sei();
    USARTInit(true);
    while (1) {
        if (ADCSRA != 0x00) {
            USARTSendByte(adcLow);
            USARTSendByte(adcHigh);
        }
    }
}

ISR(USART_RXC_vect) {
    usartValue = UDR;
    switch (usartValue) {
        case 0x52: // start transmission
            disableADC();
            ADCInit(0);
            PORTB |= (1 << PINB0);
            break;
        case 0x53: // stop transmission
            disableADC();
            PORTB &= ~(1 << PINB0);
            break;
    }
}

ISR(ADC_vect) {
    adcLow = ADCL; // ADCL should be accessed before ADCH
    adcHigh = ADCH;
    ADCSRA |= 1 << ADSC;
}
