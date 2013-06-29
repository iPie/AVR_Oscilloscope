/* 
 * File:   ADC.h
 * Author: Ajarax
 *
 * Created on November 10, 2012, 2:33 AM
 */

#ifndef ADC_H
#define	ADC_H

#include <avr/io.h>

void ADCInit() {
    ADMUX = (1 << REFS0) | (0 << REFS1); // Internal 2.56V reference
    ADCSRA = (1 << ADEN) | (1 << ADPS2); // Division factor = 16
}

/**
 * Starting ADC by enabling ADEN, ADSC and ADIE registers.
 * To handle interrupts:
 *
 * volitile unsigned char value;
 * ISR(ADC_vect) {
 *     value = ADC; // Read ADC register
 *     ADCSRA |= 1 << ADSC;
 * }
 * 
 * @param channel
 * ADC input pin (0..7)
 */
void ADCInit(uint8_t channel) {
    ADMUX = (1 << REFS0) | channel; // AVCC with external capacitor at AREF pin
    ADCSRA = (1 << ADEN)
            | (1 << ADIE)
            // Division factor = 128
            | (1 << ADPS0)
            | (1 << ADPS1)
            | (1 << ADPS2);
    MCUCR = 1 << SM0;
    MCUCR |= 1 << SE; // Sleep-mode enabled
    ADCSRA |= (1 << ADSC);
}

void DisableADC() {
    ADCSRA = 0x00;
}

uint16_t AnalogRead(uint8_t channel) {
    ADMUX |= channel;
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF))) {
        // Do nothing
    }
    ADCSRA |= (1 << ADIF); // Clear ADIF
    return (ADC);
}

#endif	/* ADC_H */