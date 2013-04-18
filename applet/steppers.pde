

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


