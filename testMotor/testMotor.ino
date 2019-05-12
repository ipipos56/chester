int corn[] = {3,2,6,5};
void setup()
{
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop()
{
  //pov(0,1);
  //delay(2000);
  pov(0,0);
  delay(10000000000000);
}

void pov(int a,int mot)
{
  if(a == 0) //left
  {
    digitalWrite(corn[mot * 2],1);
    digitalWrite(corn[(mot * 2)+1],0);
    delay(770);
  }
  else //up
  {
    digitalWrite(corn[mot * 2],0);
    digitalWrite(corn[mot * 2+1],1);   
    delay(770); 
  }
  digitalWrite(corn[mot * 2],0);
  digitalWrite(corn[mot * 2+1],0);
  delay(100);
}
