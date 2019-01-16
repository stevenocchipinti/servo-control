#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>
#include <math.h>

const char* ssid     = "NETWORK SSID GOES HERE";
const char* password = "NETWORK PASSWORD GOES HERE";

WiFiUDP UDPTestServer;
unsigned int UDPPort = 2807;
const int packetSize = 8;
byte packetBuffer[packetSize];

Servo myservo;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  UDPTestServer.begin(UDPPort);

  myservo.attach(D1);
}

void loop() {
   handleUDPServer();
   delay(1);
}

int byteToDegrees(int byteInt) {
  int rel = byteInt - 127;
  if (rel < 0) return round(90 - ((-rel / 127.0) * 90));
  if (rel == 0) return 90;
  if (rel > 0) return round(90 + ((rel / 128.0) * 90));
}

int Axis1 = 0; // integer to hold axis1
int Axis2 = 0; // integer to hold axis2
String inString = "";

void handleUDPServer() {
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer, packetSize);
    for(int i = 0; i < packetSize; i++) {
      int inChar = packetBuffer[i];

      if (isDigit(inChar)) {
        inString += (char)inChar;
      }

      if (inChar == 'a') {
        Axis1 = (inString.toInt());
        Serial.print(byteToDegrees(Axis1));
        Serial.print("a ");
        myservo.write(byteToDegrees(Axis1));
        delay(15);
        // clear the string for new input:
        inString = "";
      }

      if (inChar == 'b') {
        Axis2 = (inString.toInt());
        Serial.print(byteToDegrees(Axis2));
        Serial.println("b");
        //myservo.write(byteToDegrees(Axis2));
        //delay(15);
        // clear the string for new input:
        inString = "";
      }
    }
  }
}
