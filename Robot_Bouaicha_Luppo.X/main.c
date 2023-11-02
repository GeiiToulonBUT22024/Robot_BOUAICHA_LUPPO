#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "robot.h"

//Pour les include, on utilise < et >

unsigned int * result;
double captG;
double captM;
double captD;
int main(void)
{
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();
   
    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();
    
    InitTimer23();
    InitTimer1();
    InitPWM();

    InitADC1();



    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    
    while (1) { 

        if (ADCIsConversionFinished()){
            ADCClearConversionFinishedFlag();
            result = ADCGetResult();
            //pour 0V double var0 = result[0]*3.3/4096*3.2;
            //Pour 3.3Vdouble var1 = result[1]*3.3/4096*3.2;
            //val en tension: float captG = result[0]*3.3/4096*3.2;
            // val en analog float captG = result[0]*3.2;
            
            captG = result[0]*3.3/4096*3.2;
            robotState.distanceTeleGauche = 34/captG - 5;
            
            
            captM = result[1]*3.3/4096*3.2;
            robotState.distanceTeleCentre = 34/captM - 5;
            
            captD = result[2]*3.3/4096*3.2;
            robotState.distanceTeleDroite = 34/captD - 5;
            
            if(robotState.distanceTeleGauche > 30)
            {
                LED_BLANCHE = 1;
            }
            else {
                LED_BLANCHE = 0;
            }
            
            if(robotState.distanceTeleCentre > 30)
            {
                LED_BLEUE = 1;
            }
            else {
                LED_BLEUE = 0;
            }            
           // PWMUpdateSpeed();
            if(robotState.distanceTeleDroite > 30)
            {
                LED_ORANGE = 1;
            }            
            else{
                LED_ORANGE = 0;
            }    

        }
        //PWMSetSpeed(20,DROITE);
    //tESTER SI LA CONVERSION EST FINIE

    }
}//fin main 