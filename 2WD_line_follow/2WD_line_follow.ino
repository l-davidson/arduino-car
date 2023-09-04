// motor A (right side)
int enA = 3;
int in1 = 4;
int in2 = 5;

// motor B (left side)
int enB = 11;
int in3 = 9;
int in4 = 10;

// IR sensors
const int left_IR = A3;
const int right_IR = A0;

// motor speeds
int A_speed = 110;
int B_speed = 110;

int A_turn_speed = 40;
int B_turn_speed = 40;

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

void setup() {
  // motors
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // set motor speed
  // analogWrite(enA, 100);
  // analogWrite(enB, 100);

  // IR sensors
  pinMode(left_IR, INPUT);
  pinMode(right_IR, INPUT);
}

void loop() {
  
  // IR sensors detect white --> digitalRead() == 0
  // IR sensors detect black line --> digitalRead() == 1

  if ((digitalRead(left_IR) == 0) && (digitalRead(right_IR) == 0)) {
    forward();
  } else if ((digitalRead(left_IR) == 1) && (digitalRead(right_IR) == 0)) {
    left();
  } else if ((digitalRead(left_IR) == 0) && (digitalRead(right_IR) == 1)) {
    right();
  } else {
    stop();
  }
}
