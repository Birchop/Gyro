#ifndef GYRO_H
#define GYRO_H

#include <Wire.h>

class Gyro {
  public:
    Gyro(uint8_t MPD, TwoWire &w);
    void writeRegister(uint8_t reg, uint8_t data);
    void begin();
    void calibrate();
    void update();
    void getAngle(int16_t Ax, int16_t Ay, int16_t Az);
    double getPitch();
    double getRoll();
    double getTempC();
    double getTempF();
    double getAcX();
    double getAcY();
    double getAcZ();
    double getGyX();
    double getGyY();
    double getGyZ();

  private:
    uint8_t _MPD;
    TwoWire *_wire;
    int16_t Tmp;
    int16_t AcX, AcY, AcZ, GyX, GyY, GyZ, AcXcal, AcYcal, AcZcal, Tmpcal, GyXcal, GyYcal, GyZcal;
    double t, tx, tf, pitch, roll;
    
};

#endif
