#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "robot.h"
#include "main.h"

//Pour les include, on utilise < et >

unsigned int * result;
//double captG;
//double captM;
//double captD;
float captG;
float captM;
float captD;
unsigned char stateRobot;

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
    //InitTimer23();
    InitPWM();

    InitTimer1();
    InitADC1();
    InitTimer4();


    /****************************************************************************************************/
    // Boucle Principale
    /******************************************-+**********************************************************/

    while (1) {
        

        if (ADCIsConversionFinished()) {
           
            result = ADCGetResult();
            //pour 0V double var0 = result[0]*3.3/4096*3.2;
            //Pour 3.3Vdouble var1 = result[1]*3.3/4096*3.2;
            //val en tension: float captG = result[0]*3.3/4096*3.2;
            // val en analog float captG = result[0]*3.2;

            //float captG = result[0]*3.3 / 4096 * 3.2;
            //robotState.distanceTelemetreGauche = 34 / captG - 5;


            //float captM = result[1]*3.3 / 4096 * 3.2;
            //robotState.distanceTelemetreCentre = 34 / captM - 5;

            //float captD = result[2]*3.3 / 4096 * 3.2;
            //robotState.distanceTelemetreDroit = 34 / captD - 5;
            if (ADCIsConversionFinished() == 1)
            {
                ADCClearConversionFinishedFlag();
                unsigned int * result = ADCGetResult();
                float volts = ((float) result [2])* 3.3 / 4096 * 3.2;
                robotState.distanceTelemetreDroit = 34 / volts - 5;
                volts = ((float) result [1])* 3.3 / 4096 * 3.2;
                robotState.distanceTelemetreCentre = 34 / volts - 5;
                volts = ((float) result [0])* 3.3 / 4096 * 3.2;
                robotState.distanceTelemetreGauche = 34 / volts - 5;
            }
            
            if (robotState.distanceTelemetreGauche > 30) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }

            if (robotState.distanceTelemetreCentre > 30) {
                LED_BLEUE = 1;
            } else {
                LED_BLEUE = 0;
            }
            // PWMUpdateSpeed();
            if (robotState.distanceTelemetreDroit > 30) {
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }
            ADCClearConversionFinishedFlag();
        }
        
    }
    
}//fin main

void OperatingSystemLoop(void)
{
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);//10
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);//10
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode()
{
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //D�termination de la position des obstacles en fonction des t�l�m�tres
    if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //Obstacle � droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30) //Obstacle � gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;

    //D�termination de l?�tat � venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

    //Si l?on n?est pas dans la transition de l?�tape en cours
    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;
    }
}
