#include <SPI.h>
#include <WiFi.h>
#include <Thread.h>

char ssid[] = "IT-lyceumKFU";
char pass[] = "32323223";
int keyIndex = 0;
int num[2] = {1, 1};
String elev = "";
bool sending = false;
int corn[] = {3,2,6,5};

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
  for(int i =0;i<sizeof(corn);i++)
  {
    pinMode(corn[i], OUTPUT);
  }
}

void loop()
{
    String inf = sendMessage();
    String in1 = String(inf[0]);
    String in2 = String(inf[1]);
    int ac = in1.toInt();
    int bc = in2.toInt();
    Serial.println(inf);
    if(ac > 0)
    {
      if(num[ac] != bc)
      {
        pov(ac,bc);
        num[ac] = bc;
      }
    }
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

String sendMessage()
{
  Serial.print("\nStarting connection to server: ");
  Serial.println(server);
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    client.print("GET /chester/getcorn.php");
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
  return mess;
}

void pov(int mot,int a)
{
  if(a == 0) //left garizontal
  {
    digitalWrite(corn[mot * 2],1);
    digitalWrite(corn[(mot * 2)+1],0);
    delay(770);
  }
  else if(a == 1) //up vertical
  {
    digitalWrite(corn[mot * 2],0);
    digitalWrite(corn[mot * 2+1],1);   
    delay(770); 
  }
  digitalWrite(corn[mot * 2],0);
  digitalWrite(corn[mot * 2+1],0);
  delay(100);
}
