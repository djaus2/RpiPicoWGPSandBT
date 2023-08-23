// Echo from Terminal through RPI Pico USART1.

#include <SerialBT.h>
#define TXD2 4  // Nb: GPn pin numbers not package pin numbers
#define RXD2 5

void setup() {
  Serial2.setRX(RXD2);  //Jumper package pins 6&7
  Serial2.setTX(TXD2);
  Serial2.begin(9600);
  Serial.begin(9600);
  delay(1000);
  while(!Serial){};
  Serial.println("Started 1");
  while(!Serial2){};
  Serial.println("Started 2");
}

void loop() {
  if (Serial.available()) 
  {
    
    Serial2.write(Serial.read());
  /*
    if (!Serial2.available()) 
    {
    }
    while(Serial2.available()) 
    {
      char c1 = Serial2.read(); 
      Serial.write(c1);
    };*/
  }
  if (Serial2.available()) 
  {
    
    Serial.write(Serial2.read());
  /*
    if (!Serial2.available()) 
    {
    }
    while(Serial2.available()) 
    {
      char c1 = Serial2.read(); 
      Serial.write(c1);
    };*/
  }
}
