/*
Simple GPIO

A simple Arduino sketch that turn off and on the onboard relais and leds of the Lucky Shield.
When the joystick is moved on the left, led n1 and relay n1 are turned on,
when the joystick is moved on the right, led n2 and relay n2 are turned off.

The following pins are defined in the Lucky Shield library, they directly map the digitals
of the Lucky Shield

LED1: on board first LED
LED2: on board second LED

REL1: on board first Relay
REL2: on board second Relay

JOYR: on board Joystick - moved right
JOYL: on board Joystick - moved left
JOYU: on board Joystick - moved up
JOYD: on board Joystick - moved down
JOYC: on board Joystick - central pressed

PIR: on board (Passive Infrared Sensor) Presence Sensor

PB1: on board first Push Button
PB2: on board second Push Button

created Mar 2016
by  andrea@arduino.org,
    sergio@arduino.org
*/

#include <Lucky.h>
#include <Wire.h>

void setup() {
  lucky.begin();

}

void loop() {

  //Check if the the joystick is moved to the left
  if(lucky.gpio().digitalRead(JOYL) == HIGH){
    lucky.gpio().digitalWrite(LED1, HIGH);
    lucky.gpio().digitalWrite(REL1, HIGH);
  }
  else {
    lucky.gpio().digitalWrite(LED1, LOW);
    lucky.gpio().digitalWrite(REL1, LOW);
  }

  //Check if the the joystick is moved to the right
  if(lucky.gpio().digitalRead(JOYR) == HIGH){
    lucky.gpio().digitalWrite(LED2, HIGH);
    lucky.gpio().digitalWrite(REL2, HIGH);
  }
  else {
    lucky.gpio().digitalWrite(LED2, LOW);
    lucky.gpio().digitalWrite(REL2, LOW);
  }
}
