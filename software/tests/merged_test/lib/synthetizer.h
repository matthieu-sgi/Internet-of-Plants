#ifndef SYNTHETIZER_H
#define SYNTHETIZER_H

#include "includes.h"
#include "../include/sound_hex.h"




AudioInfo info2(8000, 1, 16);
int16_t sine_array[] = {0, 4560, 9031, 13327, 17363, 21062, 24350, 27165, 29450, 31163, 32269, 32747, 32587, 31793, 30381, 28377, 25820, 22761, 19259, 15383, 11206, 6812, 2285, -2285, -6812, -11206, -15383, -19259, -22761, -25820, -28377, -30381, -31793, -32587, -32747, -32269, -31163, -29450, -27165, -24350, -21062, -17363, -13327, -9031, -4560  };
GeneratorFromArray<int16_t> sineWave(sine_array,0,false);
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
AnalogAudioStream out; 
StreamCopy copier(out, sound);                             // copies sound into i2



void synthetizer_setup(HardwareSerial synthe_serial);
void synthetizer_loop();


#endif