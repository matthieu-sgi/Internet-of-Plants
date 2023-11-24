// #include <Arduino.h>
// #include <WiFi.h>
#include "formatted_file.h"

#define BUFFER_SIZE 400
#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 34

extern "C"{

const char* ssid = "4D-Space";
const char* password = "CestPasRFIci42";

const char* host = "10.1.65.13";
const int port = 13000;

// WiFiClient server; // TCP server  

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


// float average(int* data, int size){
//   int sum = 0;
//   for(int i = 0; i < size; i++){
//     sum += data[i];
//   }
//   return sum / size;
// }

// float average(int** data, int size){
//   float sum = 0;
//   for(int i = 0; i < size; i++){
//     sum += average(data[i], sweep_data_size);
//   }
//   return sum / size;
// }

float average_custom(float* data, int size){
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



float sum_custom(float* data, int size){
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






}