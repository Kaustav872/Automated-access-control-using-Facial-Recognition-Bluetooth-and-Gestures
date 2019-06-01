#include <Wire.h>                // importing the required libraries
#include<SoftwareSerial.h>
#include <MPU6050.h>             // library for the gyroscope and accelerometer
#include<String.h>               // for the password from bluetooth
#include <Servo.h>               // servo lib for rotating the servo
char final[5];                   // declaring for the password
char correct[5] = {'a','d','m','i','n'};          //the correct sequence of password from the bluetooth device
Servo sg90; 
int pass_true;                    // creating a servo object
char state[10];                   // declaring state for matching the password
int servo_pin = 2;                 
MPU6050 sensor ;               
int16_t ax, ay, az ;              // declaring variableds for storing accelometer values
int16_t gx, gy, gz ;              // declaring variableds for storing gyroscope values
SoftwareSerial ab(5,6);            // declaring Serial communication pins for the bluetooth device

void setup ( )

{ 
sg90.attach ( servo_pin );
Wire.begin ( );
ab.begin(9600);                  // begining bluetooth module communication baudrate
Serial.begin  (19200);           // begining python script communication baudrate for face recognise
Serial.println  ( "Initializing the sensor" ); 
sensor.initialize ( ); 
Serial.println (sensor.testConnection ( ) ? "Successfully Connected" : "Connection failed"); 
delay (1000); 
Serial.println ( "Taking Values from the sensor" );
delay (1000);
pinMode(13,OUTPUT);          // output for the green LED
pinMode(10,OUTPUT);          // output for the red LED
}


void loop ( ) 
{ 
sensor.getMotion6 (&ax, &ay, &az, &gx, &gy, &gz);          // getting the sensor data
ax = map (ax, -17000, 17000, 0, 180) ;                     // mapping the sensor axis
  if(Serial.read() == 'A')                                 //loop for checking serial communication from the python script
  {
    sg90.write (90); 
    digitalWrite(10,LOW);
    digitalWrite(13,HIGH);
    delay (10000); 
  }
  {
  while(Serial.available()){                              // loop for checking the data received from bluetooth module
    for(int i=0; i<5; i++){
      final[i] = Serial.read();
    }
    for(int i=0; i<5; i++){
      if(final[i]==correct[i]){
        pass_true = 1;
      }
      else{
    pass_true = 0;
    break;
      }
    }
  }
if(pass_true==1){
    sg90.write (90); 
    digitalWrite(10,LOW);
    digitalWrite(13,HIGH);
    delay (10000);
    pass_true = 0;
  }}
 {
  while(ab.available()){                                // same loop if we dont give this loop the python script data generates some garbage values and continiously moves the servo 
    for(int i=0; i<5; i++){
      final[i] = ab.read();
    }
    for(int i=0; i<5; i++){
      if(final[i]==correct[i]){
        pass_true = 1;
      }
      else{
    pass_true = 0;
    break;
      }
    }
  }

  if(pass_true==1){
    sg90.write (90); 
    digitalWrite(10,LOW);
    digitalWrite(13,HIGH);
    delay (10000);
    pass_true = 0;
  }}

if(ax>140){                         // for checking the feedback from the accelerometer

    sg90.write (90); 
    delay (200);
    digitalWrite(10,LOW);
    digitalWrite(13,HIGH);

}
else if(ax<140){                   // else part if all the other conditions fails the lock wont open
  sg90.write(0);
  delay (200);
  digitalWrite(13,LOW);
  digitalWrite(10,HIGH);
}
}
