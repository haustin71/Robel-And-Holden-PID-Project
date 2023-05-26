// Define motor pins
const int motor1Pin1 = 2;
const int motor1Pin2 = 3;
const int motor2Pin1 = 4;
const int motor2Pin2 = 5;

// Define IR sensor pins
const int ir1Pin = A2;
const int ir2Pin = A3;
const int ir3Pin = A4;
const int ir4Pin = A5;

// Define IR sensor threshold
const int threshold = 900; // Adjust this threshold value as per your line color and lighting conditions

// Define switch pin
const int switchPin = 6;

// PID Constants
const float Kp = 0.5;   // Proportional constant
const float Ki = 0.2;   // Integral constant
const float Kd = 0.1;   // Derivative constant

// Define variables for PID control
float error = 0;
float lastError = 0;
float integral = 0;
float derivative = 0;
float pidValue = 0;

bool motorsEnabled = true;

void setup() {
  // Initialize motor pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
 
  // Initialize switch pin as input
  pinMode(switchPin, INPUT_PULLUP);
 
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Check the state of the switch
  if (digitalRead(switchPin) == LOW) {
    motorsEnabled = !motorsEnabled; // Toggle the motorsEnabled variable
    delay(500); // Add a small delay for switch debounce
  }
 
  if (motorsEnabled) {
    // Read IR sensor values
    int ir1 = analogRead(ir1Pin);
    int ir2 = analogRead(ir2Pin);
    int ir3 = analogRead(ir3Pin);
    int ir4 = analogRead(ir4Pin);
 
    // Print IR sensor values to serial monitor
    Serial.print(ir1);
    Serial.print("\t");
    Serial.print(ir2);
    Serial.print("\t");
    Serial.print(ir3);
    Serial.print("\t");
    Serial.println(ir4);
 
    // Determine direction to turn
    int turn = 0;
    if (ir1 < threshold) {
      turn = 1;
    }
    else if (ir4 < threshold) {
      turn = -1;
    }
    else if (ir2 && ir3 && ir4 && ir1 > threshold){
      turn = 0;
    }
    // PID control
    error = ir3 - ir2; // Adjust the IR sensor order for line-following
    integral += error;
    derivative = error - lastError;
    pidValue = (Kp * error) + (Ki * integral) + (Kd * derivative);
 
    // Adjust motor speed and direction based on PID value
    int leftSpeed = 0;
    int rightSpeed = 0;
 
    if (turn == 0) {
      leftSpeed = 200;
      rightSpeed = 200;
    }
    else if (turn == 1) {
      leftSpeed = 200 + pidValue;
      rightSpeed = 200 - pidValue;
    }
    else if (turn == -1) {
      leftSpeed = 200 - pidValue;
      rightSpeed = 200 + pidValue;
    }
 
    // Set motor speeds and directions
    if (leftSpeed > 0) {
      analogWrite(motor1Pin1, leftSpeed);
      analogWrite(motor1Pin2, 0);
    } else {
      analogWrite(motor1Pin1, 0);
      analogWrite(motor1Pin2, -leftSpeed);
    }
 
    if (rightSpeed > 0) {
      analogWrite(motor2Pin1, rightSpeed);
      analogWrite(motor2Pin2, 0);
    } else {
      analogWrite(motor2Pin1, 0);
      analogWrite(motor2Pin2, -rightSpeed);
    }
 
    // Store the last error for the next iteration
    lastError = error;
  }
  else {
    // Turn off motors
    analogWrite(motor1Pin1, 0);
    analogWrite(motor1Pin2, 0);
    analogWrite(motor2Pin1, 0);
    analogWrite(motor2Pin2, 0);
  }
 
  // Add delay to prevent motor chatter
  delay(50);
}
