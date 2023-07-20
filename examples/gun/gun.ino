#include <LaserGunProjectLab.h>

#define GUN_TRIGGER_PIN 2

#define DAMAGE 10
#define ID 1

LaserGunProjectLab gun(GUN);
long bullet;

bool isTrigger(int trigPin) {
  static bool flagCheck = false;
  if (digitalRead(trigPin) == 0) {
    if (flagCheck == false) {
      flagCheck = true;
      delay(50);
      return true;
    } else {
      return false;
    }
  } else {
    flagCheck = false;
    return false;
  }
}

void setup() {
  Serial.begin(9600);
  bullet = gun.enPacket(ID, DAMAGE);

  pinMode(GUN_TRIGGER_PIN, INPUT);
}

void loop() {
  if (isTrigger(GUN_TRIGGER_PIN)) {
    gun.shootGun(bullet);
  }
}