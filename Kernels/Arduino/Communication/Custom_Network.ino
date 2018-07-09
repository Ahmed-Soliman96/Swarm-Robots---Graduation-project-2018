#include<SPI.h>
#include<RF24.h>

char robot_id[1] = "1";
char leader_id[1] = "1";

//int robot_id = 1;
//int leader_id = 1;



RF24 radio(9, 10);

const uint64_t pipes[2] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0D2LL };
// 1st byte > from & 2nd byte > to

void setup(void)
{

  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(0x76);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.enableDynamicPayloads();
  radio.powerUp();
  //radio.startListening();
}

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
  }
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



