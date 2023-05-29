#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "Gyro.h"

#define PIN        23
#define NUMPIXELS  1

TwoWire MyWire(i2c1,18,19);
Gyro gyro(0x68, MyWire);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  MyWire.begin();
  Serial.begin(115200);
  delay(5000);
  pixels.begin();
  gyro.begin();
  //gyro.calibrate();
}

void loop() {
  gyro.update();
  
  float  red = constrain(map(gyro.getPitch(), -85, 85, 0, 255), 0, 255);
  int green = 0;  // This could be set to any value you prefer or could also be mapped to another sensor reading
  float  blue = constrain(map(gyro.getRoll(), -85, 85, 0, 255), 0, 255);
  
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(red, green, blue));
  pixels.show();
  
  delay(100);

  Serial.print("Angle: Pitch = ");
  Serial.print(gyro.getPitch());
  Serial.print(" Roll = ");
  Serial.println(gyro.getRoll());
  Serial.print("Accelerometer: X = ");
  Serial.print(gyro.getAcX());
  Serial.print(" Y = ");
  Serial.print(gyro.getAcY());
  Serial.print(" Z = ");
  Serial.println(gyro.getAcZ());
  Serial.print("Temperature in celsius = ");
  Serial.print(gyro.getTempC());
  Serial.print(" fahrenheit = ");
  Serial.println(gyro.getTempF());
  Serial.print("Gyroscope: X = ");
  Serial.print(gyro.getGyX());
  Serial.print(" Y = ");
  Serial.print(gyro.getGyY());
  Serial.print(" Z = ");
  Serial.println(gyro.getGyZ());
  delay(50);
}
