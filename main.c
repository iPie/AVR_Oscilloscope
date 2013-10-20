#include <avr/io.h>
#include <util/atomic.h>
#include "config.h"
#include "lib/adc.h"
#include "lib/usart.h"

volatile unsigned char usartValue;
volatile unsigned char transmissionEnabled;
volatile uint16_t adcBuffer;
volatile uint8_t adcBufferValidFlag;

int main(void) {
    DDRB |= (1 << PINB0);
    sei();
    USARTInit(1);
    uint16_t adcCache;
    while (1) {
        while (adcBufferValidFlag == 0) {
            // Wait for ADC result
        }
        adcBufferValidFlag = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            adcCache = adcBuffer;
        }

        uint8_t l1 = adcCache;
        uint8_t l2 = adcCache >> 8;
        USARTSendByte(l1);
        USARTSendByte(l2);
    }
}

ISR(USART_RXC_vect) {
    usartValue = UDR;
    switch (usartValue) {
        case DEVICE_START_COMMAND:
            disableADC();
            ADCInit(0);
            PORTB |= (1 << PINB0);
            break;
        case DEVICE_STOP_COMMAND:
            disableADC();
            PORTB &= ~(1 << PINB0);
            break;
    }
}

ISR(ADC_vect) {
    adcBuffer = ADC;
    adcBufferValidFlag = 1;
}
