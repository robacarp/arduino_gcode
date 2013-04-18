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
