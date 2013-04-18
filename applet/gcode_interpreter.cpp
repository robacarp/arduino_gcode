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
#include "WProgram.h"
void setup();
void loop();
void runCommand();
void unknownCommand();
void printCommand();
void clearCommandRegisters();
void runGCode();
void G_position_rapid();
void G_linear_move();
void G_CW_move();
void G_CCW_move();
void G_Reverse();
void G_home();
int charArrayToInt(char chars[], int start, int size);
void runMCode();
void getSensorStatus();
void step(int stepPin);
void stepDistance(int stepPin, float distance, int delay);
void stepTo(int stepPin, float position, int delay);
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

//facts about the hardware
#define DEGREES_PER_STEP 1.8

#define THREADS_PER_INCH 16
#define INCHES_PER_THREAD 0.0625
#define INCHES_PER_STEP 0.00017361
#define INCHES_X_TRAVEL 8.6875

#define THREADS_PER_MM 0.6299
#define MM_PER_THREAD 1.5875
#define MM_PER_STEP 0.004409
#define MM_X_TRAVEL 220.662


//constants set for the software
#define UNITS mm
#define UNITS_PER_STEP 0.004097
#define STEPS_PER_UNIT 226.7
#define MIN_DELAY 1200

//pin configurations
#define X_STEP_PIN 4
#define X_DIR_PIN 5
#define X_MIN_PIN 6
#define X_MAX_PIN 7
#define Y_STEP_PIN 0
#define Y_DIR_PIN 0
#define Z_STEP_PIN 0
#define Z_DIR_PIN 0


void runGCode(){
  int len = 0;
  while (command[0][len]!='\0') len++;
  int code = charArrayToInt(command[0], 1, len);
  switch (code){
    case 0: G_position_rapid(); break;
    case 1: G_linear_move(); break;
    case 2: G_CW_move(); break;
    case 3: G_CCW_move(); break;
    case 4: G_Reverse(); break;
    case 28: G_home(); break;
    default: unknownCommand();
  }

}

void G_position_rapid() {
  switch (command[1][0]){
    case 88: case 120: {//x
      int dist = charArrayToInt(command[1], 1, COMMAND_LENGTH - 1);
      stepDistance(4, dist, 0);
      break;}
    case 89: case 121: //y
  //    stepDistance(Y_STEP_PIN, 10, 0);
      break;
    case 90: case 122: //z
      
      break;
    default:
      Serial.println(CODE_UNKNOWN);
  }
  Serial.println(CODE_OK);
}

void G_linear_move() {
  Serial.println(CODE_NOT_IMPLEMENTED);
}
void G_CW_move() {
    Serial.println(CODE_NOT_IMPLEMENTED);
}
void G_CCW_move() {
    Serial.println(CODE_NOT_IMPLEMENTED);
}
void G_Reverse() {
  digitalWrite(5, !digitalRead(5));
}
void G_home() {
    Serial.println(CODE_NOT_IMPLEMENTED);
}
//*/


int charArrayToInt(char chars[], int start, int size){
  int val = 0;
  for (int i=start; i<size; i++){
    if (chars[i] > 47 && chars[i] < 58){
      val*=10;
      val+=chars[i]-48;
    } else 
    return val;
  }
  return val;
}

void runMCode(){
  int len = 0;
  while (command[0][len]!='\0') len++;
  int code = charArrayToInt(command[0], 1, len);
  switch (code){
    case 0: break;
    case 1: getSensorStatus(); break;
    default: Serial.println(CODE_UNKNOWN);
  }
    Serial.println(CODE_OK);
}

void getSensorStatus(){
  char out[15];
  sprintf(out,"XMin: %d XMax: %d",digitalRead(X_MAX_PIN),digitalRead(X_MIN_PIN) );
  Serial.println(out);
}



void step(int stepPin){
  digitalWrite(stepPin, 1);
  delayMicroseconds(100);
  digitalWrite(stepPin, 0);
}

void stepDistance(int stepPin, float distance, int delay){
//  int wholeDistance = (int) distance;
  int steps = distance * STEPS_PER_UNIT;
  if (delay < MIN_DELAY) delay = MIN_DELAY;
//    Serial.print("steps: ");
//    Serial.println(steps);
//    Serial.print("delay: ");
//    Serial.println(delay);
//    Serial.print("distance: ");
//    Serial.println(distance);
  for (int i=0; i<steps; i++){
    step(stepPin);
    delayMicroseconds(delay);
    char buf[10];
    if (digitalRead(X_DIR_PIN) && !digitalRead(X_MIN_PIN)){
      sprintf(buf, "min %d out of %d", i, steps);
      Serial.println(buf);
      break;
    }
    if (!digitalRead(X_DIR_PIN) && !digitalRead(X_MAX_PIN)){
      sprintf(buf, "max %d out of %d", i, steps);
      Serial.println(buf);
      break;
    }
  }
}

void stepTo(int stepPin, float position, int delay){
  
}



int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

