// sensor_test.ino
// 10-2-2025
// test program for ultrasonic sensor to print out values. originally compiled and run in arduino IDE
/*
  *@File  : DFRobot_Distance_A01.ino  
  *@Brief : This example use A01NYUB ultrasonic sensor to measure distance
  *         With initialization completed, We can get distance value  
  *@Copyright [DFRobot](http://www.dfrobot.com),2016          
  *           GUN Lesser General Pulic License
  *@version V1.0            
  *@data  2019‐8‐28
*/
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D5, D6); // TX green d5, RX blue d6 // according to AI, these are good chioces for pins for NodeMCU
// we need software serial 
unsigned char data[4]={};
float distance;
void setup()
{
  mySerial.begin(9600);   // match ESP8266's baud rate
  Serial.begin(9600); // match ESP8266's baud rate
}

void loop()
{
  // Serial.print("checkpoint 1");
    do{
     for(int i=0;i<4;i++)
     {
       data[i]=mySerial.read();
     }
  }while(mySerial.read()==0xff);
  // Serial.printf("checkpoint 2 data= %d|%d|%d|%d|\n", data[0], data[1], data[2], data[3]);
  // Serial.printf("checkpoint 2 data = %02X|%02X|%02X|%02X|\n", data[0], data[1], data[2], data[3]);
  mySerial.flush();
  if(data[0]==0xff)
    {
      int sum;
      sum=(data[0]+data[1]+data[2])&0x00FF;
      if(sum==data[3])
      {
        distance=(data[1]<<8)+data[2];
        if(distance>280)
          {
           Serial.print("distance=");
           Serial.print(distance/10);
           Serial.println("cm");
          }else  
              {
                Serial.println("Below the lower limit");         
              }
      }else Serial.println("ERROR: checksum failed");
     }
     delay(150);
}