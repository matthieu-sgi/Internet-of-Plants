#include <Arduino.h>
#include <WiFi.h>

#define BUFFER_SIZE 400
#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 34

const char* ssid = "4D-Space";
const char* password = "CestPasRFIci42";

const char* host = "10.1.65.13";
const int port = 13000;

WiFiClient server; // TCP server  

const float alpha = 0.95;
const int sweep_data_size = (MAX_FREQ - MIN_FREQ) / FREQ_STEP;
int old_sum = 0;
// float new_standard_dev = 0.0;

float old_standard_dev = 0.0;

int swept_data[sweep_data_size];
float ema[sweep_data_size];
float ema_std[sweep_data_size];
float old_sum_normalized;
float normalized_data[sweep_data_size];


float average(int* data, int size){
  int sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum / size;
}

// float average(int** data, int size){
//   float sum = 0;
//   for(int i = 0; i < size; i++){
//     sum += average(data[i], sweep_data_size);
//   }
//   return sum / size;
// }

float average(float* data, int size){
  float sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum / size;
}


float standard_deviation(float* data, int size, float avg){
  float sum = 0;
  for(int i = 0; i < size; i++){
    sum += pow(data[i] - avg, 2);
  }
  return sqrt(sum / size);
}

void data_normalizer(int* data, int size, float* ema, float* ema_std, float* normalized_data){
  
  for(int i = 0; i < size; i++){
    normalized_data[i] = ((float)(data[i]) - ema[i]) / ema_std[i];
  }
}


void add_matrix_line(int** matrix, int* line, int size){
  for(int i = 0; i < size - 1; i++){
    matrix[i] = matrix[i + 1];
  }
  matrix[size - 1] = line;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Wifi connection setup

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to WiFi network");

  // Connect to TCP server
  if(!server.connect(host, port)){
    Serial.println("Connection to TCP server failed");
    return;
  }

  Serial.println("Connected to TCP server");

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

}

void sweep(int* data, int size){
  int value = 0;
  for(int index = 0; index <= size; index += FREQ_STEP){
    value = analogRead(ANALOG_PIN);
    data[index] = value;
    int freq = MIN_FREQ + index * FREQ_STEP;
    ledcChangeFrequency(0, freq*1000, 2);
    delayMicroseconds(10);
  }
}

int sum(int* data, int size){
  int sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum;
}

float sum(float* data, int size){
  float sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum;
}


void ema_update(float* ema, int* data, int size){

  for(int i = 0; i<size; i++){
    ema[i] = ema[i] * alpha + (1-alpha)*data[i];
  }


}

void ema_std_update(int* data, int size,float* ema_std, float* ema){



  for (int i = 0; i < size; i++){
    ema_std[i]= alpha * ema[i] + (data[i]-ema[i]) *(1-alpha);
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
  sweep(swept_data, sweep_data_size);
  // new_average = average(swept_data, sweep_data_size);
  ema_update(ema, swept_data, sweep_data_size);
  ema_std_update(swept_data, sweep_data_size, ema_std, ema);

  data_normalizer(swept_data, sweep_data_size, ema, ema_std, normalized_data);

  float avg = average(normalized_data, sweep_data_size);
  float standard_dev = standard_deviation(normalized_data, sweep_data_size,avg);

  float sum_swept_data = sum(normalized_data, sweep_data_size);
  float diff = abs(sum_swept_data - old_sum_normalized);
  int threshed_value =  (diff > 90.0) ? 1 : 0;
  Serial.print("Sum: ");
  Serial.println(sum_swept_data);
  Serial.print("Diff: ");
  Serial.println(diff);

  // normalize data
  /*

  float anorm = standard_deviation(normalized_data, sweep_data_size);
*/


  char* message = new char[100];
  sprintf(message, "%d %d %f ;\n", sum_swept_data, threshed_value, standard_dev);
  Serial.println(message);
  int written = server.print(message);
  Serial.print("Written: ");
  Serial.println(written);

  old_sum_normalized = sum_swept_data;
  delay(1000);


}
