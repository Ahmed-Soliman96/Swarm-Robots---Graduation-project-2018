#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <Robot.h>

Robot rowbot(1);

int leaderID = 1;

char robot_id[1] = "1";
char leader_id[1] = "1";

struct point {
  float x;
  float y;
};

struct target {
  float distance;
  float angle;
};



RF24 radio(9, 10);

const uint64_t pipes[2] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0D2LL };
// 1st byte > from & 2nd byte > to

void setup(void)
{

  Serial.begin(9600);
  rowbot.left.attach(5);
  rowbot.right.attach(6);
  rowbot.setSpeed1(40);
  rowbot.setSpeed2(60);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(0x76);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.enableDynamicPayloads();
  radio.powerUp();
  //radio.startListening();
}

unsigned long previousMillis = 0;
const long interval = 10000;    // 10 seconds timeout to elect a new leader

void loop(void) {
  if (leader_id[0] == robot_id[0])
  {
    Serial.println("yuo");
    leader();
  }
  else
  {
    get_position();
    Serial.println(to_leader("1111111"));
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval && to_leader("1111111") == "F")
      elect_leader();

  }
}

void elect_leader() {
int randNumber = random(1,7); // generate random number between 1 & 8
if (randNumber == leader_id) elect_leader();
else leaderID = randNumber; 
}


String get_position() {
  radio.stopListening();
  int i,j;
  char command[9] = {0};
  char answer[9];

  //char node_chr[1];
  //itoa(robot_id, node_chr, 2);
  //robot_id.toCharArray(node_chr, 1);
  char data[8] = "00000000";
  strcat(command, robot_id);
  strcat(command, data);

  for (i=0;i<100;i++)
  {
    radio.write(command,sizeof(command));
    
    radio.startListening();
    for (j=0;j<100;j++)
    {
      radio.read(answer,sizeof(answer));
      if(answer[0] == '0' && answer[1] == robot_id[0])
      {
        String pos(answer);
        pos = pos.substring(2,9);
        radio.stopListening();
        return(pos);
      }
    }
    radio.stopListening();
  }
  return("F");
}


String to_leader(String str) {
  radio.stopListening();
  int i,j;
  char command[9] = {0};
  char answer[9];
  char data[7];

  str.toCharArray(data, 7);
  
  strcat(command, robot_id);
  strcat(command, leader_id);
  strcat(command, data);
  
  for (i=0;i<100;i++)
  {
    radio.write(command,sizeof(command));
    
    radio.startListening();
    for (j=0;j<100;j++)
    {
      radio.read(answer,sizeof(answer));
      Serial.println(answer);
      if(answer[0] == leader_id[0] && answer[1] == robot_id[0])
      {
        String response(answer);
        response = response.substring(2,9);
        radio.stopListening();
        return(response);
      }
    }
    radio.stopListening();
  }
  return("F");
}

void leader()
{
  int i;
  char received[9];
  char command[9];
  char received_from[1];
  
  if (leader_id[0] == robot_id[0])
  {
    radio.startListening();
    for (i=0;i<100;i++)
    {
      radio.read(received,sizeof(received));
      if(received[1] == robot_id[0])
      {
        String data(received);
        data.substring(0,1).toCharArray(received_from, 1);
        data = data.substring(2,9);
        radio.stopListening();
        
        char send_data[7] = "113355"; //do what you'll do and get command
        
        strcat(command, robot_id);
        strcat(command, received_from);
        strcat(command, send_data);
        
        for (i=0;i<100;i++)
        {
          radio.write(command,sizeof(command));
        }
      }
    }
    
    radio.stopListening();
    
  }
}





target track(point start, point dist, float phy) {
  target target;
  float dx = start.x - dist.x;
  float dy = start.y - dist.y;
  float distance = sqrt(pow(dx, 2) + pow(dy, 2));
  float theta = atan2(dy, dx);
  theta = theta * 180 / 3.1415;
  float angle = theta - (90 - phy);
  target.distance = distance;
  target.angle = angle;
  rowbot.forward_delay(distance);
  delay(1000);
  if (angle > 0) {
    rowbot.goLeft(angle);
  }
  else if (angle < 0) {
    rowbot.goRight(angle);
  }

  return target;
}



