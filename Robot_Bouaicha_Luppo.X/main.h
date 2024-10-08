/* 
 * File:   main.h
 * Author: TP_EO_6
 *
 * Created on 6 septembre 2023, 08:13
 */

#ifndef MAIN_H
#define	MAIN_H
//#define DROITE 0
//#define GAUCHE 1
// Configuration des �paramtres du chip
#define FCY 60000000
#define STATE_ATTENTE 0
#define STATE_ATTENTE_EN_COURS 1
#define STATE_AVANCE 2
#define STATE_AVANCE_EN_COURS 3
#define STATE_TOURNE_GAUCHE 4
#define STATE_TOURNE_GAUCHE_EN_COURS 5
#define STATE_TOURNE_FGAUCHE 6
#define STATE_TOURNE_FGAUCHE_EN_COURS 7
#define STATE_TOURNE_DROITE 8
#define STATE_TOURNE_DROITE_EN_COURS 9
#define STATE_TOURNE_FDROITE 10
#define STATE_TOURNE_FDROITE_EN_COURS 11
#define STATE_TOURNE_SUR_PLACE_GAUCHE 12
#define STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS 13
#define STATE_TOURNE_SUR_PLACE_DROITE 14
#define STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS 15
#define STATE_ARRET 16
#define STATE_ARRET_EN_COURS 17
#define STATE_RECULE 18
#define STATE_RECULE_EN_COURS 19





#define PAS_D_OBSTACLE 0
#define OBSTACLE_A_GAUCHE 1
#define OBSTACLE_A_DROITE 2
#define OBSTACLE_EN_FACE 3
#define OBSTACLE_A_EXTDROITE 4
#define OBSTACLE_A_EXTGAUCHE 5
/**/
void OperatingSystemLoop(void);
void SetNextRobotStateInAutomaticMode();
#endif	/* MAIN_H */

