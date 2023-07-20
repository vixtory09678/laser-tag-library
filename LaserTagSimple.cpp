// LaserTagSimple - iamteam.me
// Copyright Wannachat Surasiang 2019
#include "LaserTagSimple.h"

// Armor functions

LaserTagSimple::Armor::Armor() {}

void LaserTagSimple::Armor::setOnDataReceive(CALLBACK) {
  callback = func;
  IrReceiver.enableIRIn();
}

void LaserTagSimple::Armor::handle() {
  if (isReceiveIR()) {
    dePacket();
  }
}

void LaserTagSimple::Armor::dePacket() {
  uint32_t data = (uint32_t)IrReceiver.decodedIRData.decodedRawData;
  uint8_t id = (data >> 12) & 0xFF;
  int8_t damage = (data >> 8) & 0x0F;
  uint8_t checkSum = data & 0xFF;
  uint8_t calChecksum = (~(id + damage)) & 0xFF;

  if (calChecksum == checkSum && callback != NULL && (id < 8)) {
    callback(id, damage);
  }
}

bool LaserTagSimple::Armor::isReceiveIR() {
  if (IrReceiver.decode()) {
    IrReceiver.resume(); // Receive the next value
    return true;
  }
  return false;
}

// Gun functions

void LaserTagSimple::Gun::shoot(uint32_t data) { IrSender.sendSony(data, 16); }

uint32_t LaserTagSimple::Gun::enPacket(uint8_t id, uint8_t damage) {
  id = (id > 7) ? (id % 8) : (id < 0) ? 0 : id;
  damage = (damage > 15) ? (damage % 16) : (damage < 0) ? 0 : damage;

  uint32_t data = ((uint32_t)id << 12) | ((uint32_t)damage << 8);
  data |= (~(id + damage)) & 0xFF;

  return data;
}
