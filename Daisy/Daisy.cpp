#include "Arduino.h"
#include "Daisy.h"

/**
 * PUBLIC FUNCTIONS
 */
/**
 * Note md is a private variable that represents the motor shield object.
 * Documentation for functions associated with the object DualVNH5019MotorShield
 * can be found 
 */
Daisy::Daisy() {
    Serial.begin(115200);
    Serial.println("Initializing Daisy!");
    #if ENABLE_DEBUG == 1
    Serial.println("Daisy's Debugging has been ENABLED!");
    #else
    Serial.println("Daisy's Debugging has been DISABLED!");
    #endif
    pingL = -1;
    pingR = -1;
    md.init();
}

Daisy::Daisy(int leftPingPin, int rightPingPin) : Daisy() {
    Serial.println("Initializing Daisy with Ping Values:");
    Serial.print("    Left:");
    Serial.println(leftPingPin);
    Serial.print("    Right:");
    Serial.println(rightPingPin);
    pingL = leftPingPin;
    pingR = rightPingPin;
}

/**
 * Speed should be between 0 and 350
 */
void Daisy::forward(int speed) {
    if (!validSpeed(speed)) {
        return;
    }
    DEBUGLN("Moving forward at %d speed", speed)
    motorL(speed);
    motorR(speed);
}

/**
 * Speed should be between 0 and 350
 */
void Daisy::backward(int speed) {
    if (!validSpeed(speed)) {
        return;
    }
    DEBUGLN("Moving backward at %d speed", speed)
    motorL(-speed);
    motorR(-speed);
}

/**
 * Halts Daisey T.T
 */
void Daisy::halt() {
    DEBUGLN("Daisey is stopping.")
    md.setBrakes(400,400);
}

void Daisy::turn(Dir dir, int speed) {
    if (!validSpeed(speed)) {
        return;
    }
    int left = 1;
    int right = 1;
    if (dir == CW) {
        DEBUGLN("Daisey is turning CW at %d speed", speed)
        left = 1;
        right = -1;
    } else {
        DEBUGLN("Daisey is turning CCW at %d speed", speed)
        left = -1;
        right = 1;
    }
    motorL(speed * left);
    motorR(speed * right);
}

/**
 * Specify direction clockwise (CW) or counter-clockwise (CCW).
 * Speed should be between 0 and 350.
 * Time is in milliseconds.
 */
void Daisy::turn(Dir dir, int speed, unsigned long time) {
    if (!validSpeed(speed)) {
        return;
    }
    int left = 1;
    int right = 1;
    if (dir == CW) {
        DEBUGLN("Daisey is turning CW at %d speed for %lu time", speed, time)
        left = 1;
        right = -1;
    } else {
        DEBUGLN("Daisey is turning CCW at %d speed for %lu time", speed, time)
        left = -1;
        right = 1;
    }
    for (int i = 0; i < time; i++) {
        motorL(speed * left);
        motorR(speed * right);
        delay(1);
    }
    halt();
}


/**
 * PRIVATE FUNCTIONS
 */
void Daisy::stopIfFault() {
    if (md.getM1Fault()) {
        Serial.println("M1 FAULT");
        while(1);
    }
    if (md.getM2Fault()) {
        Serial.println("M2 FAULT");
        while(1);
    }
}
void Daisy::motorL(int speed) {
    int modifiedSpeed = speed < 0 ? 
        (speed - (LMOTOR_COMP_BACK)) :
        (speed + (LMOTOR_COMP_FOR));
    #if LMOTOR == 1
    md.setM1Speed(modifiedSpeed);
    #elif LMOTOR == 2
    md.setM2Speed(modifiedSpeed);
    #else
    DEBUGLN("Unknown Left Motor Number %d," LMOTOR)
    #endif
}

void Daisy::motorR(int speed) {
    int modifiedSpeed = speed < 0 ? 
        (speed - (RMOTOR_COMP_BACK)) :
        (speed + (RMOTOR_COMP_FOR));
    #if RMOTOR == 1
    md.setM1Speed(modifiedSpeed);
    #elif RMOTOR == 2
    md.setM2Speed(modifiedSpeed);
    #else
    DEBUGLN("Unknown Right Motor Number %d," RMOTOR)
    #endif
}

bool Daisy::validSpeed(int speed) {
    if (speed >= 0 && speed <= 350) {
        DEBUGLN("Speed of %d is valid", speed)
        return true;
    } else {
        DEBUGLN("Speed of %d is invalid", speed)
        return false;
    }
}

