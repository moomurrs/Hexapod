#include <Arduino.h>
#include "Hexapod.h"

// forward declaration
bool parse_input();


Hexapod hexapod{};

int input[3];
int range[3][2] = {{-30, 30},     // coxa
                   {-60, 60},     // femur
                   {-140, 140}};  // tibia

enum Tests{Herp, Derp} typedef Test;

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(100);

    hexapod.set_calibration();

    
    // set the initial target angles for each leg
    hexapod.initialize(); 


}

bool kill = false;

void loop() {

    /*
    speed = get controller input
    hexapod.update(speed, direction)
    */

    if(!kill){
        hexapod.update(2000, false);
    }


    if (Serial.available() > 0) {
        // angle = Serial.parseInt();

        if (parse_input()) {
            Serial.println("\n\nInput received \n");
            Serial.println((String) "coxa: " + input[0]);
            Serial.println((String) "femur: " + input[1]);
            Serial.println((String) "tibia: " + input[2]);

            //L3.move_to_angle(COXA, input[0]);
            //L3.move_to_angle(FEMUR, input[1]);
            //L3.move_to_angle(TIBIA, input[2]);
            //hexapod.move_leg_to_angle(LEFT3, input[0], input[1], input[2]);
            kill = input[0];
        }

        Serial.clear();
        delay(20);

        // Serial.println((String) "progress: " + L3.progress_time + ", target:
        // " + L3.target_angle);
    }
}



bool parse_input() {
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

