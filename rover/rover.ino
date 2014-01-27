#include "variant.h"
#include <stdio.h>
#include <adk.h>
#include "DualMC33926MotorShield.h"

#define BUFFSIZE   128
#define MAX_POWER  400
#define MIN_POWER -400

// Accessory descriptor. It's how Arduino identifies itself in Android.
char accessoryName[] = "UDOO Silkworm jeep";
char manufacturer[] = "Example, Inc.";
char model[] = "UDOO-Silkworm";
 
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
  Serial.println("All power to the engines!");
  delay(2000);
  
  motor.init();
}

void loop() {
  Usb.Task();

  // Starting listening when ADK is available
  if (adk.isReady()) {
      adk.read(&bytesRead, BUFFSIZE, buffer);
    if (bytesRead > 0) {
      jeepCommandInterpreter(buffer[0], buffer[1]);
    }
  }
}

void jeepCommandInterpreter(uint8_t command, uint8_t speed) {
  switch(command) {
    case 0:
      goForward(speed);
      break;
    case 1:
      goBackward(speed);
      break;
    case 2:
      goLeft(speed);
      break;
    case 3:
      goRight(speed);
      break;
    default:
      Serial.println("Command not available");
      break;
  }
}

// Main movement command
void goForward(uint8_t speed) {
}

void goBackward(uint8_t speed) {
  turnBack();
  goForward(speed);
}

void goLeft(uint8_t speed) {
  turnLeft();
  goForward(speed);
}

void goRight(uint8_t speed) {
  turnRight();
  goForward(speed);
}

// Helper movement
int speedToPower(uint8_t speed) {
  int v;
  switch(speed) {
    case 1:
      v = 150;
      break;
    case 2:
      v = 250;
      break;
    case 3:
      v = 300;
      break;
    default:
      v = 0;
      break;
  }
  
  // Return a safe motor power
  return motorProtection(v);
}

void turnLeft() {
}

void turnRight() {
}

void turnBack() {
}

void stopEngine() {
  motor.setM1Speed(0);
  motor.setM2Speed(0);
}

// Emergency checks
int motorProtection(int v) {
  if (v <= MAX_POWER && v >= MIN_POWER) {
    return v;
  } else {
    return 0;
  }
}

