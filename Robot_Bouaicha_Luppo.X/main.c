#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "pwm.h"
#include "ADC.h"
#include "robot.h"
#include "main.h"

//Pour les include, on utilise < et >

unsigned int * result;
float captG;
float captM;
float captD;
unsigned char stateRobot;

int main(void) {

    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();
    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();
    LED_BLANCHE_1 = 0;
    LED_BLEUE_1 = 0;
    LED_ORANGE_1 = 0;
    LED_ROUGE_1 = 0;
    LED_VERTE_1 = 0;
    InitADC1();
    //PWMSetSpeed(15.0);
    InitTimer1();
    InitTimer4();
    InitTimer23();
    InitPWM();
    
    /****************************************************************************************************/
    // Boucle Principale
    /******************************************-+**********************************************************/

    while (1) {

        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [4])* 3.3 / 4096;
            robotState.captExtD = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.captD = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.captM = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.captG = 34 / volts - 5;
            volts = ((float) result [0])* 3.3 / 4096;
            robotState.captExtG = 34 / volts - 5;
        

            if (robotState.captExtG < 30) 
                LED_BLANCHE_1 = 1;
            else 
                LED_BLANCHE_1 = 0;
            
            if (robotState.captG < 30) 
                LED_BLEUE_1 = 1;
            else 
                LED_BLEUE_1 = 0;
            
            if (robotState.captM < 30) 
                LED_ORANGE_1 = 1;
            else 
                LED_ORANGE_1 = 0;
            
            //PWMUpdateSpeed();
            if (robotState.captD < 30) {
                LED_ROUGE_1 = 1;
            } else {
                LED_ROUGE_1 = 0;
            }
            // PWMUpdateSpeed();
            if (robotState.captExtD < 30) {
                LED_VERTE_1 = 1;
            } else {
                LED_VERTE_1 = 0;
            }
        }
        ADCClearConversionFinishedFlag();

        //PWMSetSpeedConsigne(15.0,MOTEUR_DROIT);
    }
}

//fin main

void OperatingSystemLoop(void)
{
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
            break;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(-10.0, MOTEUR_DROIT);//10
            PWMSetSpeedConsigne(-10.0, MOTEUR_GAUCHE);//10
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(-30.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_FGAUCHE:
            PWMSetSpeedConsigne(-15.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-5.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_FGAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_FGAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-30.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_FDROITE:
            PWMSetSpeedConsigne(-5.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_FDROITE_EN_COURS;
            break;
        case STATE_TOURNE_FDROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(-15.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(15.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15.0, MOTEUR_GAUCHE);
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

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.captExtD > 30.0 && robotState.captD < 30.0 &&
            robotState.captM > 20.0 &&
            robotState.captG > 30.0 && robotState.captExtG > 30.0) //Obstacle à droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.captExtD < 30.0 &&
            robotState.captD > 20.0 &&
            robotState.captG > 30.0 && robotState.captExtG > 30.0) //Obstacle à extdroite
        positionObstacle = OBSTACLE_A_EXTDROITE;
    else if (robotState.captExtD > 30.0 && robotState.captD > 30.0 &&
            robotState.captM > 20 &&
            robotState.captG > 30 && robotState.captExtG < 30) //Obstacle à extgauche
        positionObstacle = OBSTACLE_A_EXTGAUCHE;
    else if (robotState.captExtD > 30.0 && robotState.captD > 30 &&
            robotState.captM > 20 &&
            robotState.captG < 30 && robotState.captExtG > 30) //Obstacle à gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.captM < 20) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.captExtD > 30.0 && robotState.captD > 30 &&
            robotState.captM > 20 && 
            robotState.captG > 30 && robotState.captExtG > 3) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;

    //Détermination de l?état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_EXTDROITE)
        nextStateRobot = STATE_TOURNE_FGAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_A_EXTGAUCHE)
        nextStateRobot = STATE_TOURNE_FDROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;
    }
}
