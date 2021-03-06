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

  rowbot.setHead('N');

  rowbot.left.attach(5);
  rowbot.right.attach(6);
  rowbot.setSpeed1(40);
  rowbot.Stop();

  flag = true;

  Serial.println("RF24Network/examples/helloworld_rx/");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ node2);
}

void loop(void) {

  network.update();                   // Check the network regularly
  if (flag) {
    while ( network.available() ) {     // Is there anything ready for us?
      //Serial.print("ana hena2") ;  
      RF24NetworkHeader headerR;        // If so, grab it and print it out
      payload_t payloadR;
      network.peek(headerR);
      switch (headerR.type) {
        case 'N': network.read(headerR, &payloadR, sizeof(payloadR));
          Serial.print("\n\r Signal recieved \n\r");
          rowbot.forward_delay(payloadR.del);
          //Serial.print("ana hena");         
          while(!sendNext(node3, 'O', 3000)){}
          flag = false;
          break;

        default:  printf_P(PSTR("*** WARNING *** Unknown message type %c\n\r"), headerR.type);
          network.read(headerR, 0, 0);
          break;
      };

    }

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
