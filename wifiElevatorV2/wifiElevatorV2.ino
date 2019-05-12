#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID        "IT-lyceumKFU"
#define PASSWORD    "32323223"
#define HOST_NAME   "www.historygo.info"
#define HOST_PORT   (80)

#define upmot 5
#define downmot 6

#define mods 1  // 1 demo mode, 0 normal mode

int num[2] = {0, 3};
String elev = "325423545";
int lift = -1;
bool sending = false;
String el[3] = {"floor", "entr", "elev"};
int ret = 1;

SoftwareSerial mySerial(3, 2); /* RX:D2, TX:D3 */
ESP8266 wifi(mySerial);

void setup(void)
{
  Serial.begin(9600);
  // Serial.print("setup begin\r\n");

  // Serial.print("FW Version:");
  // Serial.println(wifi.getVersion().c_str());

  /* if (wifi.setOprToStationSoftAP()) {
     Serial.print("Wifi work\r\n");
    } else {
     Serial.print("Wifi error(don't worry)\r\n");
    }
  */
  while (true) {
    Serial.println("WiFi setup [START]");
    if (wifi.joinAP(SSID, PASSWORD)) {
      Serial.print("Join AP success\r\n");
      if (wifi.disableMUX()) {
        Serial.println("WiFi setup [DONE]");
        break;
      } else {
        Serial.println("WiFi setup [MUX ERR]");
      }
    } else {
      Serial.println("WiFi setup [AP ERR]");
    }
    //If init failed, wait 20s and try again
    delay(20000);
  }
  
  Serial.print("setup successful\r\n");
  Serial.println("registering elevator in server");

  String newdata = "elev=" + String(elev);
  while (lift == -1)
    lift = (sendmessage("authlift", newdata)).toInt();

  pinMode(upmot, OUTPUT);
  pinMode(downmot, OUTPUT);
}

void loop(void)
{
  String newdata = "";
  for (int i = 0; i < 2; i++)
  {
    if (i != 0)
      newdata += "&";
    newdata += String(el[i]);
    newdata += "=";
    newdata += String(num[i]);
  }
  newdata += "&";
  newdata += String(el[2]);
  newdata += "=";
  newdata += String(lift);
  newdata += "&ret=";
  newdata += String(ret);
  int inf = (sendmessage("setlift", newdata)).toInt();
  moved(inf);
  if (inf == 1)
    num[0]++;
  else if (inf == 2)
    num[0]--;
  delay(2000);
}

String sendmessage(String script, String gefo)
{
  uint8_t buffer[512] = {0};

  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print("connecting to server\r\n");
    String data = "GET /chester/" + script + ".php?";
    data += gefo;
    data += " HTTP/1.1\r\n";
    data += "Host: www.historygo.info\r\nConnection: close\r\n\r\n";
    Serial.println(data);
    wifi.send(data.c_str(), data.length());

    bool newe = false;

    uint32_t len = wifi.recv(buffer, sizeof(buffer), 2000);
    String mess = "";
    if (len > 0) {
      Serial.print("Received:[");
      for (uint32_t i = 0; i < len; i++) {
        char c = (char)buffer[i];
        if (c == '{')
          newe = true;
        if (c == '}')
        {
          newe = false;
          break;
        }
        if (newe && c != '{')
        {
          mess += String(c);
        }
      }
      Serial.print(mess);
      Serial.println("]");
    }

    if (wifi.releaseTCP()) {
      Serial.print("close tcp ok\r\n");
    }
    else
    {
      Serial.print("close tcp error(don't worry)\r\n");
    }
    return mess;
  }
  else {
    Serial.print("ERROR connect to server\r\n");
    return 0;
  }
  //delay(1000);
}

void moved(int dir)
{
  // dir == {1 up ,2 down}
  int sum = 0;
  int osv = 1000;
  while (sum < 2 && dir > 0)
  {
    if (dir == 1)
    {
      digitalWrite(downmot, 0);
      digitalWrite(upmot, 1);
    }
    else if (dir == 2)
    {
      digitalWrite(downmot, 1);
      digitalWrite(upmot, 0);
    }
    if (mods == 0)
    {
      osv = analogRead(A5);
      Serial.println(osv);
      if (osv < 80)
        sum++;
      delay(10);
    }
    else
    {
      delay(3000);
      sum = 10;
    }
  }
  delay(100);
  digitalWrite(downmot, 0);
  digitalWrite(upmot, 0);
}
