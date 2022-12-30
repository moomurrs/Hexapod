#include <Arduino.h>
#include "Hexapod.h"

/* forward declarations */
void center_all_legs();
void move_all_femur(int angle);
void move_all_tibia(int angle);
void idle_all_legs();
void flower_all_legs();
bool parse_input();
void power_stance();
void swing_stance();

Hexapod hexapod{};


int input[3];
int range[3][2] = {{-30, 30},     // coxa
                   {-60, 60},     // femur
                   {-140, 140}};  // tibia



void setup() {
    Serial.begin(115200);
    Serial.setTimeout(100);

    hexapod.set_calibration();

    /*
    R1.center_joint(COXA);
    R2.center_joint(COXA);
    R3.center_joint(COXA);

    L1.center_joint(COXA);
    L2.center_joint(COXA);
    L3.center_joint(COXA);
    */

    // idle_all_legs();
    /*
    center_all_legs();
    Serial.println("Centered legs");
    delay(5000);
    flower_all_legs();
    Serial.println("flowered legs");
    delay(500);
    idle_all_legs();
    Serial.println("idle legs");
    delay(100);*/
    //power_stance();
}

bool flag = true;
unsigned long previous_temp = millis();
unsigned long midpoint_previous = millis();

void loop() {
    
    
    //L3.update_leg();
    
    unsigned long elapsed_temp = millis() - previous_temp;
    unsigned long midpoint_elapsed_temp = millis() - midpoint_previous;
    //check if full cycle is complete
    if(elapsed_temp > 1200){
        previous_temp = millis();
        if(flag){
            swing_stance();
            flag = false;
        }else{
            power_stance();
            flag = true;
        }
    }else{
        //full 
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
        }

        Serial.clear();
        delay(20);

        // Serial.println((String) "progress: " + L3.progress_time + ", target:
        // " + L3.target_angle);
    }
}

void move_leg(){
    // assume hexapod is idle


}

void swing_stroke(){
    // move to swing stance

    // move to swing middle

    // move to power stance
}

void power_stroke(){
    // move to power stance

    // move to power middle

    // move to swing stance
}

void swing_stance(){
    //L3.move_to_angle(COXA, -20);
    //L3.move_to_angle(FEMUR, 10);
    //L3.move_to_angle(TIBIA, 65);
}

void power_stance(){
    //L3.move_to_angle(COXA, 25);
    //L3.move_to_angle(FEMUR, 45);
    //L3.move_to_angle(TIBIA, 135);
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

