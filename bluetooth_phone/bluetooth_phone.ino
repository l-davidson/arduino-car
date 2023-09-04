// motor A (right side)
int enA = 3;
int in1 = 4;
int in2 = 5;

// motor B (left side)
int enB = 11;
int in3 = 9;
int in4 = 10;

void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
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
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);

  // set motor speed
  analogWrite(enA, 200);
  analogWrite(enB, 200);
}

void loop() {
  
  while(Serial.available() > 0) {

    String control = Serial.readString();

    if(control == "F") {
      forward();
    }

    if(control == "B") {
      backward();
    }

    if(control == "L") {
      left();
    }

    if(control == "R") {
      right();
    }

    if(control == "S") {
      stop();
    }

  }  
}
