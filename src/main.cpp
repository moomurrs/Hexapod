#include <Arduino.h>

#include "Controller.h"
#include "Hexapod.h"

// forward declaration
bool parse_three_input();
void initial_boot();

Hexapod hexapod{};
Controller controller;

int input[3];
int range[3][2] = {{-30, 30},     // coxa
                   {-60, 60},     // femur
                   {-140, 140}};  // tibia

void setup() {
    // raspiberryi pi serial
    Serial1.begin(2000000);

    // desktop serial
    Serial.begin(115200);
    Serial.setTimeout(100);

    // set center calibration points for joints
    hexapod.set_calibration();

    // set the initial target angles for each leg
    hexapod.initialize();

    hexapod.flower_all_legs();
    delay(1000);
}

bool initialized = false;
bool continue_loop = true;

void loop() {
    // see if raspberry sent information

    
    if (Serial1.available() > 0) {
        // read data until end of JSON
        String data = Serial1.readStringUntil('}');
        data.concat("}");
        controller.update_controller(data);
    }

    if (!initialized) {
        if(controller.get_symbol_button(X) == true){
            initial_boot();
        }
        
    } else {
        if (controller.get_dpad_button(UP)) {
            // move forward on press
            // Serial.println("up pressed");
            hexapod.move_forward(2000);
        } else if (controller.get_dpad_button(RIGHT)) {
            // turn right
            // Serial.println("right pressed");
            hexapod.turn(true);
        } else if (controller.get_dpad_button(LEFT)) {
            // turn left
            // Serial.println("left pressed");
            hexapod.turn(false);
        }
    }

    /*
    if(continue_loop){
        //hexapod.update(speed_input, true);
        hexapod.update(3000, true);
    }*/

    if (Serial.available() > 0) {
        // angle = Serial.parseInt();

        /*
        if (parse_three_input()) {
            Serial.println("\n\nInput received \n");
            Serial.println((String) "coxa: " + input[0]);
            Serial.println((String) "femur: " + input[1]);
            Serial.println((String) "tibia: " + input[2]);

            //hexapod.move_leg_to_angle(LEFT3, input[0], input[1], input[2]);
            hexapod.manual_move(input[0], input[1], input[2]);
        }*/

        /*
        int serial_input = Serial.parseInt();
        Serial.printf("Input: %d\n", serial_input);
        if (serial_input == 0) {
            continue_loop = false;
        } else {
            
        }*/

        Serial.clear();
        delay(20);

        // Serial.println((String) "progress: " + L3.progress_time + ", target:
        // " + L3.target_angle);
    }
}

void initial_boot() {
    hexapod.center_all_legs();
    delay(3000);

    hexapod.idle_all_legs();
    delay(1000);
    initialized = true;
}

bool parse_three_input() {
    for (int i = 0; i < 3; i++) {
        input[i] = atoi(Serial.readStringUntil(',').c_str());
        // Serial.println((String) "Echo: " + input);
    }

    if (input[0] >= range[0][0] && input[0] <= range[0][1]) {
        if (input[1] >= range[1][0] && input[1] <= range[1][1]) {
            if (input[2] >= range[2][0] && input[2] <= range[2][1]) {
                Serial.println((String) "Received: " + input[0] + ", " +
                               input[1] + ", " + input[2]);
                Serial.flush();
                return true;
            } else {
                Serial.println("Bad tibia");
            }
        } else {
            Serial.println("Bad femur");
        }
    } else {
        Serial.println("Bad coxa");
    }

    return false;
}
