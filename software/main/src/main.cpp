#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <iop_maths_lib.h>
#include "AudioTools.h"
#include "AudioLibs/AudioSourceSD.h"
#include "AudioCodecs/CodecMP3Helix.h"
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
#define FILENAME "/"


// AudioSourceSD source(FILENAME, ".mp3", SD_CS);
// I2SStream out;
AnalogAudioStream out;
// Maximilian maximilian(out);
MP3DecoderHelix decoder;
EncodedAudioStream encoded(&out, &decoder);
File source;
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = out.defaultConfig(TX_MODE);

  out.begin(cfg);
  SD.begin(SD_CS);
  source = SD.open("/lofi_edit.mp3");


  encoded.begin();

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
void loop(){

  copier.copy();

}