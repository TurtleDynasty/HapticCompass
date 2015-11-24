#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/*  Teapot is a test area for early development on the Wearable Haptic Compass 
    project by David Miller and Daniel Greenspan. It was forked from bunny.ino 
    by Daniel Greenspan, forked from https://github.com/adafruit/Adafruit_BNO055
 *  
 *  History
 *  =======
 *  2015/NOV/23  - Fork bunny.ino from Daniel Greenspan
 */
 
/*  This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
    which provides a common 'type' for sensor data and some helper functions.

    To use this driver you will also need to download the Adafruit_Sensor
    library and include it in your libraries folder.

    You should also assign a unique ID to this sensor for use with the Adafruit 
    Sensor API so that you can identify this particular sensor in any data logs, etc.  
    To assign a unique ID, simply provide an appropriate value in the constructor 
    below (12345 is used by default in this example).

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

/*  Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
 */
void displaySensorDetails(void) {
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


void blink13() {
  digitalWrite( 13, HIGH );
  delay(200);
  digitalWrite( 13, LOW );
  delay(200);
}


void PulseRegisterClock() {
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}


void PulseSerialClock() {
  digitalWrite(SCLK, HIGH);
  digitalWrite(SCLK, LOW);
}


void ClearShiftRegister() {
  // Clear the shift register
  digitalWrite(NOT_CLR, LOW);
  digitalWrite(NOT_CLR, HIGH);
}


void WriteFloatToShiftRegister(float fValue) {
  char  value;
  bool  dataLine = LOW;  // unused?
  int   i;

  ClearShiftRegister();
  value = char(abs(floor(fValue/10)));
  //Serial.println((int)value);
  
  // Use the shift register LEDs to indicate the value.
  for(i=0; i<8; i++)
  {
    digitalWrite(DATA, (i==value));
    PulseRegisterClock();
    PulseSerialClock();
    digitalWrite(DATA, LOW);  
  }
}


void WriteByteToShiftRegister(byte bValue){
  int i;
  char value = char(bValue);
  
  ClearShiftRegister();

  for( i=1; i<255; i*=2 ){
    digitalWrite(DATA, (i==value));
    PulseRegisterClock();
    PulseSerialClock();
    digitalWrite(DATA, LOW);
  }
}

/* float CalculateCompassDegrees(){

  return 0;
}*/




/* Arduino setup function (automatically called at startup)
 */

void setup(void) {  
  pinMode(13, OUTPUT);
  delay(500);

  // Signal user - two blinks - setup started
  blink13();
  blink13();

  // Initialize the shift register control lines.
  pinMode(NOT_G, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(NOT_CLR, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  digitalWrite(NOT_CLR, HIGH);
  digitalWrite(SCLK, LOW);
  digitalWrite(RCLK, LOW);
  digitalWrite(DATA, LOW);
  digitalWrite(NOT_G, LOW);
  delay(500);

  ClearShiftRegister();
  
  Serial.begin(115200);
  Serial.println("----Orientation Sensor Test----"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin()) {
    do {
      Serial.print("*** Ooops, no BNO055 detected ... Check your wiring or I2C ADDR! ***");
      delay(1000);
    }
    while(1);
  }

  delay( 500 );

  // Display some basic information on this sensor
  displaySensorDetails();

  // Signal user - three blinks - sensor test OK
  blink13();
  blink13();
  blink13();

  delay(500);
}



/*  Display sensor calibration status
 *  Forked from adafruit calibration tutorial
 */
void displayCalStatus(void){
  //Get the four calibration values (0..3)
  //Any sensor data reporting 0 should be ignored,
  //3 means 'fully calibrated"
  
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
 
  // The data should be ignored until the system calibration is > 0
  Serial.print("\t");
  if (!system) {
    Serial.print("! ");
  }
 
  // Display the individual values
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);
}


void SerialPrintData3DVector(sensors_vec_t vector){
  Serial.print("X: ");
  Serial.print((float)vector.x);
  Serial.print("  Y: ");
  Serial.print((float)vector.y);
  Serial.print("  Z: ");
  Serial.print((float)vector.z);
  Serial.print("  ROLL: ");
  Serial.print((float)vector.roll);
  Serial.print("  PITCH: ");
  Serial.print((float)vector.pitch);
  Serial.print("  HEADING: ");
  Serial.print((float)vector.heading);
  Serial.println("");
}



/* Arduino loop function, called once 'setup' is complete.
*/
void loop(void) {
  // Get a new sensor event
  sensors_event_t event;
  bno.getEvent(&event);
  
  SerialPrintData3DVector(event.magnetic);
  
  
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
  
  // The processing sketch expects data as roll, pitch, heading
  //Serial.print(F("Orientation: "));
  //Serial.print((float)event.orientation.x);
  //Serial.print(F(" "));
  //Serial.print((float)event.orientation.y); // Long axis of dev board
  //Serial.print(F(" "));
  //Serial.print((float)event.orientation.z);
  //Serial.println(F(""));

  // Write data to shift register.
  // WriteFloatToShiftRegister((float)event.magnetic.x);
  
  
  // Also send calibration data for each sensor
  //uint8_t sys, gyro, accel, mag = 0;
  //bno.getCalibration(&sys, &gyro, &accel, &mag);
  //Serial.print(F("Calibration: "));
  //Serial.print(sys, DEC);
  //Serial.print(F(" "));
  //Serial.print(gyro, DEC);
  //Serial.print(F(" "));
  //Serial.print(accel, DEC);
  //Serial.print(F(" "));
  //Serial.println(mag, DEC);

  delay(BNO055_SAMPLERATE_DELAY_MS);
  counter += BNO055_SAMPLERATE_DELAY_MS;

  // runs a continuous blinking on the board while loop is running
  // why isn't this blink13?
  if( counter <= 500 ) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }

  if(counter >= 1000){
    counter = 0;
    }
}
