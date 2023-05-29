#include "Gyro.h"
#include <math.h>

Gyro::Gyro(uint8_t MPD, TwoWire &w) : _MPD(MPD), _wire(&w) {}

void Gyro::writeRegister(uint8_t reg, uint8_t data) {
  _wire->beginTransmission(_MPD);
  _wire->write(reg);
  _wire->write(data);
  _wire->endTransmission(true);
}

void Gyro::begin() {
  writeRegister(0x6B, 0); // Wake up the MPU6050
  writeRegister(0x1A, 6); // Set DLPF_CFG to 6
  writeRegister(0x1B, 0); // Set FS_SEL to 0
  writeRegister(0x1C, 0); // Set AFS_SEL to 0
  calibrate();
}
/*
void Gyro::calibrate() {
  int AcXcalArr[10];
  int AcYcalArr[10];
  int AcZcalArr[10];
  int GyXcalArr[10];
  int GyYcalArr[10];
  int GyZcalArr[10];

  for (int i = 0; i < 10; i++) {
    _wire->beginTransmission(_MPD);
    _wire->write(0x3B);
    _wire->endTransmission(false);
    _wire->requestFrom(_MPD, 14, true);
    AcXcalArr[i] = _wire->read() << 8 | _wire->read();
    AcYcalArr[i] = _wire->read() << 8 | _wire->read();
    AcZcalArr[i] = _wire->read() << 8 | _wire->read();
    GyXcalArr[i] = _wire->read() << 8 | _wire->read();
    GyYcalArr[i] = _wire->read() << 8 | _wire->read();
    GyZcalArr[i] = _wire->read() << 8 | _wire->read();
    delay(20);
  }
  AcXcal = 0;
  AcYcal = 0;
  AcZcal = 0;
  GyXcal = 0;
  GyYcal = 0;
  GyZcal = 0;
  for (int i = 0; i < 10; i++) {
    AcXcal += AcXcalArr[i];
    AcYcal += AcYcalArr[i];
    AcZcal += AcZcalArr[i];
    GyXcal += GyXcalArr[i];
    GyYcal += GyYcalArr[i];
    GyZcal += GyZcalArr[i];
  }
  AcXcal = AcXcal / 10;
  AcYcal = AcYcal / 10;
  AcZcal = AcZcal / 10;
  GyXcal = GyXcal / 10;
  GyYcal = GyYcal / 10;
  GyZcal = GyZcal / 10;
  
}
*/

void Gyro::calibrate() {
  int16_t AcXcalArr[10];
  int16_t AcYcalArr[10];
  int16_t AcZcalArr[10];
  int16_t GyXcalArr[10];
  int16_t GyYcalArr[10];
  int16_t GyZcalArr[10];


  for (int i = 0; i < 10; i++) {
    _wire->beginTransmission(_MPD);
    _wire->write(0x3B);
    _wire->endTransmission(false);
    _wire->requestFrom(_MPD, 14, true);
    AcXcalArr[i] = _wire->read() << 8 | _wire->read();
    AcYcalArr[i] = _wire->read() << 8 | _wire->read();
    AcZcalArr[i] = _wire->read() << 8 | _wire->read();
    int Tmp2 = _wire->read() << 8 | _wire->read();
    GyXcalArr[i] = _wire->read() << 8 | _wire->read();
    GyYcalArr[i] = _wire->read() << 8 | _wire->read();
    GyZcalArr[i] = _wire->read() << 8 | _wire->read();
    //_wire->endTransmission(true);
    delay(50);
  }
  AcXcal = 0;
  AcYcal = 0;
  AcZcal = 0;
  GyXcal = 0;
  GyYcal = 0;
  GyZcal = 0;
  for (int i = 0; i < 10; i++) {
    AcXcal += AcXcalArr[i];
    AcYcal += AcYcalArr[i];
    AcZcal += AcZcalArr[i];
    GyXcal += GyXcalArr[i];
    GyYcal += GyYcalArr[i];
    GyZcal += GyZcalArr[i];
  }
  AcXcal = AcXcal / 10;
  AcYcal = AcYcal / 10;
  AcZcal = AcZcal / 10;
  GyXcal = GyXcal / 10;
  GyYcal = GyYcal / 10;
  GyZcal = GyZcal / 10;

  // Print the final result of each 'cal' value
  Serial.println("Final calibration values:");
  Serial.print("AcXcal: ");
  Serial.println(AcXcal);
  Serial.print("AcYcal: ");
  Serial.println(AcYcal);
  Serial.print("AcZcal: ");
  Serial.println(AcZcal);
  Serial.print("GyXcal: ");
  Serial.println(GyXcal);
  Serial.print("GyYcal: ");
  Serial.println(GyYcal);
  Serial.print("GyZcal: ");
  Serial.println(GyZcal);
  delay(5000);
}

void Gyro::update() {
  _wire->beginTransmission(_MPD);
  _wire->write(0x3B);
  _wire->endTransmission(false);
  _wire->requestFrom(_MPD, 14, true);

  int16_t tcal = -1600;


  AcX = (_wire->read() << 8 | _wire->read()) - AcXcal;
  AcY = (_wire->read() << 8 | _wire->read()) - AcYcal;
  AcZ = (_wire->read() << 8 | _wire->read()) - AcZcal;
  Tmp = (_wire->read() << 8 | _wire->read()) - tcal;
  GyX = (_wire->read() << 8 | _wire->read()) - GyXcal;
  GyY = (_wire->read() << 8 | _wire->read()) - GyYcal;
  GyZ = (_wire->read() << 8 | _wire->read()) - GyZcal;
  //_wire->endTransmission(true);

  tx = Tmp + tcal;
  t = tx / 340 + 36.53;
  tf = (t * 9 / 5) + 32;

  getAngle(AcX, AcY, AcZ);

  // Gravity value
  int gravity = 17000;

  // Calculate the gravity components for each axis
  double gravityX = gravity * sin(roll);
  double gravityY = gravity * sin(pitch);
  double gravityZ = gravity * cos(pitch) * cos(roll);

  // Subtract the gravity components from the accelerometer readings
  AcX = (AcX  - gravityX);
  AcY = (AcY - gravityY);
  AcZ = (AcZ - gravityZ);
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

double Gyro::getPitch() {
  return pitch;
}
double Gyro::getRoll() {
  return roll;
}
double Gyro::getTempC() {
  return t;
}
double Gyro::getTempF() {
  return tf;
}
double Gyro::getAcX() {
  return AcX;
}
double Gyro::getAcY() {
  return AcY;
}
double Gyro::getAcZ() {
  return AcZ;
}
double Gyro::getGyX() {
  return GyX;
}
double Gyro::getGyY() {
  return GyY;
}
double Gyro::getGyZ() {
  return GyZ;
}
