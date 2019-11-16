#include <Arduino.h>

#include <Wire.h>
#include "DFRobot_INA219.h"

DFRobot_INA219_IIC     ina219(&Wire, INA219_I2C_ADDRESS4);

#define PRINTCOUNT 1000
#define DELTAINTERVAL 60.0
//#define MEASUREMENTONLY

// Revise the following two paramters according to actula reading of the INA219 and the multimeter
// for linearly calibration
float ina219Reading_mA = 0.5;
float extMeterReading_mA = 5.00;
unsigned long count = 0;
double energy = 0;
unsigned long last_interval = 0;



void setup(void) 
{
    Serial.begin(115200);
    while(!Serial);

    Serial.println();
    while(ina219.begin() != true) {
        Serial.println("INA219 begin faild");
        delay(2000);
    }
    ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);
    Serial.println();
}

void loop(void)
{
   
#ifndef MEASUREMENTONLY

  unsigned long next_interval = last_interval + DELTAINTERVAL;

  double voltage = ina219.getBusVoltage_V();
  double current = ina219.getCurrent_uA();
  if(current > 0) {
    energy += ((double)DELTAINTERVAL/1000 * voltage * current/1000000);
  }
  
  unsigned long now = millis();
  delay(next_interval - now);
  last_interval = next_interval;
  count++;
  if (count > PRINTCOUNT) {
    Serial.print("Energy:   ");
    Serial.print((float)energy, 2);
    Serial.print("J, Timestamp :");
    Serial.print(now);
    Serial.println("ms");

    Serial.print("ShuntVoltage: ");
    Serial.print(voltage, 3);
    Serial.println("mV");
    Serial.print("Current:      ");
    Serial.print(current, 1);
    Serial.println("uA");

    count = 0;
  }

  #else



  Serial.print("BusVoltage:   ");
  Serial.print(ina219.getBusVoltage_V(), 2);
  Serial.println("V");
  Serial.print("ShuntVoltage: ");
  Serial.print(ina219.getShuntVoltage_mV(), 3);
  Serial.println("mV");
  Serial.print("Current:      ");
  Serial.print(ina219.getCurrent_uA(), 1);
  Serial.println("uA");
  //Serial.print("Power:        ");
  //Serial.print(ina219.getPower_mW(), 1);
  //Serial.println("mW");
  //Serial.println("");
  delay(1000);

#endif

}