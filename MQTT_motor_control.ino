#include <PubSubClient.h>
//#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>

#define RightMotorSpeed 5
#define RightMotorDir 0
//#define LeftMotorSpeed 4
//#define LeftMotorDir 2

const char* ssid = "YourSSID";
const char* password = "YourPASSWORD";
const char* mqtt_server = "192.168.3.186";
const char* mqtt_user = "franva";
const char* mqtt_pass = "rabbit";

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(9600);
  //Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);

  client.setCallback(callback);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  // pinMode(LeftMotorSpeed, OUTPUT);
  // pinMode(LeftMotorDir, OUTPUT);
  delay(5000);
  forward();
  delay(5000);
  halt();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
    if (receivedChar == '1') {
      digitalWrite(RightMotorDir, HIGH);
      //digitalWrite(LeftMotorDir, HIGH);
      digitalWrite(RightMotorSpeed, HIGH);
      //digitalWrite(LeftMotorSpeed, HIGH);
      // ESP8266 Huzzah outputs are "reversed"
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (receivedChar == '0')
    {
      digitalWrite(RightMotorSpeed, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("brid", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // ... and subscribe to topic
      client.subscribe("bird");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void halt()
{
  digitalWrite(RightMotorSpeed, LOW);
  //digitalWrite(LeftMotorSpeed, LOW);
}


void forward()
{
  digitalWrite(RightMotorDir, HIGH);
  //digitalWrite(LeftMotorDir, HIGH);
  digitalWrite(RightMotorSpeed, HIGH);
  //digitalWrite(LeftMotorSpeed, HIGH);
}

void reverse()
{
  digitalWrite(RightMotorDir, LOW);
  // digitalWrite(LeftMotorDir, LOW);
  digitalWrite(RightMotorSpeed, HIGH);
  // digitalWrite(LeftMotorSpeed, HIGH);
}

void right()
{
  digitalWrite(RightMotorDir, LOW);
  // digitalWrite(LeftMotorDir, HIGH);
  digitalWrite(RightMotorSpeed, HIGH);
  // digitalWrite(LeftMotorSpeed, HIGH);
}

void left()
{
  digitalWrite(RightMotorDir, HIGH);
  // digitalWrite(LeftMotorDir, LOW);
  digitalWrite(RightMotorSpeed, HIGH);
  // digitalWrite(LeftMotorSpeed, HIGH);
}
