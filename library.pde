
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
