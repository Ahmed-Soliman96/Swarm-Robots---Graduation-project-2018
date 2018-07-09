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

String yalla;
int Signal = 1;
int three_forward;
bool flag;


void setup() {
  Serial.begin(9600);
  delay(1000);

  rowbot.left.attach(5);
  rowbot.right.attach(6);
  rowbot.setSpeed1(40);
  flag = 0;

  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.setChannel(108);
  radio.openReadingPipe(1, pipe);
  radio.startListening();;
}

void loop() {
  // put your main code here, to run repeatedly:
  rowbot.Stop();

  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while ( !done)
    {
      // Fetch the data payload
      radio.read( &yalla, sizeof(yalla) );
      if (yalla == "01") {
        rowbot.backward();
        delay(1500);
        rowbot.Stop();
        TX_mode();
        flag = 1;
      }
    }
  }

  if (flag) {
    while (radio.isAckPayloadAvailable()) {

      radio.write(&Signal, sizeof(Signal));
    }
    flag = 0;
    RX_mode();
  }

  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while ( !done)
    {
      // Fetch the data payload
      radio.read( &three_forward, sizeof(three_forward) );
      if (three_forward == 1) {
        rowbot.forward();
        delay(1500);
        rowbot.Stop();
        radio.stopListening();
      }
    }
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
