#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <iop_maths_lib.h>


#define BUFFER_SIZE 400
#define LED_PWM_PIN 17
#define MAX_FREQ 300
#define MIN_FREQ 100
#define FREQ_STEP 1
#define ANALOG_PIN 14
#define LED_BUILTIN 2
#define HUMIDITY_PIN 27
#define SD_CS 5
#define nomDuFichier "sound.wav"

File monFichier; // Déclaration d'un objet de type File

const int sweep_data_size = (MAX_FREQ - MIN_FREQ) / FREQ_STEP;
int swept_data[sweep_data_size];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(HUMIDITY_PIN, INPUT);
  SD.begin(SD_CS);

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

void loop(){
  // put your main code here, to run repeatedly:
  
  // Serial.print("Humidity: ");
  // Serial.println(analogRead(HUMIDITY_PIN));
  monFichier = SD.open(nomDuFichier, FILE_READ);
  if (monFichier) {
    Serial.println(F("Affichage du contenu du fichier '" nomDuFichier "', ci-après."));
    Serial.write("-> Texte présent dans le fichier = ");
    while (monFichier.available()) {              // Lecture, jusqu'à ce qu'il n'y ait plus rien à lire
      Serial.write(monFichier.read());            // ... et affichage sur le moniteur série !
    }
    monFichier.close();                           // Fermeture du fichier
    Serial.println(F("Lecture terminée."));
  }
  else {
    Serial.println(F("Échec lors de l'ouverture en lecture du fichier '" nomDuFichier "' sur la carte SD !"));
    while (1);    // Boucle infinie (arrêt du programme)
  }
  Serial.println();

}