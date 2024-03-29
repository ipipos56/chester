/**
   @example TCPClientSingleUNO.ino
   @brief The TCPClientSingleUNO demo of library WeeESP8266.
   @author Wu Pengfei<pengfei.wu@itead.cc>
   @date 2015.03

   @par Copyright:
   Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version. \n\n
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID        "IT-lyceumKFU"
#define PASSWORD    "32323223"
#define HOST_NAME   "www.historygo.info"
#define HOST_PORT   (80)

SoftwareSerial mySerial(3, 2); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);

void setup(void)
{
  Serial.begin(9600);
  Serial.print("setup begin\r\n");

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP:");
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }

  Serial.print("setup end\r\n");
}

void loop(void)
{
  sendmessage("authlift");
}

String sendmessage(String script)
{
  uint8_t buffer[512] = {0};

  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print("create tcp ok\r\n");
  } else {
    Serial.print("create tcp err\r\n");
  }

  String data = "GET /chester/" + script + ".php?";
  data += "elev=143543543";
  data += " HTTP/1.1\r\n";
  data += "Host: www.historygo.info\r\nConnection: close\r\n\r\n";
  wifi.send(/*(const uint8_t*)*/data.c_str(), data.length());

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
    Serial.print("release tcp ok\r\n");
  } else {
    Serial.print("release tcp err\r\n");
  }
  delay(5000);
}
