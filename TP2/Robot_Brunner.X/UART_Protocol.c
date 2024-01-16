#include <xc.h>
#include "UART_Protocol.h"

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

unsigned char UartCalculateChecksum(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
    unsigned char  checksum = 0xFE;
     checksum ^= (unsigned char)msgFunction;
     checksum ^= (unsigned char)(msgFunction >> 8);
     checksum ^= (unsigned char)msgPayloadLength;
     checksum ^= (unsigned char)(msgPayloadLength >> 8);
    for (int i=0;i++;i<msgPayloadLength)
    {
        checksum ^= msgPayload[i];
    }
    return checksum;
}
void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
{
    unsigned char tableau[1 + 2 + 2 + msgPayloadLength + 1]; // voir tableau sur TP page22
    int pos = 0;
    tableau[pos++] = 0xFE;
    tableau[pos++] = (unsigned char)(msgFunction >> 8);
    tableau[pos++] = (unsigned char)msgFunction;
    tableau[pos++] = (unsigned char)(msgPayloadLength >> 8);
    tableau[pos++] = (unsigned char)(msgPayloadLength);
    for (int i = 0; i < msgPayloadLength; i++) {
        tableau[pos++] = msgPayload[i];
    }
    tableau[pos++] = UartCalculateChecksum(msgFunction,msgPayloadLength,msgPayload);
    unsigned char debug1=tableau[0];
    unsigned char debug2=tableau[1];
    unsigned char debug3=tableau[2];
    unsigned char debug4=tableau[3];
    unsigned char debug5=tableau[4];
    unsigned char debug6=tableau[5];
    unsigned char debug7=tableau[6];
    unsigned char debug8=tableau[7];
    unsigned char debug9=tableau[8];
    unsigned char debug10=tableau[9];
    unsigned char debug11=tableau[10];
    unsigned char debug12=tableau[11];
    unsigned char debug13=tableau[12];
    
    SendMessage(tableau, pos);
}
/*
void UartDecodeMessage(unsigned char c)
{
    //Fonction prenant en entree un octet et servant a reconstituer les trames
    ...

}
void UartProcessDecodedMessage(int function,
int payloadLength, unsigned char* payload)
{
    //Fonction appelee apres le decodage pour executer l?action
    //correspondant au message recu
    ...
}
*/
    //*************************************************************************/
    //Fonctions correspondant aux messages
    //*************************************************************************/
