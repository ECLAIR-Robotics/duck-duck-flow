/*
Code modified from https://docs.sunfounder.com/projects/umsk/en/latest/02_arduino/uno_lesson21_vl53l0x.html

Verdict:: this sensor is really precise on solid objetc but only up to roughly a meter
          Works extremely poorly on clear water but to some extent on cloudy water when tested indoors with turmeric to dirty the water

*/

#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

//Create an object of the Adafruit_VL53L0X class
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);

  // wait until serial port opens for native USB devices
  while (!Serial) {
    delay(1);
  }

  //Initialize the sensor
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)  //Stop the program if the sensor cannot be initialized
      ;
  }

  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false);  // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }

  delay(100);
}