#include <Wire.h>
#include "MatrixLED.h"

MatrixLED matrixLed(12, 11, 13, 8, 8);
long k;
long x;
long y;

void setup()
{
  Serial.begin(9600);
  matrixLed.begin(2);
  k = -1;
  x = 3;
  y = 3;
}

void loop()
{
  matrixLed.clear();
  matrixLed.drawPixel(x, y, 1);
  matrixLed.writeDisplay();
  delay(200);
  for (int i = 1; i <= 7; i = i + (1)) {
    Serial.print('i');
    Serial.println(i);
    k = k * -1;
    for (int j = (1); j <= (i); j = j + (1)) {
      y = y + k;
      Serial.print('y');
      Serial.println(y);
      matrixLed.drawPixel(x, y, 1);
      matrixLed.writeDisplay();
      delay(200);
    }
    for (int j = (1); j <= (i); j = j + (1)) {
      x = x + k;
      Serial.print('x');
      Serial.println(x);
      matrixLed.drawPixel(x, y, 1);
      matrixLed.writeDisplay();
      delay(200);
    }
  }

}
