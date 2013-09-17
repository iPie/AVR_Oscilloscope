/* 
 * File:   usart.h
 * Author: Ajarax
 *
 * Created on November 8, 2012, 12:14 AM
 */

#ifndef USART_H
#define	USART_H

#define F_CPU 16000000UL
#define USART_BAUDRATE 500000
#define BAUD_PRESCALE 1 //(((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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
void USARTInit(bool startService = false) {
    UBRRH &= ~(1 << URSEL);
    UBRRH = (unsigned char) (BAUD_PRESCALE >> 8);
    UBRRL = (unsigned char) BAUD_PRESCALE;
    UCSRB = (1 << TXEN) | (1 << RXEN) | ((int) startService << RXCIE);
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); // 1 stop bit, 8-bit data length

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
