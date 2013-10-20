#ifndef USART_H
#define	USART_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#ifndef F_CPU
#warning "F_CPU not defined, using default value"
#define F_CPU 1000000UL
#endif

#ifndef USART_BAUDRATE
#warning "USART_BAUDRATE not defined, using default value"
#define USART_BAUDRATE 9600
#endif

/**
 * Initialize USART. Settings: 8-bit data length, 1 stop bit, no parity.
 * To handle interrupts:
 *
 * volatile unsigned char value;
 * ISR(USART_RXC_vect) {
 *     value = UDR; // Read UDR register   
 * }
 * @param startService if set RXCIE will be enabled
 */
void USARTInit(char startService) {
    UBRRH &= ~(1 << URSEL);
    uint8_t baundPrescale = lround(((F_CPU / (USART_BAUDRATE * 16UL))) - 1);
    UBRRH = (unsigned char) (baundPrescale >> 8);
    UBRRL = (unsigned char) baundPrescale;
    UCSRB = (1 << TXEN) | (1 << RXEN) | ((int) startService << RXCIE);
    UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0); // 1 stop bit, 8-bit data length
}

void USARTSendByte(char data) {
    while ((UCSRA & (1 << UDRE)) == 0) {
        // Do nothing
    }
    UDR = data;
}

uint8_t USARTReceiveByte() {
    while ((UCSRA & (1 << RXC)) == 0) {
        // Do nothing
    }
    return UDR;
}

/**
 * Send an array of characters over USART. '\n\r' is appended at the end.
 * @param str[] array of maximum 256 characters
 */
void USARTSendString(char str[]) {
    uint8_t i = 0;
    while (str[i] != '\0') {
        USARTSendByte(str[i]);
        i++;
    }
    USARTSendByte('\n');
    USARTSendByte('\r');
}

#endif	/* USART_H */
