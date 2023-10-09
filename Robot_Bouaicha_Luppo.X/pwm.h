/* 
 * File:   pwm.h
 * Author: TP_EO_6
 *
 * Created on 26 septembre 2023, 10:27
 */

#ifndef PWM_H
#define	PWM_H
#define DROITE 0
#define GAUCHE 1

#ifdef	__cplusplus
extern "C" {
#endif

void InitPWM();
//void PWMSetSpeed(int moteur,float vitesseEnPourcents);
void PWMUpdateSpeed();


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

