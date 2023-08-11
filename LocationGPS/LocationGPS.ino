// Interpretive GPS with RPI Pico USART1.

// Filter all but GPGGA
//============================
// Get lattitude, longitude and height
// s: start
// p: stop
// n display none
// d: splay GPGGA string
// l: display as location tupple
// t: display as Json tupple

// Using Arduino Compatible GPS Receiver Module Jaycar (Australia) CAT.NO:  XC3710
// https://www.jaycar.com.au/arduino-compatible-gps-receiver-module/p/XC3710
// nblox NEO-7M Module  BAUD is 9600
// NMEA Ref: https://shadyelectronics.com/gps-nmea-sentence-structure/

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

// Locations in GPGGA of entities
#define lattIndex 2
#define longIndex 4
#define heightIndex 9

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

#define bufferIndexMax  12
String strings[bufferIndexMax];

// string: string to parse
// c: delimiter
// returns number of items parsed
void split(String string)
{
  String data = "";
  int bufferIndex = 0;

  for (int i = 0; i < string.length(); ++i)
  {
    char c = string[i];
    
    if (c != ',')
    {
      data += c;
    }
    else
    {
      strings[bufferIndex++] = data;
      data = "";
      if (bufferIndex>bufferIndexMax)
      {
        return;
      }
    }
  }
}

// NMEA latt,long values ar 100x so just shift decimal point left 2 in the string
// Could convert to double then divide by 100 but want as string
String ShiftLeft2(String num)
{
  for (int i = 0; i < num.length(); ++i)
  {
    char c = num[i];
    
    if (c == '.')
    {
        num[i] = num[i-1];
        num[i-1] =num[i-2];
        num[i-2] = '.';
        return num;
    }
  }
  return "Error";
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
        char prevMode = mode;
        mode=cmd;
        Serial.print("Mode: ");
        switch (mode)
        {
          case 'd':
            Serial.println("Display GLL - Geographic Position, Latitude / Longitude");
            break;
          case 'n':
            Serial.println("Display none");
            break;
          case 'l':
            Serial.println("Display Location as a tuple");
            break;
          case 't':
            Serial.println("Display Location as Json Telemetry string");
            break;
          default:
            Serial.println();
            mode=prevMode;
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
            // $GPGGA
            if(nmea[5]=='A')
            {
              if(nmea[4]=='G')
              {
                if(nmea[3]=='G')
                {
                  switch (mode)
                  {
                    case 'd':
                      Serial.println(nmea);
                      break;
                    case 'n':
                      break;
                    case 'l': // Location
                      split(nmea);
                      Serial.print("(");
                      Serial.print(ShiftLeft2(strings[lattIndex]));
                      Serial.print(strings[lattIndex+1]);
                      Serial.print(",");
                      Serial.print(ShiftLeft2(strings[longIndex]));
                      Serial.print(strings[longIndex+1]);
                      Serial.print(",");
                      Serial.print(strings[heightIndex]);
                      Serial.print(strings[heightIndex+1]);
                      Serial.println(")");
                      break;
                    case 't': // Telemetry
                      split(nmea);
                      Serial.print("{\"geolocation\":{");
                      Serial.print("\"lat\":");
                      if(strings[lattIndex+1]=="S")
                      {
                        Serial.print("-");
                      }
                      Serial.print(ShiftLeft2(strings[lattIndex]));
                      Serial.print(",");
                      Serial.print("\"long\":");
                      if(strings[longIndex+1]=="W")
                      {
                        Serial.print("-");
                      }
                      Serial.print(ShiftLeft2(strings[longIndex]));
                      Serial.print(",");
                      Serial.print("\"alt\":");
                      Serial.print(strings[heightIndex]);
                      Serial.print(strings[heightIndex+1]);
                      Serial.println("}}");
                      break;
                    default:
                      break;
                  }
                }
              }
            }
          }
        }
        starting=true;
        done=false;
      }
    }
  }
}
