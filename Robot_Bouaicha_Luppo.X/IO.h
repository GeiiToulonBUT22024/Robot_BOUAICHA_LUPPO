#ifndef IO_H
#define IO_H

//Affectation des pins des LEDS    
#define LED_BLANCHE_1 _LATJ6
#define LED_BLEUE_1 _LATJ5
#define LED_ORANGE_1 _LATJ4 
#define LED_ROUGE_1 _LATJ11
#define LED_VERTE_1 _LATH10

// Prototypes fonctions
void InitIO();
void LockIO();
void UnlockIO();


//Définitions des pins pour les hacheurs moteurs

#define MOTEUR1_IN1 _LATE0
#define MOTEUR1_IN2 _LATE1
#define MOTEUR6_IN1 _LATE2
#define MOTEUR6_IN2 _LATE3

//Configuration spcifique du moteur gauche
#define MOTEUR_GAUCHE_H_IO_OUTPUT MOTEUR1_IN1
#define MOTEUR_GAUCHE_L_IO_OUTPUT MOTEUR1_IN2
#define MOTEUR_GAUCHE_L_PWM_ENABLE IOCON1bits.PENL
#define MOTEUR_GAUCHE_H_PWM_ENABLE IOCON1bits.PENH
#define MOTEUR_GAUCHE_DUTY_CYCLE PDC1

//Configuration spcifique du moteur droit
#define MOTEUR_DROITE_H_IO_OUTPUT MOTEUR6_IN1
#define MOTEUR_DROITE_L_IO_OUTPUT MOTEUR6_IN2
#define MOTEUR_DROITE_L_PWM_ENABLE IOCON6bits.PENL
#define MOTEUR_DROITE_H_PWM_ENABLE IOCON6bits.PENH
#define MOTEUR_DROITE_DUTY_CYCLE PDC6


// Prototypes fonctions
void InitIO();
#endif /* IO_H */
