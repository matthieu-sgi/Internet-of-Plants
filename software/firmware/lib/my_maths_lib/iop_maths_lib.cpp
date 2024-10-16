// #include <Arduino.h>
// #include <WiFi.h>
#include "iop_maths_lib.h"

#define BUFFER_SIZE 400
#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 34

extern "C"{

// WiFiClient server; // TCP server  

// const float alpha = 0.95;
// const int sweep_data_size = (MAX_FREQ - MIN_FREQ) / FREQ_STEP;
// int old_sum = 0;
// // float new_standard_dev = 0.0;

// float old_standard_dev = 0.0;

// int swept_data[sweep_data_size];
// float ema[sweep_data_size];
// float ema_std[sweep_data_size];
// float old_sum_normalized;
// float normalized_data[sweep_data_size];


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

int int_sum(int* data, int size){
  int sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum;
}



float float_sum(float* data, int size){
  float sum = 0;
  for(int i = 0; i < size; i++){
    sum += data[i];
  }
  return sum;
}

void ema_update(float* ema, int* data, int size, float alpha){
  // float alpha = 0.95;
  for(int i = 0; i<size; i++){
    ema[i] = ema[i] * alpha + (1-alpha)*data[i];
  }
  // return ema;

}

void ema_std_update(int* data, int size,float* ema_std, float* ema, float alpha){



  for (int i = 0; i < size; i++){
    ema_std[i]= alpha * ema[i] + (data[i]-ema[i]) *(1-alpha);
  }




}






}