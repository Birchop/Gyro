#include "Gyro.h"
#include <math.h>

Gyro::Gyro(uint8_t MPD, TwoWire &w) : _MPD(MPD), _wire(&w) {}

void Gyro::begin() {
  _wire->beginTransmission(_MPD);
  _wire->write(0x6B);
  _wire->write(0);
  _wire->endTransmission(true);
}

void Gyro::update() {
  _wire->beginTransmission(_MPD);
  _wire->write(0x3B);
  _wire->endTransmission(false);
  _wire->requestFrom(_MPD, 14, true);
  
  // Assuming calibration values
  int16_t AcXcal = -950;
  int16_t AcYcal = -300;
  int16_t AcZcal = 0;
  int16_t tcal = -1600;
  int16_t GyXcal = 480;
  int16_t GyYcal = 170;
  int16_t GyZcal = 210;
  /*
  AcX = _wire->read() << 8 | _wire->read() + AcXcal;
  AcY = _wire->read() << 8 | _wire->read() + AcYcal;
  AcZ = _wire->read() << 8 | _wire->read() + AcZcal;
  Tmp = _wire->read() << 8 | _wire->read() + tcal;
  GyX = _wire->read() << 8 | _wire->read() + GyXcal;
  GyY = _wire->read() << 8 | _wire->read() + GyYcal;
  GyZ = _wire->read() << 8 | _wire->read() + GyZcal;
  */
  AcX = _wire->read() << 8 | _wire->read();
  AcY = _wire->read() << 8 | _wire->read();
  AcZ = _wire->read() << 8 | _wire->read();
  Tmp = _wire->read() << 8 | _wire->read();
  GyX = _wire->read() << 8 | _wire->read();
  GyY = _wire->read() << 8 | _wire->read();
  GyZ = _wire->read() << 8 | _wire->read();
  
  tx = Tmp + tcal;
  t = tx / 340 + 36.53;
  tf = (t * 9 / 5) + 32;
  
  getAngle(GyX, GyY, GyZ);
}

void Gyro::getAngle(int16_t Ax, int16_t Ay, int16_t Az) {
  double x = Ax;
  double y = Ay;
  double z = Az;
  pitch = atan(x / sqrt((y * y) + (z * z)));
  roll = atan(y / sqrt((x * x) + (z * z)));
  pitch = pitch * (180.0 / 3.14);
  roll = roll * (180.0 / 3.14) ;
}

double Gyro::getPitch() { return pitch; }
double Gyro::getRoll() { return roll; }
double Gyro::getTempC() { return t; }
double Gyro::getTempF() { return tf; }
int16_t Gyro::getAcX() { return AcX/100; }
int16_t Gyro::getAcY() { return AcY/100; }
int16_t Gyro::getAcZ() { return AcZ/100; }
int16_t Gyro::getGyX() { return GyX/100; }
int16_t Gyro::getGyY() { return GyY/100; }
int16_t Gyro::getGyZ() { return GyZ/100; }
