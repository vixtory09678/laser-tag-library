// LaserGunProjectLab - iamteam.me
// Copyright Wannachat Surasiang 2019
#include "LaserGunProjectLab.h"

LaserGunProjectLab::LaserGunProjectLab(TypeFunction type)
    : _type(type), _recv(IR_RECV_PIN) {}

void LaserGunProjectLab::setOnDataReceive(CALLBACK) {
  if (_type == ARMOR) {
    callback = func;
    _recv.enableIRIn();
  }
}

void LaserGunProjectLab::shoot(uint32_t data) {
  if (_type != GUN)
    return;
  _send.sendSony(data, 16);
}

uint32_t LaserGunProjectLab::enPacket(uint8_t id, uint8_t damage) {
  if (_type != GUN)
    return;

  id = (id > 7) ? (id % 8) : (id < 0) ? 0 : id;
  damage = (damage > 15) ? (damage % 16) : (damage < 0) ? 0 : damage;

  uint32_t data = ((uint32_t)id << 12) | ((uint32_t)damage << 8);
  data |= (~(id + damage)) & 0xFF;

  return data;
}

void LaserGunProjectLab::handle() {
  if (isReceiveIR()) {
    dePacket();
  }
}

void LaserGunProjectLab::dePacket() {
  if (_type != ARMOR)
    return;

  uint32_t data = (uint32_t)results.value;
  uint8_t id = (data >> 12) & 0xFF;
  int8_t damage = (data >> 8) & 0x0F;
  uint8_t checkSum = data & 0xFF;
  uint8_t calChecksum = (~(id + damage)) & 0xFF;

  if (calChecksum == checkSum && callback != NULL && (id < 8)) {
    callback(id, damage);
  }
}

bool LaserGunProjectLab::isReceiveIR() {
  if (_type != ARMOR)
    return;

  if (_recv.decode(&results)) {
    _recv.resume(); // Receive the next value
    return true;
  }
  return false;
}