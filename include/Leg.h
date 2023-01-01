#include <PWMServo.h>

enum Joint { COXA, FEMUR, TIBIA } typedef Joints;
enum Direction { LEFT, RIGHT } typedef Side;

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
    int current_angle[3];
    int start_angle_offset[3];
    bool move_flag[3];
    unsigned long straight_line_start_time;
    

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

    /* move servo joint to angle*/
    void move_joint(Joints joint, int angle) {
        switch (joint) {
            case COXA:
                this->coxa.write(angle);
                break;
            case FEMUR:
                this->femur.write(angle);
                break;
            case TIBIA:
                this->tibia.write(angle);
                break;
        }
    }

    void move_from_center(Joints joint, int angle) {
        this->move_joint(joint, this->get_center(joint) - angle);
    }

    /* move all joints in leg to the idle holding position*/
    void idle_leg() {
        //this->move_to_angle(COXA, 0);
        this->move_to_angle(FEMUR, Leg::idle_femur_offset);
        this->move_to_angle(TIBIA, Leg::idle_tibia_offset);
    }

    /* point out all the joints out in a straight line in a leg */
    void center_leg() {
        center_joint(COXA);
        center_joint(FEMUR);
        center_joint(TIBIA);
    }

    /* center a single joint on a leg*/
    void center_joint(Joints joint) {
        this->move_joint(joint, this->get_center(joint));
    }

    void move_to_angle(Joints joint, int angle_offset){
        this->start_time[joint] = millis();
        this->previous_target_angle[joint] = this->target_angle[joint];
        this->target_angle[joint] = angle_offset;
        this->move_flag[joint] = true;
    }

    /* update a single joint on a leg */
    void update_leg_joint(Joints joint){
        this->progress_time[joint] = millis() - this->start_time[joint];
        if(this->progress_time[joint] <= this->moving_time[joint] && this->move_flag[joint]){
            long new_angle = map(this->progress_time[joint], 0, this->moving_time[TIBIA], this->previous_target_angle[joint], this->target_angle[joint]);
            this->move_from_center(joint, new_angle);
            //Serial.println((String) "servo read: " + L3.tibia.read());
            //Serial.println((String) "moving to: " + new_angle);
            //Serial.println((String) "old_t: " + this->previous_target_angle[TIBIA] + ", new_t: " + this->target_angle[TIBIA] + ", angle offset: " + new_angle + ", center: " + this->get_center(TIBIA));
        }else{
            this->move_flag[joint] = false;
        }
    }


    void set_leg_moving_time(unsigned long new_moving_time){
        this->moving_time[COXA] = new_moving_time;
        this->moving_time[FEMUR] = new_moving_time;
        this->moving_time[TIBIA] = new_moving_time;
    }
};
