/*
  Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.

  Update 2014 - TMRh20
*/

/**
   Simplest possible example of using RF24Network,

   RECEIVER NODE
   Listens for messages from the transmitter and prints them out.
*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <Robot.h>


RF24 radio(9, 10);                       // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);              // Network uses that radio

const uint16_t node1 = 00;                // Address of our node in Octal format
const uint16_t node2 = 01;               // Address of the other node in Octal format
const uint16_t node3 = 02;              // Address of the other node in Octal format
const uint16_t node4 = 03;             // Address of the other node in Octal format
const uint16_t node5 = 04;            // Address of the other node in Octal format

Robot rowbot(0);

bool flag;

struct payload_t {
  bool dir;       /// Direction
  uint32_t  del;  ///  Delay
};


void setup(void)
{
  Serial.begin(115200);

  rowbot.setHead('M');

  rowbot.left.attach(5);
  rowbot.right.attach(6);
  rowbot.setSpeed1(40);
  rowbot.Stop();

  flag = true;

  Serial.println("RF24Network/examples/helloworld_rx/");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ node1);
}

void loop(void) {

  network.update();                   // Check the network regularly

  if (flag) {

    //delay(7000);

    rowbot.forward_delay(1000);

    while (!sendNext(node2, 'N', 3000)) {}
    //delay(4500);

    /*while (!sendNext.0..30(node2, 'N', 4000)) {}
    delay(4500);*/

    flag = false;
  }


}
bool sendNext(uint16_t node, char type, uint32_t _del) {
  Serial.print("Sending...");
  payload_t _payload = {1, _del};
  RF24NetworkHeader header1(/*to node*/node, type);
  //bool ok = network.write(header1, &_payload, sizeof(_payload));
  if (network.write(header1, &_payload, sizeof(_payload)))
  { Serial.println("ok.");
    return 1;
  }
  else
  { Serial.println("failed.");
    return 0;
  }

}
