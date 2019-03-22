#include <LaserGunProjectLab.h>

#define ID          2

#define LED_LIFE 13

LaserGunProjectLab armor(ARMOR);
int life = 100;

void onDataReceive(uint8_t id,uint8_t damage){
    if (id != ID){
        life = life - damage;
    }
}

void setup(){
    Serial.begin(9600);
    armor.setOnDataReceive(onDataReceive);
    pinMode(LED_LIFE,OUTPUT);
}

void loop(){
    armor.handle();

    if (life <= 0){
        digitalWrite(LED_LIFE,LOW);
        delay(3000);
        digitalWrite(LED_LIFE,HIGH);
        life = 100;
    }
}