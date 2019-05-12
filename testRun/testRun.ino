
void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(6,OUTPUT);
  Serial.begin(9600);
}
int osv = 1000;
int sum = 0;

void loop() {
  digitalWrite(6,1);
  digitalWrite(3,0);
  digitalWrite(5,0);
  osv = analogRead(A5);
  Serial.println(osv);
  /*for (int i = 0; i < 5; i++)
  {
    sum = 0;
    while (sum < 2)
    {
      digitalWrite(3, 0);
      digitalWrite(4, 1);
      osv = analogRead(A5);
      Serial.println(osv);
      if (osv < 80)
        sum++;
    }
    delay(100);
    digitalWrite(3, 0);
    digitalWrite(4, 0);
    delay(1000);
  }

      digitalWrite(3, 1);
      digitalWrite(4, 0);
      delay(200);
      digitalWrite(3, 0);
      digitalWrite(4, 0);
      delay(1000);
      
  for (int i = 0; i < 4; i++)
  {
    sum = 0;
    while (sum < 2)
    {
      digitalWrite(3, 1);
      digitalWrite(4, 0);
      osv = analogRead(A5);
      Serial.println(osv);
      if (osv < 80)
        sum++;
    }
    delay(100);
    digitalWrite(3, 0);
    digitalWrite(4, 0);
    delay(1000);
  }*/
}
