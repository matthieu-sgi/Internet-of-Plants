#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <iop_maths_lib.h>
#include "AudioTools.h"
#include "AudioLibs/AudioSourceSD.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioCodecs/CodecWAV.h"
#include "AudioLibs/FileLoop.h"
// #include "AudioLibs/MaximilianDSP.h"


#define BUFFER_SIZE 400
#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 14
#define LED_BUILTIN 2
#define HUMIDITY_PIN 27
#define SD_CS 5
#define FILENAME "/sound.wav"


// AudioSourceSD source(FILENAME, ".mp3", SD_CS);
// I2SStream out;
AudioInfo info(44100, 1, 16);
AnalogAudioStream out;
// Maximilian maximilian(out);
// MP3DecoderHelix decoder;
WAVDecoder decoder;

FileLoop source;
// AudioEffectStream effects(out);
// VolumeStream vol(out);
// PitchShift pitchShift(0.5, 256);
EncodedAudioStream encoded(&out, &decoder);
// PitchShiftOutput<int16_t, VariableSpeedRingBufferSimple<int16_t>> pitchShift(out);
StreamCopy copier(encoded, source);
// AudioPlayer player(source,out, decoder);


void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}


const int sweep_data_size = (MAX_FREQ - MIN_FREQ) / FREQ_STEP;
int swept_data[sweep_data_size];

auto tremolo = new Tremolo(200,0.5,44100);
auto pitch_shift = new PitchShift(3, 1024);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // effects.addEffect(dly);
  // setup output
  // effects.addEffect(tremolo);
  // effects.addEffect(new Distortion(4000));
  // effects.addEffect(pitch_shift);
  audio_tools::AnalogConfigESP32 cfg = out.defaultConfig(TX_MODE);
  
  cfg.sample_rate = 8000;
  cfg.channels = 2;
  cfg.bits_per_sample = 16;
  // cfg.bitrate = 256;
  // vol.begin();
  // vol.setVolume(0.5);
  // auto cfg = pitchShift.defaultConfig();
  // cfg.pitch_shift = 1.5;
  // cfg.buffer_size = 1024;
  // pitchShift.begin(cfg);
  SD.begin(SD_CS);
  source.setFile(SD.open("/music.wav"));
  // source = SD.open("/lofi_edit.mp3");



  // effects.begin(cfg);
  encoded.begin();
  out.begin(cfg);
  // effects.begin(info);
  
}

void sweep(int* data, int size){
  int value = 0;
  for(int index = 0; index < size; index += FREQ_STEP){
    value = analogRead(ANALOG_PIN);
    data[index] = value;
    int freq = MIN_FREQ + index * FREQ_STEP;
    ledcChangeFrequency(0, freq*1000, 2);
    delayMicroseconds(10);
  }
}

// Get time in milliseconds
unsigned long timing = millis();
bool played = true;
float volume = 0.1;
float counter = 1.0;
int max_humidity = 2500;
int min_humidity = 1850;
void loop(){

   if(millis() - timing > 1000){
    timing = millis();
    // Serial.println("1 second");
    // Serial.println(volume);
    // // volume =(volume < 1) ? volume + 0.1 : 0.1;

    // // vol.setVolume(volume);
    // Serial.println(volume);
    // Serial.println("1 second");
    //CHANGE TREMOLO
    // counter = (counter < 1000) ? counter + 500 : 500;
    counter+= 0.5;
    // Serial.println(counter);
    // pitch_shift->setValue(counter);
    // tremolo->setDuration(counter);
   }
  int read = analogRead(HUMIDITY_PIN);
  Serial.print("Humidity: ");
  Serial.println(read);
  int value = map(read, min_humidity, max_humidity, 1, 100);
  float input_value = ((float)(value))/10;
  Serial.print("Input value: ");
  input_value = (input_value <= 1) ? 1 : input_value;
  Serial.println(input_value);
  // pitch_shift->setValue(input_value);
  if (!copier.copy()){

  }

}