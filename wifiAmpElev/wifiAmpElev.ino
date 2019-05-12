#include "ESP8266.h"
#include <SoftwareSerial.h>
#include <math.h>
#include <Thread.h>

#define SSID     "IT-lyceumKFU"
#define PASSWORD "32323223"

SoftwareSerial mySerial(4, 5);
ESP8266 wifi(mySerial);

uint32_t len;
int keyIndex = 0;
int num[2] = {0, 3};
String elev = "413235225134";
int lift = -1;
bool sending = false;
String el[3] = {"floor", "entr", "elev"};
int ret = 1;

Thread sendCoords = Thread();

char server[] = "www.historygo.info";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(SSID);
  } else {
    Serial.println("Wi-Fi connection error");
  }
  
  Serial.println("Connected to wifi");


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
{}

/*void loop()
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
}*/

/*void printWifiStatus() {
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
}*/

/*int sendMessage()
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
}*/

int createLift()
{
  uint8_t buffer[512] = {0};
  Serial.print("\nStarting connection to server: ");
  Serial.println(server);
  if (wifi.createTCP(server, 80)) {
    String data = "GET /chester/authlift.php?";
    data += el[2] + "=" + String(elev) + " HTTP/1.1\r\n";
    data += "Host: www.historygo.info";
    wifi.send(data.c_str(), data.length());
    wifi.releaseTCP();
    /*
    Serial.println("is getting number");
    client.print("GET /chester/authlift.php?");
    client.print(el[2]);
    client.print("=");
    client.print(elev);
    client.println(" HTTP/1.1");
    client.println("Host: www.historygo.info");
    client.println("Connection: close");
    client.println();*/
    Serial.println("Server got information");
  }
  else
  {
    Serial.println("Connection denied");
  }

 len = wifi.recv(buffer, sizeof(buffer), 10000);
  if (len > 0) {
    Serial.print("Received:[");
    for (uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
    }
    Serial.print("]\r\n");
  }

  //client.flush();
  //client.stop();
  //return mess.toInt();
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
