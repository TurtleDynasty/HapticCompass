#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3-5V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/


#define BNO055_SAMPLERATE_DELAY_MS (100) // Sample period (controls loop frequency)

// Define the shift register control lines.
#define RCLK    8
#define NOT_G   9
#define NOT_CLR 10
#define SCLK    11
#define DATA    12


Adafruit_BNO055 bno = Adafruit_BNO055(55);
int counter=0;

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}




void blink13()
{
  digitalWrite( 13, HIGH );
  delay(100);
  digitalWrite( 13, LOW );
  delay(100);
}





void PulseRegisterClock()
{
  digitalWrite( RCLK, HIGH );
  digitalWrite( RCLK, LOW );
}




void PulseSerialClock()
{
  digitalWrite( SCLK, HIGH );
  digitalWrite( SCLK, LOW );
}




void ClearShiftRegister()
{
  // Clear the shift register
  digitalWrite( NOT_CLR, LOW );
  digitalWrite( NOT_CLR, HIGH );
}




void WriteToShiftRegister( float fValue )
{
  char  value;
  bool  dataLine = LOW;
  int   i;


  ClearShiftRegister();
  value = char(abs(floor(fValue/10)));
  //Serial.println((int)value);
  
  // Use the shift register LEDs to indicate inclination.
  for( i=0; i<8; i++ )
  {
    digitalWrite( DATA, (i==value) );
    PulseRegisterClock();
    PulseSerialClock();
    digitalWrite( DATA, LOW );  
  }
}





/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{  
  pinMode( 13, OUTPUT );
  delay( 500 );
  
  blink13();
  blink13();

  // Initialize the shift register control lines.
  pinMode( NOT_G, OUTPUT);
  pinMode( RCLK, OUTPUT );
  pinMode( NOT_CLR, OUTPUT );
  pinMode( SCLK, OUTPUT );
  pinMode( DATA, OUTPUT );
  digitalWrite( NOT_CLR, HIGH );
  digitalWrite( SCLK, LOW );
  digitalWrite( RCLK, LOW );
  digitalWrite( DATA, LOW );
  digitalWrite( NOT_G, LOW );
  delay(500);

  ClearShiftRegister();
  
  Serial.begin(115200);
  Serial.println("----Orientation Sensor Test----"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    do
    {
      Serial.print("*** Ooops, no BNO055 detected ... Check your wiring or I2C ADDR! ***");
      delay(1000);
    }
    while(1);
  }

  delay( 500 );

  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  blink13();
  blink13();
  blink13();

  delay( 500 );
}




/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);
  

  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */
  
  /* The processing sketch expects data as roll, pitch, heading */
  //Serial.print(F("Orientation: "));
  //Serial.print((float)event.orientation.x);
  //Serial.print(F(" "));
  //Serial.print((float)event.orientation.y); // Long axis of dev board
  //Serial.print(F(" "));
  //Serial.print((float)event.orientation.z);
  //Serial.println(F(""));

  // Write inclination data to shift register.
  WriteToShiftRegister( (float)event.orientation.y );

  /* Also send calibration data for each sensor. */
//  uint8_t sys, gyro, accel, mag = 0;
//  bno.getCalibration(&sys, &gyro, &accel, &mag);
//  Serial.print(F("Calibration: "));
//  Serial.print(sys, DEC);
//  Serial.print(F(" "));
//  Serial.print(gyro, DEC);
//  Serial.print(F(" "));
//  Serial.print(accel, DEC);
//  Serial.print(F(" "));
//  Serial.println(mag, DEC);

  delay(BNO055_SAMPLERATE_DELAY_MS);
  counter += BNO055_SAMPLERATE_DELAY_MS;

  if( counter <= 500 )
    digitalWrite( 13, HIGH );
  else 
    digitalWrite( 13, LOW );

  if( counter >= 1000 )
    counter = 0;
}
