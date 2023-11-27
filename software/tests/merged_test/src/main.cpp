#include <Arduino.h>
// #include "../lib/synthetizer.h"
// #include "../lib/plant_sensor.h"
// #include "AudioTools.h"
// #include "Wifi.h"
#include "WiFi.h"

const char* ssid = "4D-Space";
// const char* ssid = "PoleDeVinci_DVIC";
// const char* password = "8PfURsp!dvic";
const char* password = "CestPasRFIci42";



const char* host = "10.1.65.13";
// const char* host = "172.21.72.178";
const int port = 13000;


#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 14
#define POWER_3V 13
#define MOISTURE_PIN 25
#define TOGGLE_PIN 33
#define CALIBRATION_STEPS 20


// AudioInfo info2(8000, 1, 16);
// int16_t sine_array[] = {0, 4560, 9031, 13327, 17363, 21062, 24350, 27165, 29450, 31163, 32269, 32747, 32587, 31793, 30381, 28377, 25820, 22761, 19259, 15383, 11206, 6812, 2285, -2285, -6812, -11206, -15383, -19259, -22761, -25820, -28377, -30381, -31793, -32587, -32747, -32269, -31163, -29450, -27165, -24350, -21062, -17363, -13327, -9031, -4560  };
// GeneratorFromArray<int16_t> sineWave(sine_array,0,false);
// GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
// AnalogAudioStream out; 
// StreamCopy copier(out, sound);                             // copies sound into i2

const int nb_values = (int)((MAX_FREQ-MIN_FREQ)/FREQ_STEP);
double freq_array[nb_values];
double calibrating_array[nb_values];

bool isOn = true;

const int esp = 10;

double avg_error = 0;

// void synthetizer_setup(HardwareSerial synthe_serial){

//     // AudioLogger::instance().begin(synthe_serial, AudioLogger::Warning);
//    // start I2S
//    synthe_serial.println("starting I2S...");
//    auto config = out.defaultConfig(TX_MODE);
//    config.copyFrom(info2); 
//    out.begin(config);
//    // Setup sine wave
//    sineWave.begin(info2);

// }


// void synthetizer_loop(){
//     copier.copy();
// }

double read_voltage(int pin){
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if(reading < 1 || reading > 4095) return 0;
  // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  return -0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089;
} // Added an improved polynomial, use either, comment out as required

void calibration() {
  float value = 0;
  int calibration_steps = 0;
  while (calibration_steps < CALIBRATION_STEPS){
  // Add all the calibrating values
    for(unsigned int freq = MIN_FREQ; freq<MAX_FREQ; freq+=FREQ_STEP){
        int index = (freq-MIN_FREQ)/FREQ_STEP;
        value = read_voltage(ANALOG_PIN);
        // avg_error += abs(freq_array[index] - value);
        calibrating_array[index] += value;
        ledcChangeFrequency(0, freq*1000, 2);
        // avg += (esp/value);
        delayMicroseconds(10);
    }
    // Serial.print("Calibration step : ");
    // Serial.println(calibration_steps);
    // Serial.println("Calibrating...");
    delay(100);
    calibration_steps++;
  }
  
  // Divide by the number of calibration steps
  for(int i = 0; i<sizeof(calibrating_array)/sizeof(calibrating_array[0]); i++){
        calibrating_array[i] /= float(CALIBRATION_STEPS);

  }

}


void plant_sensor_setup(){
    ledcSetup(0, 100000, 2);
    ledcAttachPin(LED_PWM_PIN, 0);  
    ledcWrite(0, 2);
    pinMode(ANALOG_PIN, INPUT);
    delay(5);
}


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

WiFiClient client;

void connection_setup(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi network");

  // Open socket to server
  if(!client.connect(host, port)) {
    Serial.println("Connection to server failed");
    return;
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // synthetizer_setup(Serial);
  // plant_sensor_setup();
  connection_setup();

  // Serial.print(CALIBRATION_STEPS);
}


void socket_communication(){
  
}


void loop() {
  // put your main code here, to run repeatedly:
  // synthetizer_loop();
  // plant_sensor_loop();
  // double value = read_voltage(ANALOG_PIN);
  // Serial.println(value);
  // delay(100);
  // client.print("Noice");
  client.print("16 14 15;\n");
  delay(1000);
  

}

