#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <Robot.h>

Robot manar(0);
bool flag;

struct point{
    float x;
    float y;
  };

struct target{
  float distance;
  float angle;
};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  manar.left.attach(5);
  manar.right.attach(6);
  manar.setSpeed1(40);
  manar.setSpeed2(60);
 
  flag=1;
}

void loop() {
  float phy = 90;
  point start = {-300,300};
  point dist = {-300,-300};
  target target = track(start, dist, phy);
  Serial.print("go for ");
  Serial.println(target.distance);
  Serial.print("with angle ");
  Serial.println(target.angle);
//  if(flag){
//  manar.forward_delay(target.distance);
//  delay(1000);
//  manar.goRight(target.angle*2);
//  flag=0;
//  }
}

target track(point start, point dist, float phy){
  target target;
  float dx = start.x - dist.x;
  float dy = start.y - dist.y;
  float distance = sqrt(pow(dx,2) + pow(dy,2));
  float theta = atan2(dy,dx);
  theta = theta*180/3.1415;
  float angle = theta - (90 - phy);
  target.distance = distance;
  target.angle = angle;
  manar.forward_delay(distance*1000);
  delay(1000);
  if(angle > 0){
      manar.goLeft(angle*1000);
  }
  else if(angle < 0){
    manar.goRight(angle*1000);
  }
  
  return target;
  }

