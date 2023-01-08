#include <PWMServo.h>

enum Joint { COXA, FEMUR, TIBIA };
enum Stance { POWER, POWER_MIDDLE, SWING, SWING_MIDDLE };

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

    /* move servo joint to angle, using PWMServo library*/
    void _move_joint(Joint joint, int angle) {
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

   public:
    /* Constructor*/
    Leg(int coxa_pin, int femur_pin, int tibia_pin) {
        coxa.attach(coxa_pin);
        femur.attach(femur_pin);
        tibia.attach(tibia_pin);
    }

    unsigned long start_time[3];
    unsigned long progress_time[3];
    unsigned long moving_time[3] = {500, 500, 500};
    int target_angle[3];
    int previous_target_angle[3];
    int current_angle_offset[3];
    int start_angle_offset[3];
    Stance current_stance;
    int current_leg_cycle_time = 2000;

    int power_stance_straight[3];
    int power_middle_straight[3];
    int swing_stance_straight[3];
    int swing_middle_straight[3];

    /* Set center values for the servos */
    void set_center(int coxa_center, int femur_center, int tibia_center) {
        this->coxa_center_point = coxa_center;
        this->femur_center_point = femur_center;
        this->tibia_center_point = tibia_center;
    }

    /* return the servo calibration point */
    int get_center(Joint joint) {
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

    // move a joint with respect from its center, update previous and current
    // angle offsets
    void move_joint_from_center(Joint joint, int angle) {
        this->current_angle_offset[joint] = angle;
        this->_move_joint(joint, this->get_center(joint) - angle);
    }

    void move_leg_from_center(int coxa_angle, int femur_angle,
                              int tibia_angle) {
        move_joint_from_center(COXA, coxa_angle);
        move_joint_from_center(FEMUR, femur_angle);
        move_joint_from_center(TIBIA, tibia_angle);
    }

    // point out all the joints out in a straight line in a leg
    void center_leg() {
        center_joint(COXA);
        center_joint(FEMUR);
        center_joint(TIBIA);
    }

    // center a single joint on a leg
    void center_joint(Joint joint) {
        this->_move_joint(joint, this->get_center(joint));
    }

    void set_initial_stance(Stance stance) {
        // current stance = stance
        this->set_target_stance(stance);
        this->current_stance = stance;
    }

    void leg_cycle_straight(int new_time) {
        // set new targets and previous targets
        if (new_time != current_leg_cycle_time) {
            set_new_speeds(new_time);
        } else if (progress_time[TIBIA] >= moving_time[TIBIA]) {
            Serial.printf("Changing stance: %d", this->current_stance);

            // progress time expired, move to next stance
            this->current_stance =
                static_cast<Stance>((this->current_stance + 1) % 4);

            // restart movement timings
            this->progress_time[COXA] = 0;
            this->progress_time[FEMUR] = 0;
            this->progress_time[TIBIA] = 0;

            this->set_start_time(millis());

            this->set_target_stance(current_stance);

            Serial.printf("New stance: %d", this->current_stance);
        }

        // keep moving towards target
        this->interpolate_movement(TIBIA);
        this->interpolate_movement(FEMUR);
        this->interpolate_movement(COXA);

        // Serial.printf("elapsed: %lu, new_angle: %d\n", progress_time[0],
        // target_angle[0]);
    }

    void initialize_leg_timer() {
        unsigned long start_time = millis();
        this->set_start_time(start_time);
    }

    void set_start_time(unsigned long time) {
        this->start_time[COXA] = time;
        this->start_time[FEMUR] = time;
        this->start_time[TIBIA] = time;
    }

    // move joint according to elapsed time through interpolation
    void interpolate_movement(Joint joint) {
        //
        this->progress_time[joint] = millis() - this->start_time[joint];
        long new_angle =
            map(this->progress_time[joint], 0, this->moving_time[joint],
                this->previous_target_angle[joint], this->target_angle[joint]);
        this->move_joint_from_center(joint, new_angle);
    }

    // speeds up or slows down movement
    void set_new_speeds(int time) {
        float interval = (float)time / 4.0;

        // change time to reach target
        this->moving_time[COXA] = interval;
        this->moving_time[FEMUR] = interval;
        this->moving_time[TIBIA] = interval;

        this->progress_time[COXA] = 0;
        this->progress_time[FEMUR] = 0;
        this->progress_time[TIBIA] = 0;

        // save current progress
        this->start_angle_offset[COXA] = this->current_angle_offset[COXA];
        this->start_angle_offset[FEMUR] = this->current_angle_offset[FEMUR];
        this->start_angle_offset[TIBIA] = this->current_angle_offset[TIBIA];

        this->previous_target_angle[COXA] = this->current_angle_offset[COXA];
        this->previous_target_angle[FEMUR] = this->current_angle_offset[FEMUR];
        this->previous_target_angle[TIBIA] = this->current_angle_offset[TIBIA];

        current_leg_cycle_time = time;

        // restart timer
        // this->start_time = 0;
    }

    void set_target_stance(Stance stance) {
        switch (stance) {
            case POWER:
                set_new_targets(this->power_stance_straight[COXA],
                                this->power_stance_straight[FEMUR],
                                this->power_stance_straight[TIBIA]);
                set_new_previous_targets(this->swing_middle_straight[COXA],
                                         this->swing_middle_straight[FEMUR],
                                         this->swing_middle_straight[TIBIA]);
                break;
            case POWER_MIDDLE:
                set_new_targets(this->power_middle_straight[COXA],
                                this->power_middle_straight[FEMUR],
                                this->power_middle_straight[TIBIA]);
                set_new_previous_targets(this->power_stance_straight[COXA],
                                         this->power_stance_straight[FEMUR],
                                         this->power_stance_straight[TIBIA]);
                break;
            case SWING:
                set_new_targets(this->swing_stance_straight[COXA],
                                this->swing_stance_straight[FEMUR],
                                this->swing_stance_straight[TIBIA]);
                set_new_previous_targets(this->power_middle_straight[COXA],
                                         this->power_middle_straight[FEMUR],
                                         this->power_middle_straight[TIBIA]);
                break;
            case SWING_MIDDLE:
                set_new_targets(this->swing_middle_straight[COXA],
                                this->swing_middle_straight[FEMUR],
                                this->swing_middle_straight[TIBIA]);
                set_new_previous_targets(this->swing_stance_straight[COXA],
                                         this->swing_stance_straight[FEMUR],
                                         this->swing_stance_straight[TIBIA]);
                break;
        }
    }

    void set_new_targets(int coxa_angle, int femur_angle, int tibia_angle) {
        this->target_angle[COXA] = coxa_angle;
        this->target_angle[FEMUR] = femur_angle;
        this->target_angle[TIBIA] = tibia_angle;
    }

    void set_new_previous_targets(int coxa_angle, int femur_angle,
                                  int tibia_angle) {
        this->previous_target_angle[COXA] = coxa_angle;
        this->previous_target_angle[FEMUR] = femur_angle;
        this->previous_target_angle[TIBIA] = tibia_angle;
    }

    void initialize_stance_points(Stance stance, int coxa, int femur,
                                  int tibia) {
        switch (stance) {
            case POWER:
                this->power_stance_straight[COXA] = coxa;
                this->power_stance_straight[FEMUR] = femur;
                this->power_stance_straight[TIBIA] = tibia;
                // Serial.printf("power target: %d, %d, %d\n",
                // power_stance_straight[COXA],power_stance_straight[FEMUR],power_stance_straight[TIBIA]);
                break;
            case POWER_MIDDLE:
                this->power_middle_straight[COXA] = coxa;
                this->power_middle_straight[FEMUR] = femur;
                this->power_middle_straight[TIBIA] = tibia;
                // Serial.printf("power middle target: %d, %d, %d\n",
                // power_stance_straight[COXA],power_stance_straight[FEMUR],power_stance_straight[TIBIA]);
                break;
            case SWING:
                this->swing_stance_straight[COXA] = coxa;
                this->swing_stance_straight[FEMUR] = femur;
                this->swing_stance_straight[TIBIA] = tibia;
                // Serial.printf("swing target: %d, %d, %d\n",
                // power_stance_straight[COXA],power_stance_straight[FEMUR],power_stance_straight[TIBIA]);
                break;
            case SWING_MIDDLE:
                this->swing_middle_straight[COXA] = coxa;
                this->swing_middle_straight[FEMUR] = femur;
                this->swing_middle_straight[TIBIA] = tibia;
                // Serial.printf("swing middle target: %d, %d, %d\n",
                // power_stance_straight[COXA],power_stance_straight[FEMUR],power_stance_straight[TIBIA]);
                break;
        }
    }
};
