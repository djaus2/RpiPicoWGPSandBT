// Basic GPS with RPI Pico USART1.

// Using Arduino Compatible GPS Receiver Module Jaycar (Australia) CAT.NO:  XC3710
// https://www.jaycar.com.au/arduino-compatible-gps-receiver-module/p/XC3710
// nblox NEO-7M Module  BAUD is 9600

// // Nb: GPn pin numbers not package pin numbers
#define TXD2 4  // Not used
#define RXD2 5  // Connect to GPS Unit TX

// Pins Ceramic Antenna side:
// ==========================
// PPS nc
// RXD nc
// TXD --> RXD2
// GND --> GND
// Vcc --> 3.3V

// Note: USART1 is Serial2
// RPI Pico has has 2 USART, USART0 is Serial/Serial1/USB

void setup() {
  Serial2.setRX(RXD2);  
  Serial2.setTX(TXD2);
  Serial2.begin(9600);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  if (Serial2.available()) 
  {
    while(Serial2.available()) 
    {
      char c1 = Serial2.read(); 
      Serial.write(c1);
      /*while(!Serial2.available())
      {}
      while(Serial2.available())
      {
        Serial.write(toupper(Serial2.read()));
      }*/
    }
    //Serial.write("\n");
  }
}
