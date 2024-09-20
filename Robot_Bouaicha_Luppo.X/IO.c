/*
 * File:   IO.c
 */

#include <xc.h>
#include "IO.h"

void InitIO()
{
    //****************************************************************************************************/
    // Declaration des pin Analogiques
    //****************************************************************************************************/
    ANSELA=0;             //Desactivation de toutes entree analogique
    ANSELB=0;             //Desactivation de toutes entree analogique
    ANSELD=0;             //Desactivation de toutes entree analogique
    ANSELC=0;             //Desactivation de toutes entree analogique
    ANSELE=0;             //Desactivation de toutes entree analogique
    ANSELG=0;             //Desactivation de toutes entree analogique

    // Configuration des sorties

    //******* LED ***************************
    _TRISJ6 = 0;  // LED Blanche
    _TRISJ5 = 0; //LED Orange
    _TRISJ4 = 0; // LED Bleue
    _TRISJ11 = 0; // LED Rouge
    _TRISH10 = 0; // LED Verte 
    

    
    //****** Moteurs ************************

    // Configuration des entrées
    _TRISE0=0;
    _TRISE1=0;
    _TRISE2=0;
    _TRISE3=0;

    //*************************************************************
    // Unlock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON & ~(1 << 6));

    //Assignation des remappable pins

    _U1RXR = 78; //Remappe la RPI24 sur l?éentre Rx1
    _RP79R = 0b00001; //Remappe la sortie Tx1 vers RP36
    
  // Configuration des pins remappables  
    //******************** QEI *****************
    _QEA2R = 97; //assign QEI A to pin RP97
    _QEB2R = 96; //assign QEI B to pin RP96
    _QEA1R = 70; //assign QEI A to pin RP70
    _QEB1R = 69; //assign QEI B to pin RP69
    //*************************************************************
    // Lock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON | (1 << 6));
    /****************************************************************************************************/
    // Gestion des pin remappables
    /****************************************************************************************************/
    UnlockIO(); // On unlock les registres d'entrées/sorties, ainsi que les registres des PPS
    
    //Assignation des remappable pins
        
    LockIO(); // On lock les registres d'entrées/sorties, ainsi que les registres des PPS
}


void LockIO() {
    asm volatile ("mov #OSCCON,w1 \n"
                "mov #0x46, w2 \n"
                "mov #0x57, w3 \n"
                "mov.b w2,[w1] \n"
                "mov.b w3,[w1] \n"
                "bset OSCCON, #6":: : "w1", "w2", "w3");
}

void UnlockIO() {
    asm volatile ("mov #OSCCON,w1 \n"
                "mov #0x46, w2 \n"
                "mov #0x57, w3 \n"
                "mov.b w2,[w1] \n"
                "mov.b w3,[w1] \n"
                "bclr OSCCON, #6":: : "w1", "w2", "w3");
}