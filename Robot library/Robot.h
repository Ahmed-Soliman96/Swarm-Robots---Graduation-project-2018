#ifndef Robot_h
#define Robot_h
#include "Arduino.h"

#include "Servo.h"
class Robot
{
public:
	Robot();
	Robot( int n);
	void forward();
	void forward_delay(int delay_time);
	void backward_delay(int delay_time);
	void backward();
	void Stop();
	void goRight(int delay_time);
	void goLeft(int delay_time);
	int getNum();
	void setNum(int n);
	int getSpeed1();
	void setSpeed1(int s1);
	int getSpeed2();
	void setSpeed2(int s2);
	void setisleader(bool x);
	bool getisleader();
	void setHead(char x);
	char getHead();
	Servo left;
	Servo right;

private:
	int num;
	int speed1;
	int speed2;
	bool isleader;
	char head;

};
#endif