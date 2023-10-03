/* 
 * File:   toolbox.h
 * Author: TP_EO_6
 *
 * Created on 26 septembre 2023, 10:21
 */

#ifndef TOOLBOX_H
#define	TOOLBOX_H
#define PI 3.141592653589793
float Max(float value, float value2);
float Min(float value, float value2);
float LimitToInterval(float value, float lowLimit, float highLimit);
float RadianToDegree(float value);
float DegreeToRadian(float value);
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TOOLBOX_H */
