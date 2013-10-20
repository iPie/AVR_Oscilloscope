#ifndef ADC_H
#define	ADC_H

#include <avr/io.h>

/**
 * Starting ADC by enabling ADEN, ADSC and ADIE registers.
 * To handle interrupts:
 *
 * volitile unsigned char value;
 * ISR(ADC_vect) {
 *     value = ADC; // Read ADC register
 *     ADCSRA |= 1 << ADSC;
 * }
 * @param channel ADC input pin (0..7) 
 */
void ADCInit(uint8_t channel) {
    ADMUX = (1 << REFS0) | (1 << REFS1) | channel; // AVCC with external capacitor at AREF pin
    ADCSRA = (1 << ADEN)
            | (1 << ADIE)
            | (1 << ADATE)
            | (1 << ADPS0)
            | (0 << ADPS1)
            | (1 << ADPS2); // Division factor = 32
    MCUCR |= 1 << SM0;
    MCUCR |= 1 << SE; // Sleep-mode enabled
    ADCSRA |= (1 << ADSC);
}

void disableADC() {
    ADCSRA = 0x00;
}

uint16_t analogRead(uint8_t channel) {
    ADMUX |= channel;
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF))) {
        // Do nothing
    }
    ADCSRA |= (1 << ADIF); // Clear ADIF
    return (ADC);
}

#endif	/* ADC_H */
