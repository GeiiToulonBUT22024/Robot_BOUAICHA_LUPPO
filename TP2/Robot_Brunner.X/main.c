#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ToolBox.h"
#include "ADC.h"
#include "robot.h"
#include "main.h"
#include "UART.h"
#include "CB_TX1.h"
#include "CB_RX1.h"
#include <libpic30.h>

int rotation = 0;
int rotation2 = 0;
int reculer;
float vitessebase = 25.0;
int vitessebasereculer = -20;
float captG;
float captM;
float captD;
float captEG;
float captED;
float coef1;
float coef2;
float coef3;
float coef4;
unsigned char stateRobot;
char result[5];

/*void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            // PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            // PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
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

void SetNextRobotStateInAutomaticMode(void) {
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //Obstacle à droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30) //Obstacle à gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;

    //Détermination de l?état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot);
        stateRobot = nextStateRobot;
}*/

int main(void)
{
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();
    
    /****************************************************************************************************/
    //Initialisation PWM
    /****************************************************************************************************/
    InitPWM();

    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();
    
    /****************************************************************************************************/
    // Configuration initiale des timers ( je crois )
    /****************************************************************************************************/
    InitTimer1();
    InitTimer23();
    InitTimer4();
    InitADC1();
    InitUART();
    

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        
      /*  unsigned char payload[] = {'B', 'o', 'n', 'j', 'o', 'u', 'r'};
        
        int i;
        for(i=0; i< CB_RX1_GetDataSize(); i++)
        {
            unsigned char c = CB_RX1_Get();
            SendMessage(&c,1);
        }
       __delay32(40000000);
       */
       
     /* SendMessage("Bonjour\r\n",11);
    __delay32(10000);*/
        
        
        if (ADCIsConversionFinished()) 
        {
            
            unsigned int * result = ADCGetResult();

            captED = ((float) result [0]+1)* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeDroit = 34.0 / captED - 5;

            
            //Capteur Droite
            captD = ((float) result [1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34.0 / captD - 5;
            if (robotState.distanceTelemetreDroit>=256){
            robotState.distanceTelemetreDroit=255;
            }
            unsigned char D = robotState.distanceTelemetreDroit;
            unsigned char* test[1];
            test[0] = 0x08;
            UartEncodeAndSendMessage(0x0030,1, test);
            __delay32(1000);
            //Capteur Centre
            captM = ((float) result [2])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34.0 / captM - 5;
            if (robotState.distanceTelemetreCentre>=256){
            robotState.distanceTelemetreCentre=255;
            }
            unsigned char M = robotState.distanceTelemetreCentre;
            UartEncodeAndSendMessage(0x0031,3,M);
            __delay32(1000);
            
            //Capteur Gauche
            captG = ((float) result [3])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34.0 / captG - 5;
            if (robotState.distanceTelemetreGauche>=256){
            robotState.distanceTelemetreGauche=255;
            }
            unsigned char G = robotState.distanceTelemetreGauche;
            UartEncodeAndSendMessage(0x0032,3,G);
            __delay32(1000);
            

            captEG = ((float) result [4])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeGauche = 34.0 / captEG - 5;
            //UartEncodeAndSendMessage(0x0030,7, payload);

          
            // Strat 
            coef1 = (robotState.distanceTelemetreExtremeGauche) / (robotState.distanceTelemetreExtremeDroit);
            coef2 = (robotState.distanceTelemetreGauche) / (robotState.distanceTelemetreDroit);
            coef3 = 1 / coef1;
            coef4 = 1 / coef2;

/*
            if(robotState.distanceTelemetreGauche>30)
            {
               robotState.distanceTelemetreGauche=30; 
            }
            else if(robotState.distanceTelemetreExtremeDroit>30)
            {
                robotState.distanceTelemetreExtremeDroit=30;
            }
            else if(robotState.distanceTelemetreDroit>30)
            {
                    robotState.distanceTelemetreDroit=30;
            }
            else if (robotState.distanceTelemetreExtremeGauche>30)
            {
                robotState.distanceTelemetreExtremeGauche=30;
            }
  */          
            /*PWMSetSpeedConsigne(vitessebase, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitessebase, MOTEUR_GAUCHE);

            if (coef1<0.7||robotState.distanceTelemetreExtremeGauche<15)
            {
                PWMSetSpeedConsigne(vitessebase*0.7, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*coef1*0.2, MOTEUR_GAUCHE);                
            }
            else if (robotState.distanceTelemetreCentre < 15)
            {
                PWMSetSpeedConsigne(vitessebase*0.8, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*0.8, MOTEUR_GAUCHE);  
            }
            else if (coef3 < 0.7|| robotState.distanceTelemetreExtremeDroit<17)
            {
                PWMSetSpeedConsigne(vitessebase*coef3*0.2, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*0.8, MOTEUR_GAUCHE);
                
            } else if (coef2<0.7) 
            {
                PWMSetSpeedConsigne(vitessebase*0.8, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*coef2*0.2, MOTEUR_GAUCHE);
            }
            else if (coef2<0.6|| robotState.distanceTelemetreGauche<17)
            {
                PWMSetSpeedConsigne(vitessebase*coef2, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*coef2*coef2, MOTEUR_GAUCHE);                    
            }
            else if (coef4<0.8) 
            {
                PWMSetSpeedConsigne(vitessebase*coef4, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*0.8, MOTEUR_GAUCHE);
            }
            else if (coef4<0.6 || robotState.distanceTelemetreDroit<15)
            {
                PWMSetSpeedConsigne(vitessebase*coef4*coef4*coef4, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*coef4*coef4, MOTEUR_GAUCHE);                    
            }
            else if((robotState.distanceTelemetreCentre < 22 && robotState.distanceTelemetreDroit<23)||(robotState.distanceTelemetreDroit<23 && robotState.distanceTelemetreExtremeDroit<23 ))
            {
                PWMSetSpeedConsigne(vitessebase*0.5, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);                
            }
            else if((robotState.distanceTelemetreCentre < 20 && robotState.distanceTelemetreGauche<22)|| (robotState.distanceTelemetreExtremeGauche<22 && robotState.distanceTelemetreGauche<22))
            {
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*0.5, MOTEUR_GAUCHE);                
            }
            else if((robotState.distanceTelemetreCentre < 20 && (robotState.distanceTelemetreDroit<22 && robotState.distanceTelemetreGauche<22))  )
            {
                PWMSetSpeedConsigne(vitessebase*0.1, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-vitessebase*0.1, MOTEUR_GAUCHE);   
            }
            if(robotState.distanceTelemetreCentre < 17.0)
            {
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitessebase*0.5, MOTEUR_GAUCHE);  
            }
            ADCClearConversionFinishedFlag();*/
        }
        int i;
        for(i=0; i< CB_RX1_GetDataSize(); i++)
        {
        unsigned char c = CB_RX1_Get();
        SendMessage(&c,1);
        }
        __delay32(10000);
    }
}


     /*if (robotState.distanceTelemetreCentre <= 10) {
         LED_BLEUE = 1;
         reculer=1;
     } 
     else if (robotState.distanceTelemetreCentre > 10){
         LED_BLEUE = 0;
         reculer=0;
     }
     if (robotState.distanceTelemetreDroit <= 25 && reculer==0) {
         LED_BLANCHE = 1;
         rotation-=15;
         if (rotation<-15)
         {
             rotation=-15;
         }
     } 
     if (robotState.distanceTelemetreDroit > 25 && reculer==0) {
         LED_ORANGE = 1;
        rotation+=15;
         if (rotation>0)
         {
             rotation=0;
         }
}
            
         if (robotState.distanceTelemetreGauche <= 25 && reculer==0) {

         rotation+=15;
         if (rotation>15)
         {
             rotation=15;
         }
     } 
     if (robotState.distanceTelemetreGauche > 25 && reculer==0) {
        rotation-=15;
         if (rotation<0)
         {
             rotation=0;
         }
}
            
            
            
     if (robotState.distanceTelemetreExtremeGauche <= 15 && reculer==0) {

         rotation2+=7;
         if (rotation2>15)
         {
             rotation2=15;
         }
     } 
     if (robotState.distanceTelemetreExtremeGauche > 15 && reculer==0) {
        rotation2-=15;
         if (rotation2<0)
         {
             rotation2=0;
         }
}
   
            
            
     if (robotState.distanceTelemetreExtremeDroit <= 15 && reculer==0) {
         LED_BLANCHE = 1;
         rotation2-=7;
         if (rotation2<-7)
         {
             rotation2=-7;
         }
     } 
     if (robotState.distanceTelemetreExtremeDroit > 15 && reculer==0) {
         LED_ORANGE = 1;
        rotation2+=7;
         if (rotation2>0)
         {
             rotation2=0;
         }
}
            
            
if(rotation>0){
        PWMSetSpeedConsigne(5, MOTEUR_DROIT); // le moteur tourne a droite
        PWMSetSpeedConsigne(rotation+rotation2, MOTEUR_GAUCHE);
}
if(rotation<0){
        PWMSetSpeedConsigne(rotation+rotation2, MOTEUR_DROIT);// le moteur tourne a gauche 
        PWMSetSpeedConsigne(5, MOTEUR_GAUCHE);
}
   
if(reculer==1){
PWMSetSpeedConsigne(vitessebase*0.4, MOTEUR_DROIT);//le moteur tourne sur lui meme 
PWMSetSpeedConsigne(vitessebase*0.4*-1, MOTEUR_GAUCHE);
}
        
if(rotation==0 && rotation2==0 && reculer==0){
        PWMSetSpeedConsigne(vitessebase, MOTEUR_DROIT); // le moteur tourne a droite
        PWMSetSpeedConsigne(vitessebase, MOTEUR_GAUCHE);
}
        }}}*/