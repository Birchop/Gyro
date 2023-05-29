#ifndef GYRO_H
#define GYRO_H

#include <Wire.h>

class Gyro {
  public:
    Gyro(uint8_t MPD, TwoWire &w);
    void begin();
    void update();
    void getAngle(int16_t Ax, int16_t Ay, int16_t Az);
    double getPitch();
    double getRoll();
    double getTempC();
    double getTempF();
    int16_t getAcX();
    int16_t getAcY();
    int16_t getAcZ();
    int16_t getGyX();
    int16_t getGyY();
    int16_t getGyZ();

  private:
    uint8_t _MPD;
    TwoWire *_wire;
    int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
    double t, tx, tf, pitch, roll;
    
};

#endif
