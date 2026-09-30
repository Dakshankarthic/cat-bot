#include <Arduino.h>
#include <ESP32Servo.h>

// ======================================================================
// --- CONFIGURATION ---
// ======================================================================

Servo servos[8];

// Motor Pin Mapping
// Index: 0  1  2  3  4   5   6   7
// const int servoPins[8] = {1, 2, 4, 6, 8, 10, 13, 14};
// Distro board V3 pins: (uncomment if needed)
// const int servoPins[8] = {4, 5, 6, 7, 10, 11, 12, 13};
// Distro board V2 pins: (uncomment if needed)
const int servoPins[8] = {4, 5, 6, 7, 15, 16, 17, 18}; // Distro board V2
// Distro board V1 pins: (uncomment if needed)
// const int servoPins[8] = {15, 2, 23, 19, 4, 16, 17, 18};



// Pulse width settings for standard MG90S / SG90 servos
// 500us (0 deg) to 2500us (180 deg), Center = 1500us (90 deg)
const int MIN_PULSE = 500;
const int MAX_PULSE = 2500;

// ======================================================================
// --- SETUP ---
// ======================================================================

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("--------------------------------------------------");
  Serial.println("         Sesame Motor Tester & Diagnostic         ");
  Serial.println("--------------------------------------------------");
  Serial.println("Commands:");
  Serial.println("1. id,angle  (or 'id angle')   -> e.g. '0,90' or '0 90'");
  Serial.println("2. all,angle (or 'all angle')  -> e.g. 'all,90' or 'all 90'");
  Serial.println("3. us,microseconds             -> e.g. 'us,1500' (test raw pulse)");
  Serial.println("4. stop                        -> Detaches/Powers down motors");
  Serial.println("--------------------------------------------------");
  Serial.println("DIAGNOSTIC TEST FOR CONTINUOUS ROTATION VS 180 SERVO:");
  Serial.println("- If 90 stops, but 0 and 180 spin endlessly: you have a 360-DEGREE CONTINUOUS servo.");
  Serial.println("- 180-degree servos MUST move to position and lock stiff.");
  Serial.println("--------------------------------------------------");
  Serial.println("Status: Motors are currently OFF (Limp).");

  // Allocate timers for ESP32Servo
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // We do NOT attach motors here. They stay limp until a command is sent.
}

// ======================================================================
// --- MAIN LOOP ---
// ======================================================================

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove whitespace/newlines

    if (input.length() == 0) return;

    // --- Check for STOP command ---
    if (input.equalsIgnoreCase("stop")) {
      stopMotors();
      return;
    }

    // Convert spaces to commas for flexible input: 'all 90' -> 'all,90'
    input.replace(' ', ',');

    // --- Check for raw microsecond command: 'us,1500' ---
    if (input.startsWith("us,") || input.startsWith("pulse,")) {
      int commaIndex = input.indexOf(',');
      int us = input.substring(commaIndex + 1).toInt();
      for (int i = 0; i < 8; i++) {
        if (!servos[i].attached()) {
          servos[i].setPeriodHertz(50);
          servos[i].attach(servoPins[i], MIN_PULSE, MAX_PULSE);
        }
        servos[i].writeMicroseconds(us);
      }
      Serial.print("All servos set to raw pulse: ");
      Serial.print(us);
      Serial.println(" us (1500 us = stop for 360 deg servo, center for 180 deg servo)");
      return;
    }

    // --- Parse "key,value" format ---
    int commaIndex = input.indexOf(',');
    
    if (commaIndex != -1) {
      String cmd = input.substring(0, commaIndex);
      String valStr = input.substring(commaIndex + 1);
      int angle = valStr.toInt();

      // Check Angle Limits
      if (angle < 0) angle = 0;
      if (angle > 180) angle = 180;

      // Check if command is "all" or a motor ID
      if (cmd.equalsIgnoreCase("all")) {
        moveAll(angle);
      } else {
        // Assume it's a number
        int motorId = cmd.toInt();
        if (motorId == 0 && cmd.charAt(0) != '0') {
           Serial.println("Error: Invalid Motor ID");
        } else {
           moveMotor(motorId, angle);
        }
      }
      
    } else {
      Serial.println("Error: Invalid format. Use 'id,angle', 'all,angle', 'us,1500', or 'stop'.");
    }
  }
}

// ======================================================================
// --- HELPER FUNCTIONS ---
// ======================================================================

void moveMotor(int id, int angle) {
  if (id < 0 || id > 7) {
    Serial.println("Error: Motor ID must be 0-7");
    return;
  }

  // If servo isn't attached yet (powered off), attach it now
  if (!servos[id].attached()) {
    servos[id].setPeriodHertz(50);
    servos[id].attach(servoPins[id], MIN_PULSE, MAX_PULSE);
  }

  servos[id].write(angle);
  int pulseUs = map(angle, 0, 180, MIN_PULSE, MAX_PULSE);
  
  Serial.print("OK: Motor ");
  Serial.print(id);
  Serial.print(" -> ");
  Serial.print(angle);
  Serial.print(" deg (pulse: ");
  Serial.print(pulseUs);
  Serial.println(" us)");
}

void moveAll(int angle) {
  int pulseUs = map(angle, 0, 180, MIN_PULSE, MAX_PULSE);
  Serial.print("Moving ALL to ");
  Serial.print(angle);
  Serial.print(" deg (pulse: ");
  Serial.print(pulseUs);
  Serial.println(" us)");
  for (int i = 0; i < 8; i++) {
    moveMotor(i, angle);
  }
}

void stopMotors() {
  Serial.println("Stopping (Detaching) all motors...");
  for (int i = 0; i < 8; i++) {
    if (servos[i].attached()) {
      servos[i].detach();
    }
  }
  Serial.println("Motors are now OFF.");
}
