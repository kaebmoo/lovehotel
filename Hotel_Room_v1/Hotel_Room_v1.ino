/*
MIT License

Copyright (c) 2017 kaebmoo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <PubSubClient.h>
#include "ESP8266WiFi.h"
#include <BlynkSimpleEsp8266.h>

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "27092c0fc50343bc917a97c755012c9b";

const char *ssid = "CAT-Register";
const char *password = "";
const char* mqtt_server = "192.168.9.1";

const int relayPin = D1;
const int buzzer=D5; //Buzzer control port, default D5

byte mac[6] { 0x60, 0x01, 0x94, 0x82, 0x85, 0x54};
IPAddress ip(192, 168, 9, 201);
IPAddress gateway(192, 168, 9, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress server(192, 168, 9, 1);

WiFiClient espClient;
PubSubClient client(espClient);

char *myRoom = "room1";
char *mqtt_user = "chang";
char *mqtt_password = "chang";
char *room_status = "room1/status";

int mqtt_reconnect = 0;

void buzzer_sound()
{
  analogWriteRange(1047);
  analogWrite(buzzer, 512);
  delay(100);
  analogWrite(buzzer, 0);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  delay(100);

  analogWriteRange(1175);
  analogWrite(buzzer, 512);
  delay(300);
  analogWrite(buzzer, 0);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  delay(300);
}

void callback(char* topic, byte* payload, unsigned int length) {
  /*
  char *room_status;
  char *topic = "/status";

  room_status = malloc(strlen(myRoom)+strlen(topic)+1);
  strcpy(room_status, "");
  strcat(room_status, myRoom);
  strcat(room_status, topic);
  */

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(relayPin, HIGH);
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)

    client.publish(room_status,"ON", true);
    Serial.print(room_status);
    Serial.println(" : ON");
    buzzer_sound();
  } else {
    digitalWrite(relayPin, LOW);
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    client.publish(room_status,"OFF", true);
    Serial.print(room_status);
    Serial.println(" : OFF");
  }

  //free(room_status);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  WiFi.config(ip,gateway,subnet);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[0],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.println(mac[5],HEX);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(myRoom, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(room_status, "hello world");
      // ... and resubscribe
      client.subscribe(myRoom);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.print(" try : ");
      Serial.print(mqtt_reconnect+1);
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    mqtt_reconnect++;
    if (mqtt_reconnect > 9) {
      mqtt_reconnect = 0;

      break;
    }
  }
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  Serial.println(pinValue);
  if (pinValue == 1)
    buzzer_sound();
}

void reconnectBlynk() {
  if (!Blynk.connected()) {
    if(Blynk.connect()) {
      BLYNK_LOG("Reconnected");
    } else {
      BLYNK_LOG("Not reconnected");
    }
  }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(buzzer, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (client.connect(myRoom, mqtt_user, mqtt_password)) {
    client.publish(room_status,"hello world");
    client.subscribe(myRoom);
  }

  delay(500);

  Blynk.config(auth);  // in place of Blynk.begin(auth, ssid, pass);
  Blynk.connect(3333);  // timeout set to 10 seconds and then continue without Blynk, 3333 is 10 seconds because Blynk.connect is in 3ms units.

  //pinMode(BUILTIN_LED, OUTPUT);
  buzzer_sound();
  digitalWrite(BUILTIN_LED, LOW);  // turn on LED with voltage HIGH
  delay(200);                      // wait one second
  digitalWrite(BUILTIN_LED, HIGH);   // turn off LED with voltage LOW
  delay(200);

}

void loop() {
  // put your main code here, to run repeatedly:

  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  else {
    if (!client.connected()) {
      reconnect();
    }
  }

  client.loop();
  Blynk.run();

}

