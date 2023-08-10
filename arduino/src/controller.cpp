#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include <Arduino.h>
#include <Servo.h>

#define LEFT_BACKWARD D5
#define RIGHT_BACKWARD D6
#define RIGHT_FORWARD D7
#define LEFT_FORWARD D8
#define TURN_DURATION 400
#define CHANGE_DELAY 1

class Pod {
 private:
  Servo servo;

 public:
  char state = 's';
  Pod(Servo servo) {
    this->servo = servo;
    pinMode(LEFT_BACKWARD, OUTPUT);
    pinMode(LEFT_FORWARD, OUTPUT);
    pinMode(RIGHT_BACKWARD, OUTPUT);
    pinMode(RIGHT_FORWARD, OUTPUT);
  }
  void move_forward() {
    Serial.println("Moving Forward");
    zero_state();
    state = 'f';
    delay(CHANGE_DELAY);
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_FORWARD, HIGH);
  }
  void move_backward() {
    Serial.println("Moving Backward");
    zero_state();
    state = 'b';
    delay(CHANGE_DELAY);
    digitalWrite(LEFT_BACKWARD, HIGH);
    digitalWrite(RIGHT_BACKWARD, HIGH);
  }
  void zero_state() {
    state = 's';
    Serial.println("Zero State");
    digitalWrite(LEFT_BACKWARD, LOW);
    digitalWrite(LEFT_FORWARD, LOW);
    digitalWrite(RIGHT_BACKWARD, LOW);
    digitalWrite(RIGHT_FORWARD, LOW);
    this->servo.write(90);
  }
  void turn_left() {
    Serial.println("Turning Left");
    zero_state();
    state = 'l';
    delay(CHANGE_DELAY);
    digitalWrite(RIGHT_FORWARD, HIGH);
    digitalWrite(LEFT_BACKWARD, HIGH);
    delay(TURN_DURATION);
    zero_state();
  }
  void turn_right() {
    Serial.println("Turning Right");
    zero_state();
    state = 'r';
    delay(CHANGE_DELAY);
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_BACKWARD, HIGH);
    delay(TURN_DURATION);
    zero_state();
  }
  void rotate_right() {
    Serial.println("Rotate Right");
    zero_state();
    state = 'w';
    delay(CHANGE_DELAY);
    digitalWrite(RIGHT_BACKWARD, HIGH);
    digitalWrite(LEFT_FORWARD, HIGH);
  }
  void rotate_left() {
    Serial.println("Rotate Left");
    zero_state();
    state = 'e';
    delay(CHANGE_DELAY);
    digitalWrite(RIGHT_FORWARD, HIGH);
    digitalWrite(LEFT_BACKWARD, HIGH);
  }
};

#endif