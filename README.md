# HapticCompass
My Arduino project, software for a wearable haptic compass.

# North Paw
The open-source project that inspired this project. This project is put together from what's at hand, instead of reproducing the North Paw's specifications. Check out their jam at http://sensebridge.net/projects/northpaw/

# bunny.ino
Some test code by Daniel. An inclinometer program that graphs incilation in the long axis of the prototype board on the LED bar. Forked from https://github.com/adafruit/Adafruit_BNO055

# teapot.ino
Some test code by David. A fork of bunny.ino circa 2015/NOV/23. A test area for early development. Goal: graph magnetic heading

# Notes
Baseline current is about 60 mA.
All LEDs lit is about 120 mA.
Predict that running Arduino could increase to 200 mA.
When we move from prototyping to real implementation, we'll use a better supply that makes no extra heat.
For now, I can use my 9V supply at home or use USB power.

# Pin usage
blue wire    12  Data
white wire   11  Serial clock SRCK
yellow wire  10  Not Clear 
green wire    9  Not G
white wire    8  Register clock RCK  
