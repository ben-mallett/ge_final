#include <Arduino.h>

void handle_button_input(void);
void set_status_indicators(void);
void check_status(void);
void indicators_on_no_violation(void);
void indicators_on_violation(void);
void indicators_off(void);
void rotate_motor(void);

#define GREEN_LED_PIN 2
#define RED_LED_PIN 3
#define SPEAKER_PIN 4
#define PHOTORESISTOR_PIN_ONE 6
#define PHOTORESISTOR_APIN_ONE A0 
#define PHOTORESISTOR_PIN_TWO 7
#define PHOTORESISTOR_APIN_TWO A1
#define PHOTORESISTOR_PIN_THREE 8
#define PHOTORESISTOR_APIN_THREE A2
#define BUTTON_PIN 5
#define SOLENOID_PIN 8 
#define BEAM_BREAK_THRESHOLD 100
#define BAUD_RATE 9600
#define SMALL_DELAY 10
#define ALARM_FREQUENCY 400
#define ALARM_DURATION 300
#define LASER_PIN_ONE 9
#define LASER_PIN_TWO 10
#define LASER_PIN_THREE 11


bool system_on = false;
bool security_violation = false;
int button_voltage;
int photoresistor_voltage;
int laser_pins[] = {LASER_PIN_ONE, LASER_PIN_TWO, LASER_PIN_THREE};
int photoresistor_pins[] = {PHOTORESISTOR_PIN_ONE, PHOTORESISTOR_PIN_TWO, PHOTORESISTOR_PIN_THREE};
int photoresistor_apins[] = {PHOTORESISTOR_APIN_ONE, PHOTORESISTOR_APIN_TWO, PHOTORESISTOR_APIN_THREE};



/**
 * Performs setup operations like initializing pines.
*/
void setup() {
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(SPEAKER_PIN, OUTPUT); 
  pinMode(SOLENOID_PIN, OUTPUT);

  for (int pin : laser_pins) {
    pinMode(pin, OUTPUT);
  }

  Serial.begin(BAUD_RATE);
}

/**
 * Main control of program. 
 * 
 * Handles inputs and monitors security systems
*/
void loop() {
  if (system_on) {
    check_status();
    rotate_motor();
  }

  handle_button_input();
  set_status_indicators();
  delay(SMALL_DELAY);
}

/**
 * Handles button inputs. Toggles system state + resets violation status
*/
void handle_button_input(void) {
  button_voltage = digitalRead(BUTTON_PIN);
  if (button_voltage == LOW) {
    system_on = !system_on;
    security_violation = false; // regardless of what power status is triggered we can reset security violations
  }
}

/**
 * Determines which status indicators should be displayed based on system status.
*/
void set_status_indicators(void) {
  if (system_on && !security_violation) {
    indicators_on_no_violation();
  } else if (system_on && security_violation) {
    indicators_on_violation();
  } else {
    indicators_off();
  }
}

/**
 * Sets the indicators for system on, no violations.
 * 
 * This means
 * - Lasers on
 * - Photoresistors on
 * - Solenoid unlocked
 * - Green LED on
 * - Red LED off
 * - Motor on
*/
void indicators_on_no_violation(void) {
  // Turn lasers on
  for (int pin : laser_pins) {
    digitalWrite(pin, HIGH);
  }

  // Turn photoresistors on
  for (int pin : photoresistor_pins) {
    digitalWrite(pin, HIGH);
  }

  digitalWrite(SOLENOID_PIN, LOW);   // ensures deadbolt is unlocked
  digitalWrite(GREEN_LED_PIN, HIGH); // gives indication system is all good
  digitalWrite(RED_LED_PIN, LOW);    // ensures alarm indicator is off

  // TODO: Turn motor on
}

/**
 * Sets the indicators for system on, violations.
 * 
 * This means
 * - Solenoid locked
 * - Green LED off
 * - Red LED on
 * - Motor off
*/
void indicators_on_violation(void) {
  digitalWrite(SOLENOID_PIN, HIGH);  // locks door on security violation
  digitalWrite(GREEN_LED_PIN, LOW);  // ensures all good is off
  digitalWrite(RED_LED_PIN, HIGH);   // gives indication system detected alarm

  // plays alarm on security violation
  tone(SPEAKER_PIN, ALARM_FREQUENCY, ALARM_DURATION);
  delay(SMALL_DELAY);
  tone(SPEAKER_PIN, ALARM_FREQUENCY, ALARM_DURATION);

  // TODO: turn motor off
}

/**
 * Sets the indicators for the off system
 * 
 * This means
 * - Lasers off
 * - Photoresistors off
 * - Solenoid unlocked
 * - Green LED off
 * - Red LED off
 * - Motor off
*/
void indicators_off(void) {
  // Turn lasers off
  for (int pin : laser_pins) {
    digitalWrite(pin, LOW);
  }

  // Turn photoresistors off
  for (int pin : photoresistor_pins) {
    digitalWrite(pin, LOW);
  }

  digitalWrite(SOLENOID_PIN, LOW);   // deadbolt unlocked on inactive system
  digitalWrite(GREEN_LED_PIN, LOW);  // LED off on inactive system
  digitalWrite(RED_LED_PIN, LOW);    // LED off on inactive system

  // TODO: Turn motor off
}

/**
 * Checks and sets the status of the system, namely whether or not any of the laser beams have been broken.
*/
void check_status(void) {
  for (int pr : photoresistor_apins) {
    // TODO: Consider adding debouncing here to stablize the value. Weigh with timing concerns
    if (analogRead(pr) < BEAM_BREAK_THRESHOLD) {
      security_violation = true;
    }
  }
}

/**
 * Rotates the motor to a random interval
*/
void rotate_motor(void) {
  // TODO: rotate the motor
  int x = 1;
}

