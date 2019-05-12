#include <SPI.h>
#include <WiFi.h>
#include <Thread.h>

char ssid[] = "IT-lyceumKFU";
char pass[] = "32323223";
int keyIndex = 0;
int num[2] = {0, 3};
String elev = "";
int lift = -1;
bool sending = false;
String el[3] = {"floor", "entr", "elev"};
int ret = 1;

int status = WL_IDLE_STATUS;

Thread sendCoords = Thread();

char server[] = "www.historygo.info";
WiFiClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not connect, restart the USB port");
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
  int mes = -1;
  while (mes == -1)
    mes = createLift();

  Serial.println("Lift number is " + String(mes));

  lift = mes;
    pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);

  //sendCoords.onRun(sendMessage);
  //sendCoords.setInterval(1000);
}

void loop()
{
  //if (sendCoords.shouldRun() && !sending)
  //{
    //sending = true;
    //sendCoords.run();
    //Serial.println(icos);
    int inf = sendMessage();
    Serial.println(inf);
    if(inf == 1)
    {
      //up();
      num[0]++;
    }
    else if(inf == 2)
    {
      //down();
      num[0]--;
    } 
  //}
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  elev = String(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

int sendMessage()
{
  Serial.print("\nStarting connection to server: ");
  Serial.println(server);
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    client.print("GET /chester/setlift.php?");
    for (int i = 0; i < 2; i++)
    {
      if (i != 0)
        client.print("&");
      client.print(el[i]);
      client.print("=");
      client.print(num[i]);
    }
    client.print("&");
    client.print(el[2]);
    client.print("=");
    client.print(lift);
    client.print("&ret=");
    client.print(ret);
    client.println(" HTTP/1.1");
    client.println("Host: www.historygo.info");
    client.println("Connection: close");
    client.println();
    Serial.println("Server got information");
  }
  else
  {
    Serial.println("Connection denied");
  }

  char c = "s";
  String mess = "";
  while (true) {
    if (client.available()) {
      c = client.read();
      if (c == '{')
      {
        while (c != '}')
        {
          c = client.read();
          mess += String(c);
        }
        break;
      }
      //else
       // Serial.print(c);
    }
  }

  client.flush();
  client.stop();
  sending = false;
  return mess.toInt();
}

int createLift()
{
  Serial.print("\nStarting connection to server: ");
  Serial.println(server);
  if (client.connect(server, 80)) {
    Serial.println("is getting number");
    client.print("GET /chester/authlift.php?");
    client.print(el[2]);
    client.print("=");
    client.print(elev);
    client.println(" HTTP/1.1");
    client.println("Host: www.historygo.info");
    client.println("Connection: close");
    client.println();
    Serial.println("Server got information");
  }
  else
  {
    Serial.println("Connection denied");
  }

  char c = "s";
  String mess = "";
  while (true) {
    if (client.available()) {
      c = client.read();
      if (c == '{')
      {
        while (c != '}')
        {
          c = client.read();
          mess += String(c);
        }
        break;
      }
    //  else
      //  Serial.print(c);
    }
  }

  client.flush();
  client.stop();
  return mess.toInt();
}

void down()
{
  //analogWrite(6,255);
  int sum = 0;
  int osv = 1000;
  while (sum < 2)
  {
    digitalWrite(3, 1);
    digitalWrite(5, 0);
    osv = analogRead(A5);
    Serial.println(osv);
    if (osv < 80)
      sum++;
  }
  delay(100);
  digitalWrite(3, 0);
  digitalWrite(5, 0);
  delay(1000);
  //analogWrite(6,0);
}


void up()
{
  int sum = 0;
  int osv = 1000;
  while (sum < 2)
  {
    digitalWrite(6, 1);
    digitalWrite(3, 0);
    digitalWrite(5, 1);
    osv = analogRead(A5);
    Serial.println(osv);
    if (osv < 80)
     sum++;
    delay(10);
  }
  delay(100);
  digitalWrite(3, 0);
  digitalWrite(5, 0);
  delay(1000);
  digitalWrite(6,0);
}

