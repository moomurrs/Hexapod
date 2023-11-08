#include <Bluepad32.h>

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

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepad == gp) {
            Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n",
                          i);
            myGamepad = nullptr;
            foundGamepad = true;
            break;
        }
    }

    if (!foundGamepad) {
        Serial.println(
            "CALLBACK: Gamepad disconnected, but not found in myGamepads");
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
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
            myGamepad->dpad(),       // DPAD
            myGamepad->buttons(),    // bitmask of pressed buttons
            myGamepad->axisX(),      // (-511 - 512) left X Axis
            myGamepad->axisY(),      // (-511 - 512) left Y axis
            myGamepad->axisRX(),     // (-511 - 512) right X axis
            myGamepad->axisRY(),     // (-511 - 512) right Y axis
            myGamepad->miscButtons() // bitmak of pressed "misc" buttons
        );
        */
    }
    Serial.printf("size: %d\n", sizeof(myGamepad->miscButtons()));
    delay(1000);
}
