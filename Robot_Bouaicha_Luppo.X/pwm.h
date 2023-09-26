/* 
 * File:   pwm.h
 * Author: TP_EO_6
 *
 * Created on 26 septembre 2023, 10:27
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

void InitPWM(void);
void PWMSetSpeed(float vitesseEnPourcents);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

