#ifndef PLANT_SENSOR_H
#define PLANT_SENSOR_H

#include <Arduino.h>

#define LED_PWM_PIN 32
#define MAX_FREQ 3000
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 27
#define POWER_3V 13
#define MOISTURE_PIN 25
#define TOGGLE_PIN 33


bool isOn = true;

const int esp = 10;

int avg_error = 0;

void plant_sensor_setup();
void plant_sensor_loop();
double read_voltage(int pin);
void toggle();

#endif