# PID Line Following Car
For our second project we decided to create a car that uses PID to run on a track made out of tape. The car uses IR sensors to detect the color of the tape, and it will follow the tape and correct itself when taking turns. 

Our Planning Document can be found [here](https://docs.google.com/document/d/19GB9ZAeACxbWW0eaMtp071EdHyZ0K5-Lup9LcHnjQMY/edit?usp=sharing)

## Table of Contents
* [Table of Contents](#TableOfContents)
* [CAD](#CAD)
* [Wiring Diagram](#Wiring_Diagram)
* [Code](#Code)
* [Working Prototype](#Working_Prototype)
* [Reflection](#Reflection)

# CAD
![Car CAD 1](https://github.com/haustin71/Robel-And-Holden-PID-Project/blob/fa9e027d30cbe8c66332671fc321e581bb650ff5/Car%201.PNG)

![Car CAD 2](https://github.com/haustin71/Robel-And-Holden-PID-Project/blob/fa9e027d30cbe8c66332671fc321e581bb650ff5/Car%202.PNG)


# Wiring_Diagram


# Code 
Here is the code that we are currently using 
```C++
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
```

# Working_Prototype
Here is a video of the Car spinning, we couldn't get it to follow the line but it still moves
(https://drive.google.com/file/d/1Naclon4Qbiw_vXSGhq8xGBdsn7rGnV0l/view)

Here are some pictures of the finished Car
![PID Car Prototype 1]()

# Reflection
Overall, we think that this project went very well because we ended up finishing it. Our last project wasn't finished because we didn't manage our time. We learned from our mistakes that we made on the last project. Unfortunately, we had finished code that worked but we couldn't tune it to get the robot to actually follow a line. The robot is fully finished with it working it's just that the hardest part was getting the 4 IR sensors to detect the line. The biggest issue with the code was tuning it to read the line because we were trying to get the farthest left and right sensors to turn when they detected a white line and have the middle sensors detect the black tape. 
