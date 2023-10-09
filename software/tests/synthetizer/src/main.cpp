#include <Arduino.h>
#include "../include/sound_hex.h"
#include "AudioTools.h"
// #include "AudioLibs/AudioKit.h"

typedef int16_t sound_t;                                   // sound will be represented as int16_t (with 2 bytes)

AudioInfo info(8000, 1, 8);
GeneratorFromArray<int16_t> array_generator(sound_wav, 0, false);
GeneratedSoundStream<int16_t> sound_stream(array_generator);
AnalogAudioStream out;
StreamCopy copier(out, sound_stream);


void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // start the analog output
  auto config = out.defaultConfig(TX_MODE);
  config.copyFrom(info); 
  out.begin(config);

  // Setup sine wave
  // sineWave.begin(info, N_B4);
  Serial.println("started...");

}

void loop() {
  copier.copy();
}