#include "variant.h"
#include <stdio.h>
#include <adk.h>
#include "DualMC33926MotorShield.h"

#define BUFFSIZE   128
#define MAX_POWER  400

// Command interpreter logic
const static boolean DEBUG = false;

// Accessory descriptor. It's how Arduino identifies itself in Android.
char accessoryName[] = "UDOO DroidRover";
char manufacturer[] = "Example, Inc.";
char model[] = "UDOO-Rover";

char versionNumber[] = "0.1.0";
char serialNumber[] = "1";
char url[] = "http://www.example.com";

// ADK configuration
USBHost Usb;
ADK adk(&Usb, manufacturer, model, accessoryName, versionNumber, url, serialNumber);
uint8_t buffer[BUFFSIZE];
uint32_t bytesRead = 0;

// MC33926 motor shield configuration
DualMC33926MotorShield motor;

void setup() {
  cpu_irq_enable();
  Serial.begin(115200);
  delay(1000);
  Serial.println("All power to the engines!");
  delay(1000);

  motor.init();
  stopEngine();
}

void loop() {
  if (DEBUG) {
    readingFromSerial();
  } else {
    readingFromADK();
  }
}

void readingFromADK() {
  Usb.Task();

  // Starting listening when ADK is available
  if (adk.isReady()) {
      adk.read(&bytesRead, BUFFSIZE, buffer);
    if (bytesRead > 0) {
      jeepCommandInterpreter(extractMovement(), extractSpeed());
    }
  }
}

void jeepCommandInterpreter(uint8_t commandMovement, unsigned int commandSpeed) {
  unsigned int vPower = motorProtection(commandSpeed);
  Serial.print("Required power: ");Serial.println(vPower);

  switch(commandMovement) {
    case 0:
      Serial.println("Received command: 0 -> move forward");
      goForward(vPower);
      break;
    case 1:
      Serial.println("Received command: 1 -> move backward");
      goBackward(vPower);
      break;
    case 2:
      Serial.println("Received command: 2 -> turn left");
      turnLeft(vPower);
      break;
    case 3:
      Serial.println("Received command: 3 -> turn right");
      turnRight(vPower);
      break;
    case 4:
      Serial.println("Received command: 4 -> turn back");
      turnBack(vPower);
      break;
    case 5:
      Serial.println("Received command: 5 -> testing all movements");
      testAllMovements(vPower);
      break;
    default:
      Serial.println("Command not available");
      break;
  }
}

// Main movement command
void goForward(int vPower) {
  motor.setM1Speed(vPower);
  motor.setM2Speed(vPower);

  delay(1000);
  stopEngine();
}
void goBackward(int vPower) {
  motor.setM1Speed(-vPower);
  motor.setM2Speed(-vPower);

  delay(1000);
  stopEngine();
}

void turnLeft(int vPower) {
  motor.setM1Speed(-vPower);
  motor.setM2Speed(vPower);

  delay(500);
  stopEngine();
}

void turnRight(int vPower) {
  motor.setM1Speed(vPower);
  motor.setM2Speed(-vPower);

  delay(500);
  stopEngine();
}

void turnBack(int vPower) {
  motor.setM1Speed(vPower);
  motor.setM2Speed(-vPower);

  delay(1000);
  stopEngine();
}

void stopEngine() {
  motor.setM1Speed(0);
  motor.setM2Speed(0);
}

// Emergency checks
unsigned int motorProtection(int v) {
  if (v >= 0 && v <= MAX_POWER) {
    return v;
  } else {
    Serial.println("DANGER: too much V!");
    return 0;
  }
}

// Testing rover with Serial port
void readingFromSerial() {
  int bytesRead = 0;
  while (Serial.available() > 0) {
    if (bytesRead < BUFFSIZE) {
      buffer[bytesRead] = Serial.read();
      bytesRead++;
    }
    delay(20);
  }

  if (bytesRead == 2) {
    // Send commands to internal controller
    jeepCommandInterpreter(extractMovement(), extractSpeed());
    resetBuffer();
  }
}

void testAllMovements(int vPower) {
  jeepCommandInterpreter(0, vPower);
  jeepCommandInterpreter(1, vPower);
  jeepCommandInterpreter(2, vPower);
  jeepCommandInterpreter(3, vPower);

  stopEngine();
}

// Generic helpers
void resetBuffer() {
  int index = 0;
  while (index < BUFFSIZE) {
    buffer[index] = 0;
    index++;
  }
}

uint8_t extractMovement() {
  return buffer[0] - 48;
}

unsigned int extractSpeed() {
  uint8_t speedBuffer[3] = { 45, 45, 45 };

  int i = 0;
  while (buffer[i + 2] >= 48 && i < 3) {
    speedBuffer[i] = buffer[i + 2];
    i++;
  }

  return atoi((char*)speedBuffer);
}
