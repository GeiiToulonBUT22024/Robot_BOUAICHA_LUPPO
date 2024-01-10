/* 
 * File:   CB_TX1.h
 * Author: TP_EO_6
 *
 * Created on 5 décembre 2023, 14:48
 */

#ifndef CB_TX1_H
#define	CB_TX1_H
void Sendmessage(unsigned char* message,int length);
void CB_TX1_Add(unsigned char value);
unsigned char CB_TX1_Get(void);
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void);
void SendOne(void);
int CB_TX1_IsTranmitting(void);
int CB_TX1_GetDataSize(void);
int CB_TX1_GetRemainingSize(void);


#endif


#ifdef	__cplusplus
extern "C" {
#endif




