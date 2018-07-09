#include "Robot.h"
#include "Arduino.h"

Robot::Robot(){
  speed1 = 0;
  speed2=0;
  num = -1;
  isleader=0;
  right.write(speed1);
  left.write(180 - speed2);
  }

  Robot::Robot(int n){
	  num = n;
	  speed1 = 0;
	  speed2=0;
	  isleader=false;
	  right.write(speed1);
	  left.write(180 - speed2);
  }
  
void Robot::forward(){
  right.write(speed1);
  left.write(180 - speed2);
  }

void Robot::forward_delay(int delay_time){
	right.write(speed1);
	left.write(180 - speed2);
	delay(delay_time);
	Stop();
}

void Robot::backward(){
  right.write(180 - speed1);
  left.write(speed2);
  }

void Robot::backward_delay(int delay_time){
  right.write(180 - speed1);
  left.write(speed2);
	delay(delay_time);
	Stop();
}

void Robot::Stop(){
  right.write(90);
  left.write(90);
  }

void Robot:: goRight(int delay_time){
  right.write(180 - speed1);
  left.write(180 - speed2);
  delay( delay_time);
  Stop();
  }

void Robot:: goLeft(int delay_time){
  right.write(speed1);
  left.write(speed2);
  delay( delay_time);
  Stop();
  }

int Robot:: getNum(){
  return num;
  }
void Robot::setNum(int n){
  num = n;
  }

int Robot:: getSpeed1(){
  return speed1;
  }
void Robot::setSpeed1(int s1){
  speed1 = s1;
  }
  
  int Robot:: getSpeed2(){
  return speed2;
  }
void Robot::setSpeed2(int s2){
  speed2 = s2;
  }

void Robot::setisleader(bool x){
  isleader = x;
  }

bool Robot::getisleader(){
  return isleader;
  }
  
  void Robot::setHead(char c){
  head = c;
  }

char Robot::getHead(){
  return head;
  }
  
  