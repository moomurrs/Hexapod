#include <Bluepad32.h>
#include <Wire.h>
#include <stdio.h>

#define I2CADDR 0x55

// int is 2 bytes
unsigned int rx = 0;
unsigned int ry = 0;
unsigned int lx = 0;
unsigned int ly = 0;
unsigned int buttons = 0;
unsigned char dpad = 0;
unsigned char misc = 0;

GamepadPtr myGamepad;

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
    if (myGamepad == nullptr) {
        Serial.printf("CALLBACK: Gamepad is connected\n");
        // Additionally, you can get certain gamepad properties like:
        // Model, VID, PID, BTAddr, flags, etc.
        GamepadProperties properties = gp->getProperties();
        Serial.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n",
                      gp->getModelName().c_str(), properties.vendor_id,
                      properties.product_id);
        myGamepad = gp;
        myGamepad->setColorLED(0, 255, 0);
    }
}

void onDisconnectedGamepad(GamepadPtr gp) {
    bool foundGamepad = false;

    if (myGamepad == gp) {
        Serial.printf("CALLBACK: Gamepad is disconnected\n");
        myGamepad = nullptr;
        foundGamepad = true;
    }

    if (!foundGamepad) {
        Serial.println(
            "CALLBACK: Gamepad disconnected, but not found in myGamepads");
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Wire.setClock(100000);
    Wire.begin(I2CADDR);
    Wire.onRequest(requestEvent);

    Serial.begin(115200);
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t *addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1],
                  addr[2], addr[3], addr[4], addr[5]);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();
}

// Arduino loop function. Runs in CPU 1
void loop() {
    // This call fetches all the gamepad info from the NINA (ESP32) module.
    // Just call this function in your main loop.
    // The gamepads pointer (the ones received in the callbacks) gets updated
    // automatically.
    BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.

    if (myGamepad && myGamepad->isConnected()) {
        // There are different ways to query whether a button is pressed.
        // By query each button individually:
        //  a(), b(), x(), y(), l1(), etc...
        if (myGamepad->a()) {
            static int colorIdx = 0;
            // Some gamepads like DS4 and DualSense support changing the color
            // LED. It is possible to change it by calling:
            switch (colorIdx % 3) {
            case 0:
                // Red
                myGamepad->setColorLED(255, 0, 0);
                break;
            case 1:
                // Green
                myGamepad->setColorLED(0, 255, 0);
                break;
            case 2:
                // Blue
                myGamepad->setColorLED(0, 0, 255);
                break;
            }
            colorIdx++;
        }

        if (myGamepad->b()) {
        }

        if (myGamepad->x()) {
            // Duration: 255 is ~2 seconds
            // force: intensity
            myGamepad->setRumble(0xc0 /* force */, 0xc0 /* duration */);
        }

        // Another way to query the buttons, is by calling buttons(), or
        // miscButtons() which return a bitmask.
        /*
        Serial.printf(
            "dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: "
            "%4d, %4d, misc: 0x%02x\n",
            myGamepad->dpad(),       // DPAD, 1 bytes
            myGamepad->buttons(),    // bitmask of pressed buttons, 2 bytes
            myGamepad->axisX(),      // (-511 - 512) left X Axis
            myGamepad->axisY(),      // (-511 - 512) left Y axis
            myGamepad->axisRX(),     // (-511 - 512) right X axis
            myGamepad->axisRY(),     // (-511 - 512) right Y axis
            myGamepad->miscButtons() // bitmak of pressed "misc" buttons, 1 byte
        );*/
    }
    delay(50);
}

void requestEvent() {
    unsigned int offset = 508;

    rx = myGamepad->axisRX() + offset;
    ry = myGamepad->axisRY() + offset;
    lx = myGamepad->axisX() + offset;
    ly = myGamepad->axisY() + offset;
    buttons = myGamepad->buttons();
    dpad = myGamepad->dpad();
    misc = myGamepad->miscButtons();

    const unsigned char mask = 0xFF;

    // send rx
    unsigned char higher_char = (rx >> 8) & mask;
    unsigned char lower_char = rx & mask;

    // write can only send 1 byte at a time
    Wire.write(higher_char);
    Wire.write(lower_char);

    // send ry
    higher_char = (ry >> 8) & mask;
    lower_char = ry & mask;

    Wire.write(higher_char);
    Wire.write(lower_char);

    // send lx
    higher_char = (lx >> 8) & mask;
    lower_char = lx & mask;

    Wire.write(higher_char);
    Wire.write(lower_char);

    // send ly
    higher_char = (ly >> 8) & mask;
    lower_char = ly & mask;

    Wire.write(higher_char);
    Wire.write(lower_char);

    // send buttons bitmask
    higher_char = (buttons >> 8) & mask;
    lower_char = buttons & mask;

    Wire.write(higher_char);
    Wire.write(lower_char);

    // send dpad char
    Wire.write(dpad);

    // send misc button char
    Wire.write(misc);

    Serial.printf(
        "rx: %d, ry: %d, lx: %d, ly: %d, buttons: %x, dpad: %x, misc: %x\n", rx,
        ry, lx, ly, buttons, dpad, misc);
}
