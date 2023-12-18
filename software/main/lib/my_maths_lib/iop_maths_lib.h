#ifndef IOP
#define IOP
#include <cmath>

extern "C"{

float average(float* data, int size);

float standard_deviation(float* data, int size, float avg);

void data_normalizer(int* data, int size, float* ema, float* ema_std, float* normalized_data);

float float_sum(float* data, int size);
int int_sum(int* data, int size);

// Problem on ema update
void ema_update(float* ema, int* data, int size, float alpha);

void ema_std_update(int* data, int size,float* ema_std, float* ema, float alpha);

}

#endif