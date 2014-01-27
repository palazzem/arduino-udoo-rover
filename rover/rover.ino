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
      jeepCommandInterpreter(buffer[0]);
    }
  }
}

void jeepCommandInterpreter(uint8_t command) {
  switch(command) {
    default:
      Serial.println("Command not available");
      break;
  }
}
