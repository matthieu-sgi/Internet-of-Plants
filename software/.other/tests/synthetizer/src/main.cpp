#include <Arduino.h>
// #include "../include/sound_hex.h"
// #include "AudioTools.h"
// // #include "AudioLibs/AudioKit.h"

// #define PWM_PIN 12

// typedef int16_t sound_t;                                   // sound will be represented as int16_t (with 2 bytes)

// AudioInfo info(44000, 1, 16);
// sound_t sine_array[] = {0, 4560, 9031, 13327, 17363, 21062, 24350, 27165, 29450, 31163, 32269, 32747, 32587, 31793, 30381, 28377, 25820, 22761, 19259, 15383, 11206, 6812, 2285, -2285, -6812, -11206, -15383, -19259, -22761, -25820, -28377, -30381, -31793, -32587, -32747, -32269, -31163, -29450, -27165, -24350, -21062, -17363, -13327, -9031, -4560  };
// GeneratorFromArray<sound_t> array_generator(sine_array, 0, false);

// GeneratedSoundStream<sound_t> sound_stream(array_generator);
// AnalogAudioStream out;
// StreamCopy copier(out, sound_stream);


// void setup() {
//   Serial.begin(115200);
//   AudioLogger::instance().begin(Serial, AudioLogger::Debug);
//   pinMode(PWM_PIN, OUTPUT);
//   // start the analog output
//   auto config = out.defaultConfig(TX_MODE);
//   config.copyFrom(info); 
//   out.begin(config);

//   // Setup sine wave
//   // sineWave.begin(info, N_B4);
//   Serial.println("started...");
//   sound_stream.begin(info);

//   Serial.println(sizeof(sound_wav) / sizeof(sound_wav[0]));

// }

// // int counter = 0;
// void loop() {
  
//   copier.copy();
//   analogWrite(PWM_PIN, 100);
//   // counter = (counter + 20) % 255;
//   // Serial.println(counter);
// }

/**
 * @file streams-generator_fromarray-analog.ino
 * @author Phil Schatzmann
 * @brief Example for GeneratorFromArray
 * @version 0.1
 * @date 2022-03-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 #include "AudioTools.h"
#include "../include/sound_hex.h"

//  AudioInfo info(8000, 1, 16);
 AudioInfo info2(8000, 1, 16);
 int16_t sine_array[] = {0, 4560, 9031, 13327, 17363, 21062, 24350, 27165, 29450, 31163, 32269, 32747, 32587, 31793, 30381, 28377, 25820, 22761, 19259, 15383, 11206, 6812, 2285, -2285, -6812, -11206, -15383, -19259, -22761, -25820, -28377, -30381, -31793, -32587, -32747, -32269, -31163, -29450, -27165, -24350, -21062, -17363, -13327, -9031, -4560  };
 GeneratorFromArray<int16_t> sineWave(sine_array,0,false);
 GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
 AnalogAudioStream out; 
 StreamCopy copier(out, sound);                             // copies sound into i2

 FormatConverterStream converter(sound);
 // Arduino Setup
 void setup(void) {  
   // Open Serial 
   Serial.begin(115200);
   AudioLogger::instance().begin(Serial, AudioLogger::Warning);
   // start I2S
   Serial.println("starting I2S...");
   auto config = out.defaultConfig(TX_MODE);
   config.copyFrom(info2); 
   out.begin(config);
  //  converter.begin(info, info2);
   // Setup sine wave
   sineWave.begin(info2);
   Serial.println("started...");
 }
 // Arduino loop - copy sound to out 
 void loop() {
   copier.copy();
 }