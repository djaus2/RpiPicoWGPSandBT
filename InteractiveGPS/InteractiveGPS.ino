// Interactive GPS with RPI Pico USART1.

// Using Arduino Compatible GPS Receiver Module Jaycar (Australia) CAT.NO:  XC3710
// https://www.jaycar.com.au/arduino-compatible-gps-receiver-module/p/XC3710
// nblox NEO-7M Module  BAUD is 9600
// NMEA Ref: https://www.opencpn.org/wiki/dokuwiki/doku.php?id=opencpn:opencpn_user_manual:advanced_features:nmea_sentences

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

bool run = true;
bool done=false;
char mode ='a';
char nmea[128];
int indx=0;
bool starting = true;

void setup() {
  run = true;
  mode ='d';
  indx=0;
  done=false;
  starting=true;
  nmea[0]=0;
  Serial2.setRX(RXD2);  
  Serial2.setTX(TXD2);
  Serial2.begin(9600);
  Serial.begin(9600);
  delay(1000);
}



void loop() {

  if (Serial.available()) 
  {
    char cmd = Serial.read();

    cmd = tolower(cmd);
    switch (cmd)
    {
      case 's':
        run= true;
        indx=0;
        done=false;
        starting = true;
        Serial.println("Start");
        nmea[0]=0;
        break;
      case 'p':
        run= false;
        Serial.println("Stop");
        break;
      case '\n':
        break;
      case '\r':
        break;
      default:
        mode=cmd;
        Serial.print("Mode: ");
        switch (mode)
        {
          case 'd':
            Serial.println("Display all");
            break;
          case 'n':
            Serial.println("Display none");
            break;
          case 'l':
            Serial.println("Display GLL - Geographic Position, Latitude / Longitude");
            break;
          case 'a':
            Serial.println("Display GGA - Global Positioning System Fix Data");
            break;
          case 'v':
            Serial.println("Display GSV - Satellites in view");
            break;          
          default:
            Serial.println("Not a command!");
            break;
        }
        break;
    }
  }

  if(run)
  {
    if (Serial2.available()) 
    {
      while((Serial2.available()) &&(!done))
      {
        char c1 = Serial2.read();
        if(starting)
        {
          if(c1=='$')
          {
            starting=false;
            indx=0;
            done=false;
            nmea[indx]=c1;
            indx++;
            continue;
          }
          else
          {
            continue;
          }
        }
        else if (c1=='\n')
        {
          done = true;
          break;
        }
        else if (c1=='\r')
        {
          done = true;
          break;
        }
        else
        {
          nmea[indx]=c1;
          indx++;
          if(indx>128)
          {
            Serial.println("Overrun.");
            indx=0;
            done=false;
            nmea[0]=0;
            break;
          }
          nmea[indx]=0;
        }
      } 
      if (done)
      {
        // Expect starting with GP
        if(nmea[1]=='G')
          {
            if(nmea[2]=='P')
            {
            switch (mode)
            {
              case 'd':
                Serial.println(nmea);
                break;
              case 'n':
                break;
              case 'l': // $GPGLL
                if(nmea[5]=='L')
                {
                  if(nmea[4]=='L')
                  {
                    if(nmea[3]=='G')
                    {
                      Serial.println(nmea);
                    }
                  }
                }
                break;
              case 'a': // $GPGGA
                if(nmea[5]=='A')
                {
                  if(nmea[4]=='G')
                  {
                    if(nmea[3]=='G')
                    {
                      Serial.println(nmea);
                    }
                  }
                }
                break;
              case 'v': // $GPGSV
                if(nmea[5]=='V')
                {
                  if(nmea[4]=='S')
                  {
                    if(nmea[3]=='G')
                    {
                      Serial.println(nmea);
                    }
                  }
                }
                break;
              default:
                break;
            }
            starting=true;
            done=false;
          }
        }
      }
    }
  }
}
