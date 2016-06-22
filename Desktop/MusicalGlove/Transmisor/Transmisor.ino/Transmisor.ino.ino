#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_L3GD20.h>
#include "nRF24L01.h"
#include "RF24.h"

#define DEBUG 0
#define USE_I2C

#ifdef USE_I2C
  // The default constructor uses I2C
  Adafruit_L3GD20 gyro;
#else
  // To use SPI, you have to define the pins
  #define GYRO_CS 4 // labeled CS
  #define GYRO_DO 5 // labeled SA0
  #define GYRO_DI 6  // labeled SDA
  #define GYRO_CLK 7 // labeled SCL
  Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);
#endif

RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
int data[8];

void setup(void)
   { 
     pinMode(10, OUTPUT);
     pinMode(2, OUTPUT);
     pinMode(3, OUTPUT);
     pinMode(4, OUTPUT);
     pinMode(5, OUTPUT);
     pinMode(6, OUTPUT);
     Serial.begin(9600);
     gyro.begin(gyro.L3DS20_RANGE_250DPS);
     radio.begin();

     radio.setRetries(15,15);      // Maximos reintentos
     //radio.setPayloadSize(8);    // Reduce el payload de 32 si tienes problemas
     radio.openWritingPipe(pipes[0]);
     //radio.openReadingPipe(1,pipes[1]);
     radio.stopListening();
   }

void loop(void)
   { 
      gyro.read();
      data[0] = (int)gyro.data.x;
      data[1] = (int)gyro.data.y;
      data[2] = (int)gyro.data.z;
      data[3] = (int)digitalRead(2);
      data[4] = (int)digitalRead(3);
      data[5] = (int)digitalRead(4);
      data[6] = (int)digitalRead(5);
      data[7] = (int)digitalRead(6);
      if (DEBUG){
         Serial.print("Enviando  ");
      }
      bool ok = radio.write( &data, sizeof(data) );
      if (DEBUG){
        if (ok)
             Serial.println("ok...");
        else
             Serial.println("failed");
      }
      
      delay(500);
   }
