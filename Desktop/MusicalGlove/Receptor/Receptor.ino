#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

int data[8];
int estado[5];
int velocity = 0x45;
int base = 0x1E;

void setup(void)
{
    pinMode(10, OUTPUT); 
    Serial.begin(9600);
  
    radio.begin();
    radio.setRetries(15,15);
    //radio.setPayloadSize(8);
    radio.startListening();
    //radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);

    radio.startListening();
    for (int i=0;i<5;i++){
      estado[i] = 0;
    }
}

void loop(void)
{
    if ( radio.available() )  // Si hay datos disponibles
    {
      radio.read( &data, sizeof(data) );
      for (int i=0;i<5;i++){
        if((data[i]+estado[i])&&1){
          estado[i] = data[i];
          if (estado[i]){
            noteOn(0x90, 0x1E + i, velocity);
          }else {
            noteOn(0x90, 0x1E + i, 0x00);
          }
        }
      }
      if (data[0] > 120 && base != 0x5A){
        base++;
      }else if(data[0] < -120 && base != 0x1E){
        base--;
      }
      if (data[1] > 120 && velocity != 0xFF){
        velocity++;
      }else if (data[1] < -120 && velocity != 0x00){
        velocity--;
      }
    }
}

void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

