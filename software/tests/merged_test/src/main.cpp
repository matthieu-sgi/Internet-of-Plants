#include <Arduino.h>
#include "../lib/synthetizer.h"
#include "../lib/plant_sensor.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  synthetiser_setup(Serial);
  plant_sensor_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  synthetizer_loop();
  plant_sensor_loop();

}

