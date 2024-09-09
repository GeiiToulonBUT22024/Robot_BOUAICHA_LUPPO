#ifndef ROBOT_H
#define ROBOT_H
typedef struct robotStateBITS {
    union {
        struct {
                unsigned char taskEnCours;
                float vitesseGaucheConsigne;
                float vitesseGaucheCommandeCourante;
                float vitesseDroiteConsigne;
                float vitesseDroiteCommandeCourante;
                float captExtG;
                float captG;
                float captM;
                float captD;
                float captExtD;
                };
          };
} ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState;

#endif /* ROBOT_H */