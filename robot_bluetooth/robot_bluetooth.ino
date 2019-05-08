#include <BluetoothSerial.h>
#include <Wire.h>
#include "BluetoothSerial.h"
#include <stdio.h>
#include <math.h>
#include <dummy.h>
#include <VL53L1X.h> //LIDAR sensor library

BluetoothSerial SerialBT;

//TwoWire I2C_ONE = TwoWire(0); //INIT LIDAR1 I2C BITBANG PIN
//TwoWire I2C_TWO = TwoWire(1); //INIT LIDAR2 I2C BITBANG PIN

VL53L1X LIDAR_ONE;
VL53L1X LIDAR_TWO;

int count = 0;
const int PWM1pin = 14; //LEFT DRIVE MOTOR
const int DIR1pin = 32;
const int PWM1channel = 0;

const int PWM2pin = 15; // RIGHT DRIVE MOTOR
const int DIR2pin = 12;
const int PWM2channel = 0;

const int PWM3pin = 36; // TRAWLER MOTOR
const int DIR3pin = 39;
const int PWM3channel = 0;


const int IR_1pin = A0; // IR SENSORS
const int IR_2pin = A1;



void setup() {
  Serial.begin(115200);
  SerialBT.begin("A R N I E"); //Bluetooth device name
  //Serial.println("The device started, now you can pair it with bluetooth!");
  //pinMode(13,OUTPUT);
  //digitalWrite(13,LOW);

  ledcSetup(PWM1channel,5000,8); // pwm channel, frequency, resolution in bits
  ledcAttachPin(PWM1pin,PWM1channel); // pin, pwm channel

  ledcSetup(PWM2channel,5000,8);
  ledcAttachPin(PWM2pin,PWM2channel);

  ledcSetup(PWM3channel,5000,8);
  ledcAttachPin(PWM3pin,PWM3channel);

  pinMode(DIR1pin, OUTPUT); // set direction pins to output
  pinMode(DIR2pin, OUTPUT);
  pinMode(DIR3pin, OUTPUT);

  ledcWrite(PWM1channel,0); // pwm channel, speed 0-255
  digitalWrite(DIR1pin, LOW); // set direction to cw/ccw

  ledcWrite(PWM2channel,0); // pwm channel, speed 0-255
  digitalWrite(DIR2pin, LOW); // set direction to cw/ccw

  ledcWrite(PWM3channel,0); // pwm channel, speed 0-255
  digitalWrite(DIR3pin, LOW); // set direction to cw/ccw
  
  Wire.begin();
  Wire.setClock(400000);
  //I2C_ONE.begin(21,22,400000); //LIDAR 1 I2C bus
  //I2C_TWO.begin(27,33,400000); //LIDAR 2 I2C bus

  LIDAR_ONE.setTimeout(500);
  if (!LIDAR_ONE.init()) {
    Serial.println("LIDAR 1 initialization failed");
    //while(1);
  }
  LIDAR_ONE.setDistanceMode(VL53L1X::Long);
  LIDAR_ONE.setMeasurementTimingBudget(33000);
  LIDAR_ONE.startContinuous(33);
  
}

void loop() {
  char c = 0;
  int an = 0;
  int i;
  int LIDAR_1_RANGE;
  int LIDAR_2_RANGE;

  
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
    c = SerialBT.read();
    if(c == 'o'){
      //digitalWrite(13,HIGH);
    }
    else if (c == 'p'){
      //digitalWrite(13,LOW);
    }
    else if (c == 'a'){
      an = analogRead(A2);
      SerialBT.print("a ");
      SerialBT.println(an);
    }
    else if (c == 'x'){
      count = count + 1;
      for (i=0; i<100; i++){
        SerialBT.print("x ");
        SerialBT.print(i);
        SerialBT.print(" ");
        SerialBT.println(int(50*sin(2*3.14/50*i+count)));
      }
    }
    else if (c == 'c') {
      SerialBT.println("c 1");
    }
    else if (c == 'w') {
      digitalWrite(DIR1pin, LOW);
      ledcWrite(PWM1channel, 256);
      digitalWrite(DIR2pin, LOW);
      ledcWrite(PWM2channel, 256);
    }
    else if (c == 's') {
      digitalWrite(DIR1pin, HIGH);
      ledcWrite(PWM1channel, 256);
      
      digitalWrite(DIR2pin, HIGH);
      ledcWrite(PWM2channel, 256);
    }
    else if (c == 'b') {
      ledcWrite(PWM1channel, 0);
      ledcWrite(PWM2channel, 0);
      ledcWrite(PWM3channel, 0);
    }
    else if (c=='q') {
      digitalWrite(DIR3pin, LOW);
      ledcWrite(PWM3channel, 256);
    }
    else if (c == 'e') {
      digitalWrite(DIR3pin, HIGH);
      ledcWrite(PWM3channel, 256);
    }
    else if (c == 'r') {
      LIDAR_1_RANGE = LIDAR_ONE.read();
      SerialBT.println(LIDAR_1_RANGE);
    }
  }
  delay(20);
}
