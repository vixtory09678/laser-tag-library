#include <LaserGunProjectLab.h>
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

DFRobotDFPlayerMini player;
SoftwareSerial mSerial(10, 11); // rx tx

typedef enum Media{
    REBORN = 1,
    DEAD = 2,
    ATTACKED = 3
}MusicName;

MusicName sound = REBORN;

#define PIN_LED 5
#define NUM_LED 24
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

#define ID          2
#define LED_LIFE    13

LaserGunProjectLab armor(ARMOR);
int life = 100;

void setLedColor(int r, int g, int b) {
    for (int i = 0; i < NUM_LED; i++) {
        pixel.setPixelColor(i, pixel.Color(r,g,b));
        pixel.show();
    }
}

void setLedColor(int num_led, int r, int g, int b) {
    for (int i = 0; i < num_led; i++) {
        pixel.setPixelColor(i, pixel.Color(r,g,b));
        pixel.show();
    }
}

void onDataReceive(uint8_t id,uint8_t damage){
    if (id != ID){
        life = life - damage;
    }

    int ledCount =  map(life,0,100,0,24); // 0 - 24

    setLedColor(0,0,0);
    for (int i = 0; i < ledCount; i++){
        setLedColor(i,255,0,0);
    }

    player.play(ATTACKED);
    Serial.print("life is ");
    Serial.println(life);
}

void setup(){
    Serial.begin(9600);
    mSerial.begin(9600);

    pixel.begin();
    pixel.setBrightness(150); // length 0-255

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

    // set function event on armor receive data
    armor.setOnDataReceive(onDataReceive);

    pinMode(LED_LIFE,OUTPUT);
    delay(1000);
    player.play(REBORN);
    setLedColor(255,0,0);
}

void loop(){
    armor.handle();

    if (life <= 0){
        digitalWrite(LED_LIFE,LOW);
        player.play(DEAD);
        setLedColor(0,0,0); // die
        delay(3000);
        digitalWrite(LED_LIFE,HIGH);
        player.play(REBORN);
        setLedColor(255,0,0); // reborn
        life = 100;
    }
}