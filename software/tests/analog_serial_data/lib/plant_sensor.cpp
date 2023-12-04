#include "plant_sensor.h"


void plant_sensor_setup(){
    ledcSetup(0, 100000, 2);
    ledcAttachPin(LED_PWM_PIN, 0);  
    ledcWrite(0, 2);
    pinMode(MOISTURE_PIN, INPUT);
    pinMode(POWER_3V, OUTPUT);
    digitalWrite(POWER_3V, HIGH);

    pinMode(TOGGLE_PIN, OUTPUT);
    digitalWrite(TOGGLE_PIN, HIGH);
    delay(5);
}

double read_voltage(int pin){
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if(reading < 1 || reading > 4095) return 0;
  // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  return -0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089;
} // Added an improved polynomial, use either, comment out as required

void toggle(){
  isOn = !isOn;
  if(isOn){
    digitalWrite(TOGGLE_PIN, HIGH);

  }
  else{
    digitalWrite(TOGGLE_PIN, LOW);
  }
}

void plant_sensor_loop(){
    float avg = 0;
    float value = 0;
    for(unsigned int freq = MIN_FREQ; freq<MAX_FREQ; freq+=FREQ_STEP){
        ledcChangeFrequency(0, freq*1000, 2);
        value = read_voltage(ANALOG_PIN);
        avg += (esp/value);
        delayMicroseconds(10);
    }
    toggle();

    // Serial.print("Value : ");
    // Serial.println(value);
    Serial.print("Avg : ");
    Serial.println(avg/(MAX_FREQ - MIN_FREQ));
}