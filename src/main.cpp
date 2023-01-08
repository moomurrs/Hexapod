#include <Arduino.h>
#include "Hexapod.h"

// forward declaration
bool parse_three_input();


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

int speed_input = 3000;
bool continue_loop = true;

void loop() {

    /*
    speed = get controller input
    hexapod.update(speed, direction)
    */

    
    if(continue_loop){
        hexapod.update(speed_input, false);
    }
    

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

        
        int serial_input = Serial.parseInt();
        Serial.printf("Input: %d\n", serial_input);
        if(serial_input == 0){
            continue_loop = false;
        }else{
            speed_input = serial_input;
        }

        Serial.clear();
        delay(20);

        // Serial.println((String) "progress: " + L3.progress_time + ", target:
        // " + L3.target_angle);
    }
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

