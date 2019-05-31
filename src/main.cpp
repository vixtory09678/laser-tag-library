#include <Arduino.h>
#include "LaserGunProjectLab.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

#define USE_DELAY_SHOOT

// declear DFPlayer module music
DFRobotDFPlayerMini player;
SoftwareSerial mSerial(10, 11);             // rx tx

// declear pin
#define GUN_TRIGGER_PIN         2           //connect trigger
#define GUN_LASER_POINT_PIN     7           //connect laser

// declear constance variable
#define DAMAGE                  10          // set damage   0-15
#define ID                      1           // set id       0 - 7

// declear Main library LaserGunProjectlab
LaserGunProjectLab gun(GUN);
long bullet = 0;

// this function will be one shot pressed from the switch button and return true if button press and return false if button release
bool isTrigger(int trigPin){
    static bool flagCheck = false;
    if (digitalRead(trigPin) == 0){
        if (flagCheck == false){
            flagCheck = true;
            delay(50);
            return true;
        }else{
            return false;
        }
    }else{
        flagCheck = false;
        return false;
    }
}

long loopTime = 0;

void setup(){
    // setup serial communication for communicate between Hardware and Computer
    Serial.begin(9600);
    // setup software serial communication for communicate between Hardware and DFPlayer
    mSerial.begin(9600);

    // set media player
    if (!player.begin(mSerial) ){
        Serial.println(F("Unable to begin"));
        Serial.println(F("check sd card or serial communication"));
        // error when not connect software serial
        while (1) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
    }
    // if success initialize
    Serial.println(F("DFPlayer online!!"));
    player.volume(25);

    // fill bullet
    bullet = gun.enPacket(ID,DAMAGE);
    pinMode(GUN_TRIGGER_PIN, INPUT);
    pinMode(GUN_LASER_POINT_PIN, OUTPUT);
    loopTime = millis();
}

void loop(){
    if (isTrigger(GUN_TRIGGER_PIN)){

        #ifdef USE_DELAY_SHOOT
        long now = millis();
        if (now - loopTime > 100){
            digitalWrite(GUN_LASER_POINT_PIN, HIGH);
            gun.shootGun(bullet);
            player.play(1);
            digitalWrite(GUN_LASER_POINT_PIN, LOW);
            loopTime = now;
        }
        #else
        digitalWrite(GUN_LASER_POINT_PIN, HIGH);
        gun.shootGun(bullet);
        player.play(1);
        digitalWrite(GUN_LASER_POINT_PIN, LOW);
        #endif
    }
}