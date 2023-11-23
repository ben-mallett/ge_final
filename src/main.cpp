#include <Arduino.h>

void handle_button_input(void);
void set_status_indicators(void);
void check_status(void);

#define GREEN_LED_PIN 2
#define RED_LED_PIN 3
#define SPEAKER_PIN 4
#define PHOTORESISTOR_PIN 12
#define PHOTORESISTOR_ANALOG_PIN A0 
#define BUTTON_PIN 5
#define SOLENOID_PIN 8 
#define BEAM_BREAK_THRESHOLD 100;
#define BAUD_RATE 9600
#define SMALL_DELAY 100
#define ALARM_FREQUENCY 400
#define ALARM_DURATION 300

bool system_on = false;
bool security_violation = false;
int button_voltage;
int photoresistor_voltage;


/**
 * Performs setup operations like initializing pines.
*/
void setup() {
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(SPEAKER_PIN, OUTPUT); 
  pinMode(SOLENOID_PIN, OUTPUT);

  // TODO: Initialize photoresistors and lasers, power lasers on

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
    digitalWrite(SOLENOID_PIN, LOW);   // ensures deadbolt is unlocked
    digitalWrite(GREEN_LED_PIN, HIGH); // gives indication system is all good
    digitalWrite(RED_LED_PIN, LOW);    // ensures alarm indicator is off
  } 

  else if (system_on && security_violation) {
    digitalWrite(SOLENOID_PIN, HIGH);  // locks door on security violation
    digitalWrite(GREEN_LED_PIN, LOW);  // ensures all good is off
    digitalWrite(RED_LED_PIN, HIGH);   // gives indication system detected alarm

    // plays alarm on security violation
    tone(SPEAKER_PIN, ALARM_FREQUENCY, ALARM_DURATION);
    delay(SMALL_DELAY);
    tone(SPEAKER_PIN, ALARM_FREQUENCY, ALARM_DURATION);
  } 

  else {
    digitalWrite(SOLENOID_PIN, LOW);   // deadbolt unlocked on inactive system
    digitalWrite(GREEN_LED_PIN, LOW);  // LED off on inactive system
    digitalWrite(RED_LED_PIN, LOW);    // LED off on inactive system
  }
}

/**
 * Checks and sets the status of the system, namely whether or not any of the laser beams have been broken.
*/
void check_status() {
  // TODO: iterate through laser beams and check if any photo resistor value is less than the threshold, if so trigger a security violation
  if (false) {
    security_violation = true;
  }
}

