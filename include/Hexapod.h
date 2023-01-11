#include "Leg.h"
/*
L1 ^ R1
L2   R2
L3 _ R3
*/

enum Legs { LEFT1, LEFT2, LEFT3, RIGHT1, RIGHT2, RIGHT3 } typedef Legs;

class Hexapod {
   private:
    Leg L1{36, 23, 22};
    Leg L2{37, 33, 29};
    Leg L3{8, 25, 28};

    Leg R1{5, 2, 3};
    Leg R2{4, 7, 6};
    Leg R3{9, 10, 24};

   public:
    /* calibrate servo center angles */
    void set_calibration() {
        /* center points for the joints */
        L1.set_center(90, 94, 158);
        L2.set_center(85, 95, 152);
        L3.set_center(90, 94, 155);

        R1.set_center(80, 93, 150);
        R2.set_center(98, 102, 156);
        R3.set_center(90, 93, 145);
    }

    void initialize() {
        L1.set_initial_stance(POWER);
        L1.initialize_leg_timer();
        L1.initialize_stance_points(POWER, 25, 10, 65);
        L1.initialize_stance_points(POWER_MIDDLE, 15, 40, 110);
        L1.initialize_stance_points(SWING, -20, 45, 135);
        L1.initialize_stance_points(SWING_MIDDLE, -2, 60, 100);

        L2.set_initial_stance(SWING);
        L2.initialize_leg_timer();
        L2.initialize_stance_points(POWER, 23, 43, 120);
        L2.initialize_stance_points(POWER_MIDDLE, 0, 43, 120);
        L2.initialize_stance_points(SWING, -23, 43, 120);
        L2.initialize_stance_points(SWING_MIDDLE, 0, 50, 100);

        L3.set_initial_stance(POWER);
        L3.initialize_leg_timer();
        L3.initialize_stance_points(POWER, 25, 45, 135);
        L3.initialize_stance_points(POWER_MIDDLE, -2, 40, 110);
        L3.initialize_stance_points(SWING, -20, 10, 65);
        L3.initialize_stance_points(SWING_MIDDLE, 15, 60, 100);

        R1.set_initial_stance(SWING);
        R1.initialize_leg_timer();
        R1.initialize_stance_points(POWER, -25, 10, 65);
        R1.initialize_stance_points(POWER_MIDDLE, -15, 40, 110);
        R1.initialize_stance_points(SWING, 20, 45, 135);
        R1.initialize_stance_points(SWING_MIDDLE, 2, 60, 100);

        R2.set_initial_stance(POWER);
        R2.initialize_leg_timer();
        R2.initialize_stance_points(POWER, -23, 43, 120);
        R2.initialize_stance_points(POWER_MIDDLE, 0, 43, 120);
        R2.initialize_stance_points(SWING, 23, 43, 120);
        R2.initialize_stance_points(SWING_MIDDLE, 0, 50, 100);

        R3.set_initial_stance(SWING);
        R3.initialize_leg_timer();
        R3.initialize_stance_points(POWER, -25, 45, 135);
        R3.initialize_stance_points(POWER_MIDDLE, 2, 40, 110);
        R3.initialize_stance_points(SWING, 20, 10, 65);
        R3.initialize_stance_points(SWING_MIDDLE, -15, 60, 100);

        L1.initialize_turn_points(POWER, 30, 30, 100);
        L1.initialize_turn_points(POWER_MIDDLE, 0, 30, 100);
        L1.initialize_turn_points(SWING, -30, 30, 100);
        L1.initialize_turn_points(SWING_MIDDLE, 0, 50, 90);

        L2.initialize_turn_points(POWER, 30, 30, 100);
        L2.initialize_turn_points(POWER_MIDDLE, 0, 30, 100);
        L2.initialize_turn_points(SWING, -30, 30, 100);
        L2.initialize_turn_points(SWING_MIDDLE, 0, 50, 90);

        L3.initialize_turn_points(POWER, 30, 30, 100);
        L3.initialize_turn_points(POWER_MIDDLE, 0, 30, 100);
        L3.initialize_turn_points(SWING, -30, 30, 100);
        L3.initialize_turn_points(SWING_MIDDLE, 0, 50, 90);

        R1.initialize_turn_points(POWER, -30, 30, 100);
        R1.initialize_turn_points(POWER_MIDDLE, 0, 30, 100);
        R1.initialize_turn_points(SWING, 30, 30, 100);
        R1.initialize_turn_points(SWING_MIDDLE, 0, 50, 90);

        R2.initialize_turn_points(POWER, -30, 30, 100);
        R2.initialize_turn_points(POWER_MIDDLE, 0, 30, 100);
        R2.initialize_turn_points(SWING, 30, 30, 100);
        R2.initialize_turn_points(SWING_MIDDLE, 0, 50, 90);

        R3.initialize_turn_points(POWER, -30, 30, 100);
        R3.initialize_turn_points(POWER_MIDDLE, 0, 30, 100);
        R3.initialize_turn_points(SWING, 30, 30, 100);
        R3.initialize_turn_points(SWING_MIDDLE, 0, 50, 90);
    }

    void turn(bool is_right) {
        int delay_time = 100;
        L1.turn_to_power_middle();  // power middle all
        L2.turn_to_power_middle();
        L3.turn_to_power_middle();
        R1.turn_to_power_middle();
        R2.turn_to_power_middle();
        R3.turn_to_power_middle();
        delay(delay_time);

        L1.turn_to_power_middle();  // raise turning triangle
        L2.turn_to_swing_middle();
        L3.turn_to_power_middle();
        R1.turn_to_swing_middle();
        R2.turn_to_power_middle();
        R3.turn_to_swing_middle();
        delay(delay_time);

        L1.turn_to_power_middle();  // move turning triangle to offset
        L3.turn_to_power_middle();
        R2.turn_to_power_middle();

        if (is_right) {
            // right turn
            L2.turn_to_power_stance();
            R1.turn_to_swing_stance();
            R3.turn_to_swing_stance();
        } else {
            // left turn
            L2.turn_to_swing_stance();
            R1.turn_to_power_stance();
            R3.turn_to_power_stance();
        }
        delay(delay_time);

        L1.turn_to_swing_middle();  // raise support triangle
        //L2.turn_to_power_stance();
        L3.turn_to_swing_middle();
        //R1.turn_to_swing_stance();
        R2.turn_to_swing_middle();
        //R3.turn_to_swing_stance();
        delay(delay_time);

        L1.turn_to_swing_middle();  // move turning triangle to power middle,
                                    // rotating body
        L2.turn_to_power_middle();
        L3.turn_to_swing_middle();
        R1.turn_to_power_middle();
        R2.turn_to_swing_middle();
        R3.turn_to_power_middle();
        delay(delay_time);

        L1.turn_to_power_middle();  // back to middle all
        L2.turn_to_power_middle();
        L3.turn_to_power_middle();
        R1.turn_to_power_middle();
        R2.turn_to_power_middle();
        R3.turn_to_power_middle();
        delay(delay_time);
    }

    void update(int time) {
        /*
        if (current_time == new_time) {
            // change speed of cycle
            //this->set_new_speeds(new_time);
        } else {
            // continue cycle


        }*/

        L1.leg_cycle_straight(time);
        L2.leg_cycle_straight(time);
        L3.leg_cycle_straight(time);
        R1.leg_cycle_straight(time);
        R2.leg_cycle_straight(time);
        R3.leg_cycle_straight(time);
    }

    void manual_move(int coxa_angle, int femur_angle, int tibia_angle) {
        R1.move_leg_from_center(coxa_angle, femur_angle, tibia_angle);
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
