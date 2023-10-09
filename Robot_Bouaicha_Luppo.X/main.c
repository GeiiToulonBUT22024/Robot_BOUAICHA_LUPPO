#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"

//Pour les include, on utilise < et >

int main(void)
{
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entr�es sorties
    /****************************************************************************************************/
    InitIO();
    
    InitTimer23();
    InitTimer1();
    InitPWM();
    
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;
   
    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) { 
        PWMUpdateSpeed();
        //PWMSetSpeed(20,DROITE);
    }
}//fin main 