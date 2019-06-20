/*
Weather Station

A simple Arduino sketch that reads values from temperature, humidity and pressure sensors
of the Lucky Shield and display it in the OLED add-on (and also via Serial).

created Mar 2016
by  andrea@arduino.org,
    sergio@arduino.org
*/

#include <Lucky.h>
#include <Wire.h>

String tmp_lbl, tmp_um, tmp_val, hum_lbl, hum_um, hum_val, pre_lbl, pre_um, pre_val;
void setup() {
  Serial.begin(9600);

  lucky.begin();

  //setup the Lucky OLED
  lucky.oled().setTextSize(0.5);
  lucky.oled().setTextColor(WHITE);
  lucky.oled().clearDisplay();

  tmp_lbl = "Temper.: ";
  hum_lbl = "Humidity:";
  pre_lbl = "Pressure:";

  tmp_um = " C.";
  hum_um = " %";
  pre_um = " hPa";
}

void loop() {

  lucky.oled().clearDisplay();

  tmp_val = String(lucky.environment().temperature());
  lucky.oled().setCursor(5, 10);
  lucky.oled().print(tmp_lbl + tmp_val + tmp_um);
  Serial.println(tmp_lbl + tmp_val + tmp_um);

  hum_val = String(lucky.environment().humidity());
  lucky.oled().setCursor(5, 30);
  lucky.oled().print(hum_lbl + hum_val + hum_um);
  Serial.println(hum_lbl + hum_val + hum_um);

  pre_val = String(lucky.environment().pressure() / 100.0F);
  lucky.oled().setCursor(5, 50);
  lucky.oled().print(pre_lbl + pre_val + pre_um);
  Serial.println(pre_lbl + pre_val + pre_um);

  lucky.oled().display();

  delay(1000);
}
