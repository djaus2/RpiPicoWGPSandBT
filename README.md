# RPIPicoWGPSandBT

A set of Arduino Sketches for execution on a RPI Pico W involving GPS and Bluetooth

## Projects

1.  USART1 Echo
    - Echo text from **Serial terminal** _(uppercased)_ back to it after first passing it through **USART1 in loopback mode**.
3.  Basic GPS
    - Read and echo NMEA sentences to **Serial terminal**
5.  IneractiveGPS
    - Can send commands to device from **Serial terminal**:
      - s: Start
      - p: Stop
      - n: No filter on messages
      - n: Filter all _(none displayed)_
      - a: Display GPGGA - Global Positioning System Fix Data
      - l: Display GPGLL - Geographic Position, Latitude / Longitude
      - v: Display GPGSV - Satellites in view
5.  LocationGPS
    - Filter all messages except GPGGA
    - Get lattitude, longitude and height
    - Commands:
    - - s: start
      - p: stop
      - n display none
      - d: Display GPGGA string
      - l: Display as location tupple
      - t: Display as json telemetry



