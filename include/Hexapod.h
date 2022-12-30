#include "Leg.h"
/*
L1 ^ R1
L2   R2
L3 _ R3
*/
class Hexapod {
   private:
    Leg L1{36, 23, 22, LEFT};
    Leg L2{37, 33, 29, LEFT};
    Leg L3{8, 25, 28, LEFT};

    Leg R1{5, 2, 3, RIGHT};
    Leg R2{4, 7, 6, RIGHT};
    Leg R3{9, 10, 24, RIGHT};

   public:
    void set_calibration() {
        /* center points for the joints */
        L1.set_center(90, 94, 158);
        L2.set_center(85, 95, 152);
        L3.set_center(90, 94, 155);

        R1.set_center(80, 93, 150);
        R2.set_center(98, 102, 156);
        R3.set_center(90, 96, 145);
    }

    void idle_legs() {
        R1.idle_leg();
        R2.idle_leg();
        R3.idle_leg();

        L1.idle_leg();
        L2.idle_leg();
        L3.idle_leg();

        while (L3.move_flag) {
            R1.update_leg_joint(FEMUR);
            R2.update_leg_joint(FEMUR);
            R3.update_leg_joint(FEMUR);

            R1.update_leg_joint(TIBIA);
            R2.update_leg_joint(TIBIA);
            R3.update_leg_joint(TIBIA);

            L1.update_leg_joint(FEMUR);
            L2.update_leg_joint(FEMUR);
            L3.update_leg_joint(FEMUR);

            L1.update_leg_joint(TIBIA);
            L2.update_leg_joint(TIBIA);
            L3.update_leg_joint(TIBIA);
        }
    }

    /* move all femurs to the same angle */
    void move_all_femur(int angle) {
        R1.move_to_angle(FEMUR, angle);
        R2.move_to_angle(FEMUR, angle);
        R3.move_to_angle(FEMUR, angle);

        L1.move_to_angle(FEMUR, angle);
        L2.move_to_angle(FEMUR, angle);
        L3.move_to_angle(FEMUR, angle);
    }

    /* move all tibia to the same angle */
    void move_all_tibia(int angle) {
        R1.move_to_angle(TIBIA, angle);
        R2.move_to_angle(TIBIA, angle);
        R3.move_to_angle(TIBIA, angle);

        L1.move_to_angle(TIBIA, angle);
        L2.move_to_angle(TIBIA, angle);
        L3.move_to_angle(TIBIA, angle);
    }

    void flower_legs() {
        int femur_angle = 40;
        int tibia_angle = -20;
        R1.center_joint(COXA);
        R2.center_joint(COXA);
        R3.center_joint(COXA);

        L1.center_joint(COXA);
        L2.center_joint(COXA);
        L3.center_joint(COXA);

        move_all_femur(femur_angle);
        move_all_tibia(tibia_angle);

        while (L3.move_flag[TIBIA]) {
            R1.update_leg_joint(FEMUR);
            R2.update_leg_joint(FEMUR);
            R3.update_leg_joint(FEMUR);

            R1.update_leg_joint(TIBIA);
            R2.update_leg_joint(TIBIA);
            R3.update_leg_joint(TIBIA);

            L1.update_leg_joint(FEMUR);
            L2.update_leg_joint(FEMUR);
            L3.update_leg_joint(FEMUR);

            L1.update_leg_joint(TIBIA);
            L2.update_leg_joint(TIBIA);
            L3.update_leg_joint(TIBIA);
        }
    }

    /* spread the joints of all legs out like a starfish */
    void center_all_legs() {
        R1.center_leg();
        R2.center_leg();
        R3.center_leg();

        L1.center_leg();
        L2.center_leg();
        L3.center_leg();
    }
};
