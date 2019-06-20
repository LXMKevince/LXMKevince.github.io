/*
Presence Alarm

A simple Arduino sketch that play a sound with the on board buzzer when a presence is
detected by the PIR

The buzzer is directly connected on the digital pin 5

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

#define BUZ 5
#define NOTE_1 220
#define NOTE_2 300

void setup() {

  lucky.begin();

}

void loop() {

  if(lucky.gpio().digitalRead(PIR) == LOW ){
    for(int i = 0; i < 5; i++){
      tone(BUZ, NOTE_2);
      delay(500);
      tone(BUZ, NOTE_1);
      delay(500);
    }
    noTone(BUZ);
  }

}
