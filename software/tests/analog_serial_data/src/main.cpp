#include <Arduino.h>

#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 14
#define POWER_3V 13
#define TOGGLE_PIN 33
#define CALIBRATION_STEPS 20


const int nb_values = (int)((MAX_FREQ-MIN_FREQ)/FREQ_STEP);
double freq_array[nb_values];
double calibrating_array[nb_values];

bool isOn = true;

const int esp = 10;

double avg_error = 0;



void plant_sensor_setup(){
    ledcSetup(0, 100000, 2);
    ledcAttachPin(LED_PWM_PIN, 0);  
    ledcWrite(0, 2);
    pinMode(ANALOG_PIN, INPUT);
    delay(5);
}




void plant_sensor_loop(){
    avg_error = 0;
    int values_array[nb_values];
    float value = 0;
    for(unsigned int freq = MIN_FREQ; freq<MAX_FREQ; freq+=FREQ_STEP){
        int index = (freq-MIN_FREQ)/FREQ_STEP;
        value = analogRead(ANALOG_PIN);

        values_array[index] = value;

        ledcChangeFrequency(0, freq*1000, 2);
        // avg += (esp/value);
        delayMicroseconds(10);
    }
    // Serial.println(values_array.toString());
    for(int i = 0; i<nb_values; i++){
      if (i < nb_values-1){
        Serial.print(values_array[i]);
        Serial.print(",");
      }else{
        Serial.println(values_array[i]);
      }
    }
}



void setup() {

  Serial.begin(115200);

  plant_sensor_setup();

}


void socket_communication(){
  
}


void loop() {

  plant_sensor_loop();


}

