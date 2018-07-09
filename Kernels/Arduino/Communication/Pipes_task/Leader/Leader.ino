#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>
#include <Ultrasonic.h>
#include <Robot.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN  10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN);
Robot rowbot(0);

String yalla1;
String yalla2;
int three_forward;
int Signal;
bool flag1, flag2;


void setup() {
  Serial.begin(9600);
  delay(1000);
  //Serial.print(60);

  rowbot.left.attach(5);
  rowbot.right.attach(6);
  rowbot.setSpeed1(40);

  flag1 = 1;
  flag2 = 1;
  yalla1 = "01";
  yalla2 = "11";
  three_forward = 1;
  Signal = -1;
  //yalla=100;

  radio.begin();
  radio.setChannel(108);
  radio.enableAckPayload();
  radio.openWritingPipe(pipe);
}

void loop() {
  // put your main code here, to run repeatedly:
  rowbot.Stop();
  if (flag1) {
    while (radio.isAckPayloadAvailable()) {

      radio.write(&yalla1, sizeof(yalla1));
    }
    flag1 = 0;
  }
  RX_mode();
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while ( !done)
    {
      // Fetch the data payload
      radio.read( &Signal, sizeof(Signal) );
      if (Signal == 1) {
        // if (flag) {
        TX_mode();
        Signal = -1;
        //flag=0;
        //}
        rowbot.Stop();
      }

    }

  }


  if (flag2) {
    while (radio.isAckPayloadAvailable()) {

      radio.write(&yalla2, sizeof(yalla2));
    }
    flag2 = 0;
  }
  RX_mode();
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while ( !done)
    {
      // Fetch the data payload
      radio.read( &Signal, sizeof(Signal) );
      if (Signal == 2) {
        // if (flag) {
        TX_mode();
        Signal = -1;
        //flag=0;
        //}
        rowbot.Stop();
      }

    }

  }


  while (radio.isAckPayloadAvailable()) {

    radio.write(&three_forward, sizeof(three_forward));    //Three robots move forward
  }


}

void RX_mode() {

  radio.openReadingPipe(1, pipe); // closes writing pipe and opens Reading pipe
  radio.startListening();        // sets listening mode so were receiving and we start all over again from the beginning...
  delay(200);
}

void TX_mode() {

  radio.stopListening(); // must stop listening before you open a writing pipe
  radio.setChannel(108);
  radio.enableAckPayload();
  radio.openWritingPipe(pipe);

}




