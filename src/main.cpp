#include <Arduino.h>
#include "LaserGunProjectLab.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

#define USE_DELAY_SHOOT

DFRobotDFPlayerMini player;
SoftwareSerial mSerial(10, 11); // rx tx

#define GUN_TRIGGER_PIN 2

#define DAMAGE      10
#define ID          1

LaserGunProjectLab gun(GUN);
long bullet;

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
    Serial.begin(9600);
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
    Serial.println(F("DFPlayer online!!"));
    player.volume(25);

    // fill bullet
    bullet = gun.enPacket(ID,DAMAGE);
    pinMode(GUN_TRIGGER_PIN, INPUT);
    loopTime = millis();
}

void loop(){
    if (isTrigger(GUN_TRIGGER_PIN)){

        #ifdef USE_DELAY_SHOOT
        long now = millis();
        if (now - loopTime > 100){
            gun.shootGun(bullet);
            player.play(1);
            loopTime = now;
        }
        #else
        gun.shootGun(bullet);
        player.play(1);
        #endif
    }
}