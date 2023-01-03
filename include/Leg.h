#include <PWMServo.h>

enum Joint { COXA, FEMUR, TIBIA } typedef Joints;
enum Direction { LEFT, RIGHT } typedef Side;
enum Stances { POWER, POWER_MIDDLE, SWING, SWING_MIDDLE } typedef Stance;
int current_time = 1200;
class Leg {
   private:
    /* 3 servos per leg */
    PWMServo coxa;
    PWMServo femur;
    PWMServo tibia;

    /* center values */
    int coxa_center_point;
    int femur_center_point;
    int tibia_center_point;

    /* stance values, defined as offset from the center/edge*/

    Side side;

    /* move servo joint to angle*/
    void _move_joint(Joints joint, int angle) {
        switch (joint) {
            case COXA:
                this->coxa.write(angle);
                this->current_angle_offset[COXA] = angle;
                break;
            case FEMUR:
                this->femur.write(angle);
                this->current_angle_offset[FEMUR] = angle;
                break;
            case TIBIA:
                this->tibia.write(angle);
                this->current_angle_offset[TIBIA] = angle;
                break;
        }
    }


   public:
    /* Constructor*/
    Leg(int coxa_pin, int femur_pin, int tibia_pin, Side side) {
        coxa.attach(coxa_pin);
        femur.attach(femur_pin);
        tibia.attach(tibia_pin);
        this->side = side;
    }

    unsigned long start_time[3];
    unsigned long progress_time[3];
    unsigned long moving_time[3] = {300, 300, 300};
    int target_angle[3];
    int previous_target_angle[3];
    int current_angle_offset[3];
    int start_angle_offset[3];
    bool move_flag[3];
    Stance current_stance;

    const static int idle_femur_offset = 50;
    const static int idle_tibia_offset = 130;

    /* Set center values for the servos */
    void set_center(int coxa_center, int femur_center, int tibia_center) {
        this->coxa_center_point = coxa_center;
        this->femur_center_point = femur_center;
        this->tibia_center_point = tibia_center;
    }

    /* return the servo calibration point */
    int get_center(Joints joint) {
        int value;
        switch (joint) {
            case COXA:
                value = this->coxa_center_point;
                break;
            case FEMUR:
                value = this->femur_center_point;
                break;
            case TIBIA:
                value = this->tibia_center_point;
                break;
        }
        return value;
    }

    // move a joint with respect from its center
    void move_from_center(Joints joint, int angle) {
        this->_move_joint(joint, this->get_center(joint) - angle);
    }

    // move all joints in leg to the idle holding position
    void idle_leg() {
        // this->move_to_angle(COXA, 0);
        this->move_to_angle(FEMUR, Leg::idle_femur_offset);
        this->move_to_angle(TIBIA, Leg::idle_tibia_offset);
    }

    // point out all the joints out in a straight line in a leg
    void center_leg() {
        center_joint(COXA);
        center_joint(FEMUR);
        center_joint(TIBIA);
    }

    // center a single joint on a leg
    void center_joint(Joints joint) {
        this->_move_joint(joint, this->get_center(joint));
    }

    void move_to_angle(Joints joint, int angle_offset) {
        this->start_time[joint] = millis();
        this->previous_target_angle[joint] = this->target_angle[joint];
        this->target_angle[joint] = angle_offset;
        this->move_flag[joint] = true;
    }

    void set_initial_stance(Stance stance) {
        // current stance = stance
    }

    void leg_cycle_straight(int new_time) {
        if (current_time == new_time) {
            // change speed of cycle
            this->set_new_speeds(new_time);
        } else {
            // continue cycle

            /*

            if progress_time < moving_time && stance is power:
                leg.power_stance()
            else if progress_time < moving_time && stance is power_middle:
                leg.power_middle()
            else if progress_time < moving_time && stance is swing:
                leg.swing_stance()
            else if progress_time < moving_time && stance is swing_middle:
                leg.swing_middle()
            else
                // progress time expired, move to next stance
                current_stance += 1 % 4 

                // restart movement timings
                progress_time = 0       
                start_time = 0

            leg.update // move the leg towards the target

            */
        }

        /*

        // modify the start/progress times & old/new angles to re-calculates map
        this->recalculate_cycle_time(time);

        // write servo
        this->update_

        */
    }

    // speeds up or slows down movement
    void set_new_speeds(int time) {
        float interval = (float)time / 4.0;

        // change time to reach target
        this->moving_time[COXA] = interval;
        this->moving_time[FEMUR] = interval;
        this->moving_time[TIBIA] = interval;

        // save current progress
        this->start_angle_offset[COXA] = this->current_angle_offset[COXA];
        this->start_angle_offset[FEMUR] = this->current_angle_offset[FEMUR];
        this->start_angle_offset[TIBIA] = this->current_angle_offset[TIBIA];

        // restart timer
        this->start_time[COXA] = 0;
        this->start_time[FEMUR] = 0;
        this->start_time[TIBIA] = 0;
    }

    /* update a single leg, to be called every loop */
    void update_leg() {
        this->update_leg_joint(COXA);
        this->update_leg_joint(FEMUR);
        this->update_leg_joint(TIBIA);
    }

    /* update a single joint on a leg */
    void update_leg_joint(Joints joint) {
        this->progress_time[joint] = millis() - this->start_time[joint];
        if (this->progress_time[joint] <= this->moving_time[joint] &&
            this->move_flag[joint]) {
            long new_angle = map(
                this->progress_time[joint], 0, this->moving_time[joint],
                this->previous_target_angle[joint], this->target_angle[joint]);
            this->move_from_center(joint, new_angle);
            // Serial.println((String) "servo read: " + L3.tibia.read());
            // Serial.println((String) "moving to: " + new_angle);
            // Serial.println((String) "old_t: " +
            // this->previous_target_angle[TIBIA] + ", new_t: " +
            // this->target_angle[TIBIA] + ", angle offset: " + new_angle + ",
            // center: " + this->get_center(TIBIA));
        } else {
            this->move_flag[joint] = false;
        }
    }


    void power_stance() {
        // L3.move_to_angle(COXA, 25);
        // L3.move_to_angle(FEMUR, 45);
        // L3.move_to_angle(TIBIA, 135);

        //move_leg_to_angle(25, 45, 135);
        set_new_targets(25, 45, 135);
    }

    void power_middle() {  
        //move_leg_to_angle(-2, 40, 110);
        set_new_targets(-2, 40, 110);
    }

    void swing_stance() {
        // L3.move_to_angle(COXA, -20);
        // L3.move_to_angle(FEMUR, 10);
        // L3.move_to_angle(TIBIA, 65);
        // move_leg_to_angle(-20, 10, 65);
        set_new_targets(-20, 10, 65);
    }

    void swing_middle() { 
        // move_leg_to_angle(15, 60, 100); 
        set_new_targets(15, 60, 100); 
    }

    void move_leg_to_angle(int coxa_angle, int femur_angle, int tibia_angle) {
        this->move_to_angle(TIBIA, tibia_angle);
        this->move_to_angle(FEMUR, femur_angle);
        this->move_to_angle(COXA, coxa_angle);
    }

    void set_new_targets(int coxa_angle, int femur_angle, int tibia_angle){
        this->target_angle[COXA] = coxa_angle;
        this->target_angle[FEMUR] = femur_angle;
        this->target_angle[TIBIA] = tibia_angle;
    }

};

