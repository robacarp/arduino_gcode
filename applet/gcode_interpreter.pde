#define COMMAND_LENGTH 15
#define COMMAND_REGISTERS 5
#define STATUS_LED 13

#define CODE_READY 100
#define CODE_OK 200
#define CODE_UNKNOWN 404
#define CODE_NOT_IMPLEMENTED 501
#define CODE_ERROR 400
#define CODE_X_MAX 488
#define CODE_X_MIN 4120
#define CODE_Y_MAX 489
#define CODE_Y_MIN 4121
#define CODE_Z_MAX 490
#define CODE_Z_MIN 4122

#include <stdio.h>
int command_top;
char command[COMMAND_REGISTERS][COMMAND_LENGTH];
char read;

unsigned short offset;

void setup() {
  Serial.begin(9600);
  Serial.println(CODE_READY);
  pinMode(STATUS_LED, 1 );
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
  digitalWrite(4, 1);
  digitalWrite(5, 1);
  offset = 0;
  command_top = 0;
  
}

void loop() {

  if (millis() % 500 == 0){
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
  }
  delay(1);
  while (Serial.available()){
      read=Serial.read();

      if (offset <= COMMAND_LENGTH && command_top < COMMAND_REGISTERS){
        if (read == 10){ // newline 
//          Serial.print("d:");
//          Serial.println("newline");
          command[command_top][offset]='\0';
          runCommand();
          offset = 0;
          command_top = 0;

        } else if (read == 32) { //space
//          Serial.print("d:");
//          Serial.println("space");

          command[command_top][offset]='\0';
          command_top++;
          offset = 0;          
        } else { //any other character
//          Serial.print("d:");
//          Serial.println(read);
          command[command_top][offset++]=read;
        }
      } else {
        unknownCommand();
        clearCommandRegisters();
      }
//      Serial.println("----------------------------");
//      printCommand();
//      Serial.println("----------------------------");
  }
}//*/

void runCommand(){
  switch (command[0][0]) {
    case 77: case 109:
      runMCode();
      break;
    case 71: case 103:
      runGCode();
      break;
    default:
      unknownCommand();
  }
  clearCommandRegisters();
}

void unknownCommand(){
  Serial.println(CODE_UNKNOWN);
}

void printCommand(){
  for(int i=0; i<=command_top; i++){
    Serial.print("token #");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(command[i]);
  }
  Serial.println('.');
}

void clearCommandRegisters(){
  for (int i=0; i<COMMAND_REGISTERS; i++){
    command[i][0] = '\0';    
  }
}
