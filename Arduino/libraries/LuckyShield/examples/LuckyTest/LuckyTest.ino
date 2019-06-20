/*
test_lucky

A simple Arduino sketch that reads data from the on-board sensors of the Lucky Shield and checks all gpios.
It prints sensors data via the Serial Monitor.

The following pin are defined in the Lucky Shield library and directly map the digitals
of the Lucky Shield.

created Mar 2016
by  andrea@arduino.org,
    sergio@arduino.org
*/

#include <Lucky.h>
#include <Wire.h>


//set the sea level pressure
#define SEALEVELPRESSURE_HPA 1008

void setup() {

  lucky.begin();
  Serial.begin(9600);
  while(!Serial);
}

void loop() {

  //print on the oled
  lucky.oled().setTextSize(2);
  lucky.oled().setTextColor(WHITE);
  lucky.oled().setCursor(3,23);
  lucky.oled().clearDisplay();
  lucky.oled().print("Arduino");
  lucky.oled().display();

  //write gpio
  lucky.gpio().digitalWrite(LED1,HIGH);
  lucky.gpio().digitalWrite(REL1,HIGH);
  lucky.gpio().digitalWrite(LED2,LOW);
  lucky.gpio().digitalWrite(REL2,LOW);
  delay(1000);
  lucky.gpio().digitalWrite(LED2,HIGH);
  lucky.gpio().digitalWrite(REL2,HIGH);
  lucky.gpio().digitalWrite(LED1,LOW);
  lucky.gpio().digitalWrite(REL1,LOW);
  delay(1000);

  //read gpios value
  Serial.print("LED1: ");
  Serial.print(lucky.gpio().digitalRead(LED1));
  Serial.print(" LED2: ");
  Serial.print(lucky.gpio().digitalRead(LED2));
  Serial.print(" REL1: ");
  Serial.print(lucky.gpio().digitalRead(REL1));
  Serial.print(" REL2: ");
  Serial.println(lucky.gpio().digitalRead(REL2));
  Serial.print(" joy-D: ");
  Serial.print(lucky.gpio().digitalRead(JOYD));
  Serial.print(" joy-R: ");
  Serial.print(lucky.gpio().digitalRead(JOYR));
  Serial.print(" joy-L: ");
  Serial.print(lucky.gpio().digitalRead(JOYL));
  Serial.print("  joy-U: ");
  Serial.print(lucky.gpio().digitalRead(JOYU));
  Serial.print("  joy-C: ");
  Serial.println(lucky.gpio().digitalRead(JOYC));

  //read accelerometer sensor
  lucky.accelerometer().read();
  Serial.print("x: ");
  Serial.print(lucky.accelerometer().x());
  Serial.print("  y: ");
  Serial.print(lucky.accelerometer().y());
  Serial.print("  z: ");
  Serial.println(lucky.accelerometer().z());

  //read magnetometer sensor
  lucky.magnetometer().read();
  Serial.print("Mx: ");
  Serial.print(lucky.magnetometer().x());
  Serial.print("  My: ");
  Serial.print(lucky.magnetometer().y());
  Serial.print("  Mz: ");
  Serial.println(lucky.magnetometer().z());


  //read environment sensor
  Serial.print("Temperature = ");
  Serial.print(lucky.environment().temperature());
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(lucky.environment().pressure() / 100.0F);
  Serial.println(" hPa");
  Serial.print("Approx. Altitude = ");
  Serial.print(lucky.environment().altitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  Serial.print("Humidity = ");
  Serial.print(lucky.environment().humidity());
  Serial.println(" %");

  //clear oled
  lucky.oled().clearDisplay();
  lucky.oled().display();

  delay(1000);

}
