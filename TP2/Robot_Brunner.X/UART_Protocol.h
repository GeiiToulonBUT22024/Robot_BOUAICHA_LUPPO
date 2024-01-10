/* 
 * File:   UART_Protocol.h
 * Author: TP_EO_6
 *
 * Created on 10 janvier 2024, 10:36
 */

#ifndef UART_PROTOCOL_H
unsigned char UartCalculateChecksum(int msgFunction,int msgPayloadLength, unsigned char* msgPayload);
void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload);
#define	UART_PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* UART_PROTOCOL_H */

