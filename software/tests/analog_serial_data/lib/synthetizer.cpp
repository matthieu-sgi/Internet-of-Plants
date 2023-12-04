#include "synthetizer.h"


void synthetizer_setup(HardwareSerial synthe_serial){

    AudioLogger::instance().begin(synthe_serial, AudioLogger::Warning);
   // start I2S
   synthe_serial.println("starting I2S...");
   auto config = out.defaultConfig(TX_MODE);
   config.copyFrom(info2); 
   out.begin(config);
   // Setup sine wave
   sineWave.begin(info2);

}

void synthetizer_loop(){
    copier.copy();
}