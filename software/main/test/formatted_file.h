#ifndef IOP
#define IOP
#include <cmath>

extern "C"{
float average_custom(float* data, int size);


float standard_deviation(float* data, int size, float avg);

void data_normalizer(int* data, int size, float* ema, float* ema_std, float* normalized_data);

float sum_custom(float* data, int size);

void ema_update(float* ema, int* data, int size);

void ema_std_update(int* data, int size,float* ema_std, float* ema);

}

#endif