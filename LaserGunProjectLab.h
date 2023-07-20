// LaserGunProjectLab - iamteam.com
// Copyright Wannachat Surasiang 2019

#ifndef LASER_GUN_PROJECT_LAB_H
#define LASER_GUN_PROJECT_LAB_H

#include <Arduino.h>
#include <IRremote.h>

#define CALLBACK void (*func)(uint8_t id, uint8_t damage)
#define IR_PIN 3
#define IR_RECV_PIN 2

enum TypeFunction { GUN, ARMOR };

class LaserGunProjectLab {
public:
  LaserGunProjectLab(TypeFunction type);
  void handle();
  void shootGun(uint32_t bullet);
  uint32_t enPacket(uint8_t id, uint8_t damage);
  bool isReceiveIR();
  void setOnDataReceive(CALLBACK);

private:
  IRrecv _recv;
  IRsend _send;
  decode_results results;

  TypeFunction _type;
  void (*callback)(uint8_t id, uint8_t damage);
  void dePacket();
};

#endif