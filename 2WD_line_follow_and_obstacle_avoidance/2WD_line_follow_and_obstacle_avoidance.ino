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
int A_speed = 120;
int B_speed = 120;

int A_turn_speed = 40;
int B_turn_speed = 40;

int straight_time = 730;   // time to go straight when avoiding obstacle
int turn_time_90_left = 400;     // time to turn 90 degrees left in milliseconds
int turn_time_90_right = 400;     // time to turn 90 degrees right in milliseconds

// IR sensors
const int left_IR = A3;
const int right_IR = A0;

// ultrasonic sensor
const int trigger = A2;
const int echo = A1;
const float max_dist = 400.00; // max distance 400.00 cm

float front_dist;
float left_dist;
float right_dist;
float stop_dist = 30.00; // stops at distance of 30.00 cm

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
  //turn by using different motor speeds
  analogWrite(enA, A_turn_speed);
  analogWrite(enB, B_speed);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void sharpLeft() {
  // turn by turning one motor in reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void right() {
  // turn by using different motor speeds
  analogWrite(enA, A_speed);
  analogWrite(enB, B_turn_speed);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void sharpRight() {
  // turn by turning one motor in reverse
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
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

void avoidObstacle() {

  stop();
  
  getRightDist();
  getLeftDist();

  if (left_dist > right_dist) {
    sharpLeft();
    delay(turn_time_90_left);

    forward();
    delay(straight_time);
    
    sharpRight();
    delay(turn_time_90_right);
    
    forward();
    delay(straight_time);
    
    sharpRight();
    delay(turn_time_90_right);
    
    forward();
    delay(straight_time);

    if ((digitalRead(left_IR) == 1) || (digitalRead(right_IR) == 1)) {
      sharpLeft();
      delay(turn_time_90_left);
    }
  
  } else {
    
    sharpRight();
    delay(turn_time_90_right);
    
    forward();
    delay(straight_time);
    
    sharpLeft();
    delay(turn_time_90_left);
    
    forward();
    delay(straight_time);
    
    sharpLeft();
    delay(turn_time_90_left);
    
    forward();
    delay(straight_time);

    if ((digitalRead(left_IR) == 1) || (digitalRead(right_IR) == 1)) {
      sharpRight();
      delay(turn_time_90_right);
    }
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

  // IR sensors
  pinMode(left_IR, INPUT);
  pinMode(right_IR, INPUT);

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
  
  // IR sensors detect white --> low value --> digitalRead() == 0
  // IR sensors detect black line --> high value --> digitalRead() == 1

  // 1. left & right sensors NOT on line --> move forward and avoid obstacles
  // 2. left sensor detects line --> turn right 
  // 3. right sensor detects line --> turn left
  // 4. stop at T intersection

  front_dist = getDist();
  Serial.print("front dist = ");
  Serial.println(front_dist);

  if ((digitalRead(left_IR) == 0) && (digitalRead(right_IR) == 0)) {
    forward();
    if (front_dist <= stop_dist) {
      avoidObstacle();
    } 
  } else if ((digitalRead(left_IR) == 1) && (digitalRead(right_IR) == 0)) {
    left();
  } else if ((digitalRead(left_IR) == 0) && (digitalRead(right_IR) == 1)) {
    right();
  } else {
    stop();
  }
}
