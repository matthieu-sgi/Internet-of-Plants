#include <Arduino.h>
#include <WiFi.h>

#define BUFFER_SIZE 400
#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 34

const char* ssid = "cisco_ap";
const char* password = "UbuntuSUDOcisco";

const char* host = "10.8.1.5";
const int port = 3000;

WiFiClient server; // TCP server  

const int sweep_data_size = (MAX_FREQ - MIN_FREQ) / FREQ_STEP;
int* swept_data = new int[sweep_data_size];
int** data = new int*[BUFFER_SIZE];



float average(int* data, int size){
  int sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum / size;
}

float average(int** data, int size){
  float sum = 0;
  for(int i = 0; i < size; i++){
    sum += average(data[i], sweep_data_size);
  }
  return sum / size;
}

float average(float* data, int size){
  float sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum / size;
}

float standard_deviation(int** data, int size){
  float avg = average(data, size);
  float sum = 0;
  for(int i = 0; i < size; i++){
    sum += pow(average(data[i], sweep_data_size) - avg, 2);
  }
  return sqrt(sum / size);
}

float standard_deviation(float* data, int size){
  float avg = average(data, size);
  float sum = 0;
  for(int i = 0; i < size; i++){
    sum += pow(data[i] - avg, 2);
  }
  return sqrt(sum / size);
}

float* data_normalizer(int* data, int size, float avg, float std_dev){
  float* normalized_data = new float[size];
  for(int i = 0; i < size; i++){
    normalized_data[i] = (data[i] - avg) / std_dev;
  }
  return normalized_data;
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

  // GPIO setup
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);

  // Initialize data array
  for(int i = 0; i < BUFFER_SIZE; i++){
    data[i] = 0;
  }

  // Initialize PWM
  ledcSetup(0, MIN_FREQ, 2);
  ledcAttachPin(LED_PWM_PIN, 0);  
  ledcWrite(0, 2);

}

int* sweep(){
  int* data = new int[sweep_data_size];
  int value = 0;
  for(int freq = MIN_FREQ; freq <= MAX_FREQ; freq += FREQ_STEP){
    value = analogRead(ANALOG_PIN);
    data[freq] = value;
    ledcChangeFrequency(0, freq*1000, 2);
    delayMicroseconds(10);
  }
  return data;
}

int sum(int* data, int size){
  int sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum;
}


int old_sum = 0;

void loop() {
  // put your main code here, to run repeatedly:

  if(!server.connected()){
    Serial.println("Connection to TCP server lost");
    return;
  }
  swept_data = sweep();
  int sum_swept_data = sum(swept_data, sweep_data_size);
  int diff = sum_swept_data - old_sum;
  add_matrix_line(data, swept_data, BUFFER_SIZE);

  float avg = average(data, BUFFER_SIZE);
  float std_dev = standard_deviation(data, BUFFER_SIZE);

  // Serial.print("Average: ");
  // Serial.print(avg);
  // Serial.print(" Standard deviation: ");
  // Serial.println(std_dev);

  old_sum = sum_swept_data;
  int threshed_value =  (abs(diff) > 90) ? 1 : 0;

  // normalize data
  float* normalized_data = data_normalizer(swept_data, sweep_data_size, avg, std_dev);
  float anorm = standard_deviation(normalized_data, sweep_data_size);

  char* message = new char[100];
  // build string with sum_swept_data, threshed_value, anorm

  sprintf(message, "%d %d %f ;\n", sum_swept_data, threshed_value, anorm);
  Serial.println(message);
  server.print(message);

  delay(1000);


}
