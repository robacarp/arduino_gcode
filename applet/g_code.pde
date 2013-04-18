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
