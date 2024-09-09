/* 
 * File:   pwm.h
 * Author: TP_EO_6
 *
 * Created on 26 septembre 2023, 10:27
 */

#ifndef PWM_H
#define	PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

#ifdef	__cplusplus
extern "C" {
#endif

void InitPWM();
void PWMSetSpeed(float vitesseEnPourcents);
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents,char moteur);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

