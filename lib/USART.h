/* 
 * File:   usart.h
 * Author: Ajarax
 *
 * Created on November 8, 2012, 12:14 AM
 */

#ifndef USART_H
#define	USART_H

/************************************************************************/
/*                               SETTINGS	                        */
/* UBRRL = 0xCF for USBasp                                              */
/* UBRRL = 0x33 for Terminal                                            */
/************************************************************************/
#define F_CPU 16000000UL
#define USART_BAUDRATE 115200
#define BAUD_PRESCALE 8 //(((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/**
 * Initialize USART
 * 
 * @param startService
 * If set RXCIE will be enabled
 * 
 * To handle interrupts:
 *
 * volitile unsigned char value;
 * ISR(USART_RXC_vect) {
 *     value = UDR; // Read UDR register   
 * }
 */
void USARTInit(bool startService = false) {
    UBRRH = (unsigned char) (BAUD_PRESCALE >> 8);
    UBRRL = (unsigned char) BAUD_PRESCALE;
    UCSRB = (1 << TXEN) | (1 << RXEN) | ((int) startService << RXCIE);
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

