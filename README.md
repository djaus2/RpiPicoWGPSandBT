# RPI Pico W GPS and Bluetooth

A set of Arduino Sketches for execution on a RPI Pico W  building up to and including sending GPS location data over Bluetooth in BT Classic mode. Also send GPS data to an Azure IoT Hub.

<hr/>

### Update/Correction

> The calculation of Latitude and Longitude from NMEA data has been corrected ina a number of projects as per my blog post :[GPS: NMEA 101](https://davidjones.sportronics.com.au/web/GPS-NMEA_101-web.html).

<hr/>

## Blog Post

[RPI-Pico-Arduino-AzSDK: Bluetooth and GPS Update](https://davidjones.sportronics.com.au/ardpico/RPI-Pico-Arduino-AzSDK-BlueTooth_Setup_Update-ardpico.html)

## Hardware

### Bluetooth
The RPI PicoW comes with Bluetooth. The BSP used has been updated to include that.
- The repository for the Pico W Arduino BSP: [earlephilhower-arduino-pico on Github](https://github.com/earlephilhower/arduino-pico)
- Installation [instructions](https://davidjones.sportronics.com.au/ardpico/RPI-Pico-Arduino-AzSDK-PicoWSetup-pic-ard.html#how-to-install-arduino-bsp-onto-a-rpi-pico)
- [Bluetooth documentation for the BSP](https://arduino-pico.readthedocs.io/en/latest/bluetooth.html)

### Arduino GPS Receiver
  - Arduino Compatible GPS Receiver Module Jaycar (Australia) CAT.NO:  XC3710
  - https://www.jaycar.com.au/arduino-compatible-gps-receiver-module/p/XC3710
  - nblox NEO-7M Module  BAUD is 9600

## Projects

1.**USART1 Echo**

  - Echo text from **Serial terminal** _(uppercased)_ back to it after first passing it through **USART1 in loopback mode**.

2.**Basic GPS**

  - Read and echo NMEA sentences to **Serial terminal**

3.**InteractiveGPS**

  - Can send commands to device from **Serial terminal**:
    - s: Start
    - p: Stop
    - n: No filter on messages
    - n: Filter all _(none displayed)_
    - a: Display GPGGA - Global Positioning System Fix Data
    - l: Display GPGLL - Geographic Position, Latitude / Longitude
    - v: Display GPGSV - Satellites in view

4.**LocationGPS**

  - Filter all messages except GPGGA
  - Get latitude, longitude and height
  - _Note that string processing of NMEA messages is explicit rather than using existing libraries._
  - Commands:
  - - s: start
    - p: stop
    - n display none
    - d: Display GPGGA string
    - l: Display as location tupple
    - t: Display as json telemetry
  - Has been modified for Sketch 7.
    - Parsing of NMEA in "reusable" function.

5.**BTSerialUppercase**

  - Receive text over Bluetooth and echo it back after upper-casing it.
  - Is the Android IDE **SerialBT** _BTSerialUppercase_ example  
  - Added echo to IDE Terminal of the characters being passed, unadulterated.
  - **_Note:_** Bluetooth Classic, not BLE.
  - Found the [Android phone app Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) was easier to pair than desktop.

6.**BluetoothGPS**

  - **<font color="blue"><bold>**Send Location data over Bluetooth to paired host.**</bold></font>**
  - As per 4 but data and commands over Bluetooth _(Merger of 4. and 5.)_
  - Receives commands over BT as per **_BTSerialUppercase_** and interprets them as per **_LocationGPS_**.
    - That is, sends Location data back over BT when available as per **_LocationGPS_** _(to SerialBT instead of Serial)_ 
  - **_Note:_** Bluetooth Classic, not BLE.

7.**Azure_IoT_Hub_GPS**

  - Send GPS location data, generated as per **LocationGPS**, as Telemetry to an Azure IoT Hub. 
    - _Minus interactive commands._
  - Filter all messages except **GPGGA**
Get latitude, longitude and height
  - From **LocationGPS** uses json location option.
      - That is, all creation of the telemetry string explicitly uses string processing rather than GPS and json classes.
  - Now works.

8. **read-d2c-messages**

  - .NET Console app to monitor messages sent to IoT Hub by 7.
  - Uses .NET Core 3.1
  - See code for connection details.
  
  > Nb: Was found that in **Azure IoT Explorer**, needed to paste same connection string as the Custom Endpoint for the device. Built-in [Yes] option did not work.
  
9. **read-d2c-messages-Net6**

  - As per 8. but uses .NET 6.0
  - See code for connection details.

<hr />

## Footnote

Working on a Blazor app to display the GPS coordinates sent by 8. on a map.
  


