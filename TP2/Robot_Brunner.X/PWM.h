void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, int MOTOR );
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, int moteur);
#define MOTEUR_DROIT 1
#define MOTEUR_GAUCHE 0