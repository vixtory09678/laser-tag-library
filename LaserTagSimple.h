// LaserTagSimple - iamteam.com
// Copyright Wannachat Surasiang 2019

#ifndef LASER_GUN_PROJECT_LAB_H
#define LASER_GUN_PROJECT_LAB_H

#include <Arduino.h>
#include <IRremote.hpp>

#define CALLBACK void (*func)(uint8_t id, uint8_t damage)
#define IR_PIN 3
#define IR_RECV_PIN 2

enum TypeFunction { GUN, ARMOR };

namespace LaserTagSimple {
class Gun {
public:
  void shoot(uint32_t bullet);
  uint32_t enPacket(uint8_t id, uint8_t damage);
};

class Armor {
public:
  Armor();
  void handle();
  bool isReceiveIR();
  void setOnDataReceive(CALLBACK);

private:
  void (*callback)(uint8_t id, uint8_t damage);
  void dePacket();
};
} // namespace LaserTagSimple

#endif