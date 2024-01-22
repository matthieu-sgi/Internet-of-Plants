#include <Arduino.h>
#include <WiFi.h>
#include <iop_maths_lib.h>


#define BUFFER_SIZE 400
#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 34
#define LED_BUILTIN 2
#define DELAY 50

const char* ssid = <SSID>;
const char* password = <PASSWORD>;


const char* host = "192.168.10.111";
const int port = 3000;

WiFiClient server; // TCP server  

const float alpha = 0.99;
const int sweep_data_size = (MAX_FREQ - MIN_FREQ) / FREQ_STEP;
int old_sum = 0;
// float new_standard_dev = 0.0;

float old_standard_dev = 0.0;

int swept_data[sweep_data_size];
float ema[sweep_data_size];
float ema_std[sweep_data_size];
float old_sum_normalized;
float normalized_data[sweep_data_size];

unsigned long time_last_msg = millis();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Wifi connection setup

  // Comment 
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to WiFi network");

  // // // Connect to TCP server
  if(!server.connect(host, port)){
    Serial.println("Connection to TCP server failed");
    return;
  }

  Serial.println("Connected to TCP server");
  //Uncomment

  Serial.println("Pins setup");
  // GPIO setup
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);

  

  // Initialize data array
  for(int i = 0; i < sweep_data_size; i++){
    swept_data[i] = 0;
    ema[i] = 0;
    ema_std[i] = 1;
  }

  Serial.println("PWM setup");
  // Initialize PWM
  ledcSetup(0, 100000, 2);
  ledcAttachPin(LED_PWM_PIN, 0);  
  ledcWrite(0, 2);

  Serial.println("Setup done");

  pinMode(LED_BUILTIN, OUTPUT);

}

void sweep(int* data, int size){
  int value = 0;
  for(int index = 0; index < size; index += FREQ_STEP){
    value = analogRead(ANALOG_PIN);
    data[index] = value;
    int freq = MIN_FREQ + index * FREQ_STEP;
    ledcChangeFrequency(0, freq*1000, 2);
    delayMicroseconds(10);
  }
}




void loop() {
  // put your main code here, to run repeatedly:

  if(!server.connected()){
    Serial.println("Connection to TCP server lost");
    delay(1000);
    server.connect(host, port);
    return;
  }
  server.flush();
  sweep(swept_data, sweep_data_size);
  // ema_update(ema, swept_data, sweep_data_size, alpha);
  // ema_std_update(swept_data, sweep_data_size, ema_std, ema, alpha);

  // data_normalizer(swept_data, sweep_data_size, ema, ema_std, normalized_data);

  // float avg = average(normalized_data, sweep_data_size);
  // float standard_dev = standard_deviation(normalized_data, sweep_data_size,avg);

  // int sum_swept_data = int_sum(swept_data, sweep_data_size);
  // float sum_normalized_data = float_sum(normalized_data, sweep_data_size);
  // float diff = abs(sum_normalized_data - old_sum_normalized);
  // int threshed_value =  (diff > 20.0) ? true : false;

  // String message_string = "0 " + String(sum_normalized_data) + " " + String(threshed_value) + " " + String(standard_dev) + " ;\n";

  if (millis() - time_last_msg > DELAY ){
    digitalWrite(LED_BUILTIN, HIGH);
    String message_string = "#";
    String waterfall_string = "";
    for(int i=0; i < sweep_data_size; i++){
      // server.print(swept_data[i]);
      // server.print(" ");
      // message_string =(swept_data[i] == 0) ? message_string + "0" : message_string + String(swept_data[i]);
      message_string = message_string + String(swept_data[i]);
      message_string = (i < sweep_data_size-1) ?  message_string + " " : message_string;
      // waterfall_string =(swept_data[i] == 0) ? waterfall_string + "0" : waterfall_string + String(swept_data[i]);
      // waterfall_string = ( i > 0 && i < sweep_data_size-1) ? waterfall_string : waterfall_string + ",";
    }
    // Serial.println(message_string);
    message_string = message_string + ";\n";
    Serial.println(message_string);
    server.write(message_string.c_str(), message_string.length());
    time_last_msg = millis();
  }
  // Serial.print("Sum : ");
  // Serial.println(sum_swept_data);
  // Serial.print("Sum normalized sweepts : ");
  // Serial.println(sum_normalized_data);
  // Serial.print("Value 0 : ");
  // Serial.println(swept_data[0]);
  // Serial.print("Emas : ");
  // Serial.println(ema[0]);
  // Serial.print("Emas std : ");
  // Serial.println(ema_std[0]);

  // Serial.println();

  // delay(300);
  // old_sum_normalized = sum_normalized_data;
  digitalWrite(LED_BUILTIN, LOW);


}
