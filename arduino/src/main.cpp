#include <ESP8266WiFi.h>
#include <NewPing.h>
#define MQTT_MAX_PACKET_SIZE 10

#include <PubSubClient.h>
#include <Servo.h>

#include "controller.cpp"

const char* ssid = "Galaxy M13 5G";
const char* password = "littleb0is";
const char* mqtt_server = "192.168.117.206";

#define LED D0
#define TRIGGER_PIN D1
#define ECHO_PIN D2
#define SERVO_PIN D3
#define DISTANCE_THRESHOLD 15

void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void led_off();
void led_on();
void setup_wifi();

WiFiClient espClient;
PubSubClient client(espClient);
Servo servo;
Pod pod(servo);
NewPing sonar(TRIGGER_PIN, ECHO_PIN);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  servo.attach(SERVO_PIN);
  pod.zero_state();
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  int distance = sonar.ping_cm();
  if (distance < DISTANCE_THRESHOLD && distance != 0 && pod.state == 'f') {
    pod.zero_state();

    servo.write(0);
    delay(500);
    int left_distance = sonar.ping_cm();
    servo.write(180);
    delay(500);
    int right_distance = sonar.ping_cm();
    pod.move_backward();
    servo.write(90);
    delay(1000);

    if (left_distance < DISTANCE_THRESHOLD &&
        right_distance < DISTANCE_THRESHOLD && left_distance != 0 &&
        right_distance != 0) {
      pod.rotate_left();
      pod.rotate_left();
      pod.rotate_left();
      pod.rotate_left();
      pod.move_forward();
      return;
    }

    if (left_distance == 0) {
      pod.rotate_left();
      delay(500);
      pod.move_forward();
      return;
    }

    if (right_distance == 0) {
      pod.rotate_right();
      delay(500);
      pod.move_forward();
      return;
    }

    if (left_distance < right_distance) {
      pod.rotate_right();
      delay(500);
      pod.move_forward();
      return;
    } else if (right_distance < left_distance) {
      pod.rotate_left();
      delay(500);
      pod.move_forward();
      return;
    }
    pod.turn_right();
    pod.turn_right();
    pod.turn_right();

    pod.move_forward();
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    led_on();
    delay(250);
    led_off();
    delay(250);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    led_on();
    delay(100);
    led_off();
    delay(100);
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), 0, 0, 0, 0, 0, 0, 0)) {
      Serial.println("connected");
      client.subscribe("topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      delay(1000);
    }
  }
  led_on();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Received");
  switch (payload[0]) {
    case 'f':
      pod.move_forward();
      break;
    case 'b':
      pod.move_backward();
      break;
    case 'l':
      pod.turn_left();
      break;
    case 'r':
      pod.turn_right();
      break;
    case 'e':
      pod.rotate_left();
      break;
    case 'w':
      pod.rotate_right();
      break;
    case 's':
      pod.zero_state();
      break;
  }
}

void led_off() { digitalWrite(LED, HIGH); }

void led_on() { digitalWrite(LED, LOW); }