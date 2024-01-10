#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "CB_TX1.h"


////// IMPORTANT
#define CBTX1_BUFFER_SIZE 128 // taille buffer
int cbTx1Head=0; //  indice t�te (bouge)
int cbTx1Tail=0; // indice queue (bouge)
int cheat=0;
unsigned char cbTx1Buffer[CBTX1_BUFFER_SIZE]; // initialise buffer = tableau
///////

int isTransmitting = 0;
void SendMessage(unsigned char* message, int length)
{
    unsigned char i=0;

    if(CB_TX1_GetRemainingSize()>length)
    {
        //On peut �crire le message
        for(i=0;i<length;i++)
            CB_TX1_Add(message[i]);
        if(!CB_TX1_IsTranmitting())
            SendOne();
    }
}


void CB_TX1_Add(unsigned char value) // ajoute la valeur que tu rentre dans le tbaleau � l'indice de la queue 
{
  cbTx1Buffer[cbTx1Tail] =value;
  cbTx1Tail++;
  if (cbTx1Tail > 127)
  {
  cbTx1Tail=0;
  }
}
unsigned char CB_TX1_Get(void)// renvoyer la valeur dans le tableau a la tete
{
    char value = cbTx1Buffer[cbTx1Head];
    cbTx1Head++;
    if (cbTx1Head > 127){
      cbTx1Head=0;
    }
    return value;
}
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) 
   {
    IFS0bits.U1TXIF = 0; // clear TX interrupt flag
    if (cbTx1Tail!=cbTx1Head)
        {
            SendOne();
        }
    else
        isTransmitting = 0;
}
void SendOne(void)
{
    isTransmitting = 1;
    unsigned char value=CB_TX1_Get();
    U1TXREG = value; // Transmit one character
  
}
int CB_TX1_IsTranmitting(void)
{
   return isTransmitting;
}
int CB_TX1_GetDataSize(void)
{
    //return size of data stored in circular buffer
    int dataSize;
    dataSize=(CBTX1_BUFFER_SIZE- CB_TX1_GetRemainingSize());
    return dataSize;
}
int CB_TX1_GetRemainingSize(void)
{
    //return size of remaining size in circular buffer
    int remainingSize;
    if (cbTx1Head>cbTx1Tail){
         remainingSize=(CBTX1_BUFFER_SIZE-cbTx1Head)+cbTx1Tail;
    }
    else
        remainingSize=(CBTX1_BUFFER_SIZE-cbTx1Tail)+cbTx1Head ;
    
    return remainingSize;
}