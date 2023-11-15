#ifndef ROBOT_H
#define ROBOT_H
typedef struct robotStateBITS {
<<<<<<< HEAD

    union {

        struct {
            unsigned char taskEnCours;
            float vitesseGaucheConsigne;
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            float distanceTeleGauche;
            float distanceTeleCentre;
            float distanceTeleDroite;
            
            
        };
    };
=======
union {
struct {
unsigned char taskEnCours;
float vitesseGaucheConsigne;
float vitesseGaucheCommandeCourante;
float vitesseDroiteConsigne;
float vitesseDroiteCommandeCourante;
};
};
>>>>>>> parent of 94be18d (RobotV4.3)
} ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */