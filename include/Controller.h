#include <ArduinoJson.h>
#include <string>

enum AXIS { X_AXIS, Y_AXIS };

enum DPAD {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum BUTTONS { TRIANGLE, CIRCLE, X, SQUARE };

enum BUMPERS { L1, R1 };

enum TRIGGERS { L2, R2 };

enum STICK_BUTTONS { L3, R3 };

enum AUX_BUTTONS { PS_HOME, OPTIONS, SHARE };

class Controller {
   private:
    /* data */

    // symbol buttons
    bool symbol_buttons[4] = {false, false, false,
                              false};  // triangle, circle, x, square

    // back bumpers
    bool bumper_is_pressed[2] = {false, false};  // l1, r1

    // back triggers
    int trigger_value[2] = {-32767, -32767};     // r2, l2 values
    bool trigger_is_released[2] = {true, true};  // is r2, l2 released

    // stick press
    bool l3_is_pressed = false;
    bool r3_is_pressed = false;

    // directional dpad
    bool dpad_buttons[4] = {false, false, false,
                            false};  // up, down, left, right

    // PS home, options, share buttons
    bool auxiliary_buttons[3] = {false, false,
                                 false};  // PS home, options, share buttons
    // bool option_is_pressed = false;
    // bool share_is_pressed = false;
    // bool ps_is_pressed = false;

    // joystick axis rest
    bool r3_is_rest[2] = {false, false};  // x, y values right stick
    bool l3_is_rest[2] = {false, false};  // x, y values left stick

    // joystick axis values
    int r3_axis[2] = {0, 0};  // x, y values
    int l3_axis[2] = {0, 0};  // x, y values

   public:
    // Update a joystick value
    void update_controller(String data) {
        DynamicJsonDocument doc(100);
        deserializeJson(doc, data);
        JsonObject obj = doc.as<JsonObject>();

        JsonObject::iterator it = obj.begin();
        const char* key = it->key().c_str();

        /* Left Stick */
        if (!strncmp(key, "l3_x_axis", 9)) {
            String value = obj["l3_x_axis"];
            l3_axis[X_AXIS] = atoi(value.c_str());

            // left x axis NOT at rest
            l3_is_rest[X_AXIS] = false;

            // Serial.printf("l3 x value: %d\n", l3_x_y_axis[0]);

        } else if (!strncmp(key, "l3_x_rest", 9)) {
            l3_is_rest[X_AXIS] = true;

            // zero 0 left x axis
            l3_axis[X_AXIS] = 0;

            // Serial.printf("l3 x REST\n");
        } else if (!strncmp(key, "l3_y_axis", 9)) {
            String value = obj["l3_y_axis"];
            l3_axis[Y_AXIS] = atoi(value.c_str());

            // left y axis NOT at rest
            l3_is_rest[Y_AXIS] = false;

            // Serial.printf("l3 y value: %d\n", l3_x_y_axis[0]);

        } else if (!strncmp(key, "l3_y_rest", 9)) {
            l3_is_rest[Y_AXIS] = true;

            // zero 0 left y axis
            l3_axis[Y_AXIS] = 0;

            // Serial.printf("l3 y REST\n");

        } else if (!strncmp(key, "l3_pressed", 10)) {
            l3_is_pressed = true;
            // Serial.println("L3 pressed\n");
        } else if (!strncmp(key, "l3_released", 11)) {
            l3_is_pressed = false;
            // Serial.println("L3 released\n");
        }

        /* Right Stick */
        else if (!strncmp(key, "r3_x_axis", 9)) {
            String value = obj["r3_x_axis"];
            r3_axis[X_AXIS] = atoi(value.c_str());

            // right x axis NOT at rest
            r3_is_rest[X_AXIS] = false;

            // Serial.printf("r3 x value: %d\n", r3_x_y_axis[0]);

        } else if (!strncmp(key, "r3_x_rest", 9)) {
            r3_is_rest[X_AXIS] = true;

            // zero 0 right x axis
            r3_axis[X_AXIS] = 0;

            // Serial.printf("r3 x REST\n");
        } else if (!strncmp(key, "r3_y_axis", 9)) {
            String value = obj["r3_y_axis"];
            r3_axis[Y_AXIS] = atoi(value.c_str());

            // right y axis NOT at rest
            r3_is_rest[Y_AXIS] = false;

            // Serial.printf("r3 y value: %d\n", r3_x_y_axis[0]);

        } else if (!strncmp(key, "r3_y_rest", 10)) {
            r3_is_rest[Y_AXIS] = true;

            // zero 0 right y axis
            r3_axis[Y_AXIS] = 0;

            // Serial.printf("r3 y REST\n");

        } else if (!strncmp(key, "r3_pressed", 10)) {
            r3_is_pressed = true;
            // Serial.println("R3 pressed\n");
        } else if (!strncmp(key, "r3_released", 11)) {
            r3_is_pressed = false;
            // Serial.println("R3 released\n");
        }
        // Hat Arrows
        else if (!strncmp(key, "up_arrow_pressed", 16)) {
            dpad_buttons[UP] = true;
            // Serial.println("up pressed\n");

        } else if (!strncmp(key, "down_arrow_pressed", 18)) {
            dpad_buttons[DOWN] = true;
            // Serial.println("down pressed\n");

        } else if (!strncmp(key, "up_down_arrow_released", 22)) {
            dpad_buttons[UP] = false;
            dpad_buttons[DOWN] = false;
            // Serial.println("up/down released\n");
        } else if (!strncmp(key, "left_arrow_pressed", 18)) {
            dpad_buttons[LEFT] = true;
            // Serial.println("left pressed\n");

        } else if (!strncmp(key, "right_arrow_pressed", 19)) {
            dpad_buttons[RIGHT] = true;
            // Serial.println("right pressed\n");

        } else if (!strncmp(key, "left_right_arrow_released", 25)) {
            dpad_buttons[LEFT] = false;
            dpad_buttons[RIGHT] = false;
            // Serial.println("left/right released\n");
        }
        // Buttons
        else if (!strncmp(key, "triangle_pressed", 16)) {
            symbol_buttons[TRIANGLE] = true;
            // Serial.println("Tri pressed\n");

        } else if (!strncmp(key, "triangle_released", 17)) {
            symbol_buttons[TRIANGLE] = false;
            // Serial.println("Tri released\n");

        } else if (!strncmp(key, "x_pressed", 9)) {
            symbol_buttons[X] = true;
            // Serial.println("x pressed\n");

        } else if (!strncmp(key, "x_released", 10)) {
            symbol_buttons[X] = false;
            // Serial.println("x released\n");

        } else if (!strncmp(key, "circle_pressed", 14)) {
            symbol_buttons[CIRCLE] = true;
            // Serial.println("circle pressed\n");

        } else if (!strncmp(key, "circle_released", 15)) {
            symbol_buttons[CIRCLE] = false;
            // Serial.println("circle released\n");

        } else if (!strncmp(key, "square_pressed", 14)) {
            symbol_buttons[SQUARE] = true;
            // Serial.println("square pressed\n");

        } else if (!strncmp(key, "square_released", 15)) {
            symbol_buttons[SQUARE] = false;
            // Serial.println("square released\n");
        }
        // Bumpers
        else if (!strncmp(key, "r1_pressed", 10)) {
            bumper_is_pressed[R1] = true;
            // Serial.printf("R1 pressed\n");
        } else if (!strncmp(key, "r1_released", 11)) {
            bumper_is_pressed[R1] = false;
            // Serial.printf("R1 released\n");
        } else if (!strncmp(key, "l1_pressed", 10)) {
            bumper_is_pressed[L1] = true;
            // Serial.printf("L1 pressed\n");
        } else if (!strncmp(key, "l1_released", 11)) {
            bumper_is_pressed[L1] = false;
            // Serial.printf("L1 released\n");
        }
        // Triggers
        else if (!strncmp(key, "l2_pressed", 10)) {
            trigger_is_released[L2] = false;
            String value = obj["l2_pressed"];
            trigger_value[L2] = atoi(value.c_str());
            // Serial.printf("L2: %d\n", trigger_value[L2]);
        } else if (!strncmp(key, "l2_released", 11)) {
            trigger_is_released[L2] = true;
            trigger_value[L2] = -32767;
            // Serial.printf("L2: %d\n", trigger_value[L2]);
            // Serial.printf("L2 released\n");
        } else if (!strncmp(key, "r2_pressed", 10)) {
            trigger_is_released[R2] = false;
            String value = obj["r2_pressed"];
            trigger_value[R2] = atoi(value.c_str());
            // Serial.printf("R2: %d\n", trigger_value[R2]);
        } else if (!strncmp(key, "r2_released", 11)) {
            trigger_is_released[R2] = true;
            trigger_value[R2] = -32767;
            // Serial.printf("R2: %d\n", trigger_value[R2]);
            // Serial.printf("R2 released\n");
        }
        // option, ps, share buttons
        else if (!strncmp(key, "ps_pressed", 10)) {
            auxiliary_buttons[PS_HOME] = true;
            // Serial.printf("ps pressed\n");
        } else if (!strncmp(key, "ps_released", 11)) {
            auxiliary_buttons[PS_HOME] = false;
            // Serial.printf("ps released\n");
        } else if (!strncmp(key, "share_pressed", 13)) {
            auxiliary_buttons[SHARE] = true;
            // Serial.printf("share pressed\n");
        } else if (!strncmp(key, "share_released", 14)) {
            auxiliary_buttons[SHARE] = false;
            // Serial.printf("share released\n");
        } else if (!strncmp(key, "option_pressed", 14)) {
            auxiliary_buttons[OPTIONS] = true;
            // Serial.printf("option pressed\n");
        } else if (!strncmp(key, "option_released", 15)) {
            auxiliary_buttons[OPTIONS] = false;
            // Serial.printf("option released\n");
        } else {
            Serial.printf("[ERROR]: key %s\n", key);
        }
    }

    // get dpad button press
    bool get_dpad_button(DPAD dpad) {
        bool val;
        switch (dpad) {
            case UP:
                val = dpad_buttons[UP];
                break;
            case DOWN:
                val = dpad_buttons[DOWN];
                break;
            case LEFT:
                val = dpad_buttons[LEFT];
                break;
            case RIGHT:
                val = dpad_buttons[RIGHT];
                break;
        }
        return val;
    }

    // get analog stick press
    bool get_stick_press(STICK_BUTTONS button) {
        bool val;
        switch (button) {
            case L3:
                val = l3_is_pressed;
                break;

            case R3:
                val = r3_is_pressed;
                break;
        }
        return val;
    }

    // get l1, r1 bumper value
    bool get_bumper(BUMPERS bumper) {
        bool val;
        switch (bumper) {
            case L1:
                val = bumper_is_pressed[L1];
                break;
            case R1:
                val = bumper_is_pressed[R1];
                break;
        }
        return val;
    }

    // get symbol triangle, circle, x, square button presses
    bool get_symbol_button(BUTTONS button) {
        bool val;
        switch (button) {
            case TRIANGLE:
                val = symbol_buttons[TRIANGLE];
                break;
            case CIRCLE:
                val = symbol_buttons[CIRCLE];
                break;
            case X:
                val = symbol_buttons[X];
                break;
            case SQUARE:
                val = symbol_buttons[SQUARE];
                break;
        }
        return val;
    }

    // return the specified axis on the left joystick
    int get_l3_axis(AXIS axis) {
        int l3_value;
        switch (axis) {
            case X_AXIS:
                l3_value = l3_axis[X_AXIS];
                break;

            case Y_AXIS:
                l3_value = l3_axis[Y_AXIS];
                break;
        }
        return l3_value;
    }

    // return the specified axis on the right joystick
    int get_r3_axis(AXIS axis) {
        int r3_value;
        switch (axis) {
            case X_AXIS:
                r3_value = r3_axis[X_AXIS];
                break;

            case Y_AXIS:
                r3_value = r3_axis[Y_AXIS];
                break;
        }
        return r3_value;
    }

    // return r2,l2 trigger values
    int get_trigger(TRIGGERS trigger) {
        int trigger_val;
        switch (trigger) {
            case R2:
                trigger_val = trigger_value[R2];
                break;

            case L2:
                trigger_val = trigger_value[L2];
                break;
        }
        return trigger_val;
    }

    /* Print functions for reading controller values and events */
    // print all controller stick axis
    void view_axis() {
        Serial.printf("LEFT: x %6d, y %6d | ", get_l3_axis(X_AXIS),
                      get_l3_axis(Y_AXIS));
        Serial.printf("RIGHT: x %6d, y %6d\n", get_r3_axis(X_AXIS),
                      get_r3_axis(Y_AXIS));
    }

    // print dpad presses
    void view_dpad() {
        Serial.printf("\n    %d\n%d       %d\n    %d\n", get_dpad_button(UP),
                      get_dpad_button(LEFT), get_dpad_button(RIGHT),
                      get_dpad_button(DOWN));
    }

    // print square, tri, x, circle presses
    void view_buttons() {
        Serial.printf("\n    %d\n%d       %d\n    %d\n",
                      get_symbol_button(TRIANGLE), get_symbol_button(SQUARE),
                      get_symbol_button(CIRCLE), get_symbol_button(X));
    }

    // print bumper presses and trigger values
    void view_bumpers_triggers() {
        Serial.printf("\nL1:    %d   | R1:    %d\n", get_bumper(L1),
                      get_bumper(R1));
        Serial.printf("L2: %6d | R2: %6d\n", get_trigger(L2), get_trigger(R2));
    }

    // print PS home, other, and share buttons
    void view_ps_other_share_buttons() {
        Serial.printf("\nPS Home: %d\n", auxiliary_buttons[PS_HOME]);
        Serial.printf("Options: %d\n", auxiliary_buttons[OPTIONS]);
        Serial.printf("Share: %d\n", auxiliary_buttons[SHARE]);
    }

    // print all values and events on the controller
    void view_all_values() {
        Serial.println("");
        view_bumpers_triggers();

        Serial.printf(
            "\n    %d               %d\n%d       %d       %d       %d\n    %d  "
            "             %d\n",
            get_dpad_button(UP), get_symbol_button(TRIANGLE),
            get_dpad_button(LEFT), get_dpad_button(RIGHT),
            get_symbol_button(SQUARE), get_symbol_button(CIRCLE),
            get_dpad_button(DOWN), get_symbol_button(X));

        view_axis();
        Serial.println("");
    }
};
