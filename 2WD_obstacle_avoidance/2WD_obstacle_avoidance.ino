#include <NewPing.h>
#include <Servo.h>  // disables PWM on pins 9 & 10

// motor A (right side)
int enA = 3;
int in1 = 4;
int in2 = 5;

// motor B (left side)
int enB = 11;
int in3 = 9;
int in4 = 10;

// motor speeds
int A_speed = 100;
int B_speed = 110;
int A_turn_speed = 30;
int B_turn_speed = 30;

int run_time = 500;       // time to run straight when making turns to avoid obstacle
int turn_time_90 = 300;   // time to turn 90 degrees in milliseconds

// ultrasonic sensor
const int trigger = A2;
const int echo = A1;
const float max_dist = 400.00;

float front_dist = 100.00;
float left_dist;
float right_dist;
float stop_dist = 35.00; 

NewPing sonar(trigger, echo, max_dist);

// servo motor
Servo myservo;

// ----------------MOTOR FUNCTIONS----------------

void forward() {
  analogWrite(enA, A_speed);
  analogWrite(enB, B_speed);
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  analogWrite(enA, A_speed);
  analogWrite(enB, B_speed);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left() {
  analogWrite(enA, A_turn_speed);
  analogWrite(enB, B_speed);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void right() {
  analogWrite(enA, A_speed);
  analogWrite(enB, B_turn_speed);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// ----------------ULTRASONIC SENSOR FUNCTIONS----------------

float getDist() {
  float duration = sonar.ping();
  float dist = (duration / 2) * 0.0343; // at 20 degrees C

  if (dist == 0) {
    dist = 100.00;
  }

  return dist;
}

void getRightDist() {
  myservo.write(0);
  delay(1000);

  right_dist = getDist();
  delay(200);
  Serial.print("right dist = ");
  Serial.println(right_dist);

  myservo.write(90);
  delay(1000);
}

void getLeftDist() {
  myservo.write(180);
  delay(1000);

  left_dist = getDist();
  delay(200);
  Serial.print("left dist = ");
  Serial.println(left_dist);

  myservo.write(90);
  delay(1000);
}

void turnAround() {
  backward();
  delay(2000);
  changeDir();
}

void changeDir() {
  
  stop(); 
  
  getRightDist();
  getLeftDist();

  if (left_dist > right_dist) {
    left();
    delay(turn_time_90);
  } else if (left_dist < right_dist) {
    right();
    delay(turn_time_90);
  } else if (left_dist == right_dist) {
    turnAround();
  }
}

void avoidObstacle() {

  stop();
  
  getRightDist();
  getLeftDist();

  if (left_dist > right_dist) {
    left();
    delay(turn_time_90);

    forward();
    delay(run_time);
    
    right();
    delay(turn_time_90);
    
    forward();
    delay(run_time);
    
    right();
    delay(turn_time_90);
    
    forward();
    delay(run_time);
    
    left();
    delay(turn_time_90);
  
  } else {
    
    right();
    delay(turn_time_90);
    
    forward();
    delay(run_time);
    
    left();
    delay(turn_time_90);
    
    forward();
    delay(run_time);
    
    left();
    delay(turn_time_90);
    
    forward();
    delay(run_time);
    
    right();
    delay(turn_time_90);
  }
}

// ------------------------------------------------------------

void setup() {
  // motors
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // set motor speed
  analogWrite(enA, A_speed);
  analogWrite(enB, B_speed);

  // ultrasonic sensor
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  // servo motor
  myservo.attach(A5);
  myservo.write(90);

  Serial.begin(9600);

  delay(2000);
}

void loop() {
  myservo.write(90);
  delay(100);

  front_dist = getDist();
  delay(200);

  if (front_dist == 0) {
    front_dist = 100.00; 
  }

  Serial.print("front dist = ");
  Serial.println(front_dist);

  while(front_dist > stop_dist) {
    forward();
    front_dist = getDist();
    delay(200);
  }
  
  changeDir();
}
