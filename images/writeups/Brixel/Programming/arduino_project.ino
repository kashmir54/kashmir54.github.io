int msg[] = {9,0,9,0,9,0,9,0,7,8,3,4,0,7,6,5,4,3,2,0,7,8,6,5,4,2,0,2,3,4,0,7,2,3,8,4,5,0,2,3,4,5,6,7,8,0,7,6,0,7,2,3,8,4,5,0,2,3,4,5,6,7,0,2,3,8,4,5,0,2,3,8,4,5,0,2,3,8,6,5,0};

void setup()
{
  pinMode(2, OUTPUT); //A
  pinMode(3, OUTPUT); //B
  pinMode(4, OUTPUT); //C
  pinMode(5, OUTPUT); //D
  pinMode(6, OUTPUT); //E
  pinMode(7, OUTPUT); //F
  pinMode(8, OUTPUT); //G
  pinMode(9, OUTPUT); //DP
  //COMMON = 5V with 1K resistor
}

void loop()
{
  for (byte i = 0; i < (sizeof(msg)/ sizeof(msg[0])); i++) {
    if(msg[i] > 0) {
      digitalWrite(msg[i],LOW);
        }else{
            delay(500);
            reset();
    }
    }
}

void reset()
{
  for(int p = 2; p < 10;p++)
  {
    digitalWrite(p,HIGH);   
  }
  delay(500);
}