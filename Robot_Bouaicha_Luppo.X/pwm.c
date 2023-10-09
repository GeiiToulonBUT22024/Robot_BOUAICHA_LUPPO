#include <xc.h> // library xc.h inclut tous les uC
#include "IO.h"
#include "PWM.h"
#include "Robot.h"
#include "toolbox.h"

#define PWMPER 40.0

unsigned char acceleration = 20;

void InitPWM(void)
{
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //Période en pourcentage
    //éRglage PWM moteur 1 sur hacheur 1
    IOCON1bits.POLH = 1; //High = 1 and active on low =0
    IOCON1bits.POLL = 1; //High = 1
    IOCON1bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON1 = 0x0003; //Désactive la gestion des faults
    //Reglage PWM moteur 2 sur hacheur 6
    IOCON6bits.POLH = 1; //High = 1
    IOCON6bits.POLL = 1; //High = 1
    IOCON6bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON6 = 0x0003; //éDsactive la gestion des faults
    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}

/*void PWMSetSpeed(float vitesseEnPourcents)
{

    LED_ORANGE = 0;
}
 */

/*void PWMSetSpeed(int moteur,float vitesseEnPourcents){

    if(moteur == GAUCHE){    
        robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
                    
    if(vitesseEnPourcents>=0)
    {
        MOTEUR_GAUCHE_L_PWM_ENABLE = 0; //Pilotage de la pin en mode IO // Pour pilotage en sens inverse
        MOTEUR_GAUCHE_L_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
        //MOTEUR_GAUCHE_H_IO_OUTPUT = 0;
       
    }
    else if ((vitesseEnPourcents<0))//
    {
        // Pilotage en sens inverse 
        MOTEUR_GAUCHE_L_PWM_ENABLE = 1; //Pilotage de la pin en mode IO // Pour pilotage en sens inverse
        //MOTEUR_GAUCHE_L_IO_OUTPUT = 0; //Mise à 1 de la pin
        MOTEUR_GAUCHE_H_PWM_ENABLE = 0; //Pilotage de la pin en mode PWM
        MOTEUR_GAUCHE_H_IO_OUTPUT = 1;
        
    }
                MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante*PWMPER);
          }
    if(moteur == DROITE){
        robotState.vitesseDroiteCommandeCourante = vitesseEnPourcents;
                    
                if(vitesseEnPourcents>=0)
    {
        
        
        MOTEUR_DROITE_L_PWM_ENABLE = 1; //Pilotage de la pin en mode IO // Pour pilotage en sens inverse
        MOTEUR_DROITE_L_IO_OUTPUT = 0; //Mise à 1 de la pin
        MOTEUR_DROITE_H_PWM_ENABLE = 0; //Pilotage de la pin en mode PWM
        MOTEUR_DROITE_H_IO_OUTPUT = 1;
       
    }
    else if ((vitesseEnPourcents<0))//
    {
        // Pilotage en sens inverse 
        MOTEUR_DROITE_L_PWM_ENABLE = 0; //Pilotage de la pin en mode IO // Pour pilotage en sens inverse
        MOTEUR_DROITE_L_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_DROITE_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
        MOTEUR_DROITE_H_IO_OUTPUT = 0;
        
    }
        
        MOTEUR_DROITE_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante*PWMPER); 
    }
        
       
        
        LED_ORANGE = 0;
}
 */

void PWMUpdateSpeed()
{
    // Cette fonction est appelée sur timer et permet de suivre des rampes d?accélération
    if (robotState.vitesseDroiteCommandeCourante < robotState.vitesseDroiteConsigne) {
        robotState.vitesseDroiteCommandeCourante = Min((robotState.vitesseDroiteCommandeCourante + acceleration), (robotState.vitesseDroiteConsigne));
    }
    if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne) {
        robotState.vitesseDroiteCommandeCourante = Max((robotState.vitesseDroiteCommandeCourante - acceleration), (robotState.vitesseDroiteConsigne));
    }
    if (robotState.vitesseDroiteCommandeCourante > 0) {
        MOTEUR_DROITE_L_PWM_ENABLE = 0; //pilotage de la pin en mode IO
        MOTEUR_DROITE_L_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_DROITE_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_DROITE_H_PWM_ENABLE = 0; //pilotage de la pin en mode IO
        MOTEUR_DROITE_H_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_DROITE_L_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_DROITE_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante) * PWMPER;

    if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Min(
            robotState.vitesseGaucheCommandeCourante + acceleration,
            robotState.vitesseGaucheConsigne);
    if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Max(robotState.vitesseGaucheCommandeCourante - acceleration, robotState.vitesseGaucheConsigne);

    if (robotState.vitesseGaucheCommandeCourante > 0) {
        MOTEUR_GAUCHE_L_PWM_ENABLE = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_L_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_GAUCHE_H_PWM_ENABLE = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_H_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_L_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante) * PWMPER;
}

