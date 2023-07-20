#include <Adafruit_NeoPixel.h>
#include <LaserTagSimple.h>

#define PIN_LED 5
#define NUM_LED 24

Adafruit_NeoPixel pixel =
    Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

#define ID 2
#define LED_LIFE 13

LaserTagSimple::Armor armor;
int life = 100;

void setLedColor(int r, int g, int b) {
  for (int i = 0; i < NUM_LED; i++) {
    pixel.setPixelColor(i, pixel.Color(r, g, b));
    pixel.show();
  }
}

void setLedColor(int num_led, int r, int g, int b) {
  for (int i = 0; i < num_led; i++) {
    pixel.setPixelColor(i, pixel.Color(r, g, b));
    pixel.show();
  }
}

void onDataReceive(uint8_t id, uint8_t damage) {
  if (id != ID) {
    life = life - damage;
  }

  int ledCount = map(life, 0, 100, 0, 24); // 0 - 24

  setLedColor(0, 0, 0);
  for (int i = 0; i < ledCount; i++) {
    setLedColor(i, 255, 0, 0);
  }

  Serial.print("life is ");
  Serial.println(life);
}

void setup() {
  Serial.begin(9600);
  pixel.begin();
  pixel.setBrightness(150); // length 0-255
  armor.setOnDataReceive(onDataReceive);
  pinMode(LED_LIFE, OUTPUT);
  delay(1000);
  setLedColor(255, 0, 0);
}

void loop() {
  armor.handle();

  if (life <= 0) {
    digitalWrite(LED_LIFE, LOW);
    setLedColor(0, 0, 0); // die
    delay(3000);
    digitalWrite(LED_LIFE, HIGH);
    setLedColor(255, 0, 0); // reborn
    life = 100;
  }
}