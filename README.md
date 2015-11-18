# HapticCompass
My Arduino project, software for a wearable haptic compass.

# Notes
5V regulator may get hot. Will attach external heat sink.
Baseline current is about 60 mA.
All LEDs lit is about 120 mA.
Predict that running Arduino could increase to 200 mA.
When we move from prototyping to real implementation, we'll use a better supply that makes no extra heat.
For now, I can use my 9V supply at home that has 9V battery clips as extensions.


pins
blue wire    12  Data
white wire   11  Serial clock SRCK
yellow wire  10  Not Clear 
green wire   9  Not G
white wire   8  Register clock RCK  
