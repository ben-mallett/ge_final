#include <Arduino.h>
#include <Servo.h>

void handle_button_input(void);
void set_status_indicators(void);
void check_status(void);
void indicators_on_no_violation(void);
void indicators_on_violation(void);
void indicators_off(void);
float get_distance(void);

#define GREEN_LED_PIN 2
#define RED_LED_PIN 3
#define SPEAKER_PIN 4
#define PHOTORESISTOR_PIN_ONE 6
#define BUTTON_PIN 5
#define SERVO_PIN 8 
#define BEAM_BREAK_THRESHOLD 100
#define BAUD_RATE 9600
#define SMALL_DELAY 10
#define LARGE_DELAY 300
#define ALARM_FREQUENCY 400
#define ALARM_DURATION 300
#define LASER_PIN_ONE 9
#define UNLOCKED_ROTATION 0
#define LOCKED_ROTATION 90
#define US_TRIG_PIN 12
#define US_ECHO_PIN 11
#define ROUND_TRIP_FACTOR 2
#define PULSE_LENGTH_TO_CM_CONV_FACTOR 29.1
#define SIGNAL_INTEGRITY_DELAY_US 5
#define US_TRIGGER_TIME 10
#define US_DISTANCE_THRESHOLD 3

bool system_on = false;
bool security_violation = false;
int button_voltage;
int photoresistor_voltage;
int laser_pins[] = {LASER_PIN_ONE};
int photoresistor_pins[] = {PHOTORESISTOR_PIN_ONE};

Servo servoLock;



/**
 * Performs setup operations like initializing pines.
*/
void setup() {
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(SPEAKER_PIN, OUTPUT); 
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(US_TRIG_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);

  for (int pin : laser_pins) {
    pinMode(pin, OUTPUT);
  }

  for (int pin : photoresistor_pins) {
    pinMode(pin, INPUT);
  }

  servoLock.attach(SERVO_PIN);

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
    servoLock.write(system_on ? LOCKED_ROTATION : UNLOCKED_ROTATION);
    delay(LARGE_DELAY);
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
 * - Servo locked
 * - Green LED on
 * - Red LED off
 * - Motor on
*/
void indicators_on_no_violation(void) {
  // Turn lasers on
  for (int pin : laser_pins) {
    digitalWrite(pin, HIGH);
  }

  digitalWrite(GREEN_LED_PIN, HIGH); // gives indication system is all good
  digitalWrite(RED_LED_PIN, LOW);    // ensures alarm indicator is off
}

/**
 * Sets the indicators for system on, violations.
 * 
 * This means
 * - Servo locked
 * - Green LED off
 * - Red LED on
 * - Motor off
*/
void indicators_on_violation(void) {
  for (int pin : laser_pins) {
    digitalWrite(pin, LOW);
  }

  digitalWrite(GREEN_LED_PIN, LOW);  // ensures all good is off
  digitalWrite(RED_LED_PIN, HIGH);   // gives indication system detected alarm


  // plays alarm on security violation
  tone(SPEAKER_PIN, ALARM_FREQUENCY, ALARM_DURATION);
}

/**
 * Sets the indicators for the off system
 * 
 * This means
 * - Lasers off
 * - Photoresistors off
 * - Servo unlocked
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

  digitalWrite(GREEN_LED_PIN, LOW);   // LED off on inactive system
  digitalWrite(RED_LED_PIN, LOW);     // LED off on inactive system
}

/**
 * Checks and sets the status of the system, namely whether or not any of the laser beams have been broken.
*/
void check_status(void) {
  get_distance();

  // if (cm < US_DISTANCE_THRESHOLD) {
  //   security_violation = true;
  // }
  for (int pr : photoresistor_pins) {
    if (digitalRead(pr) == HIGH) {
      security_violation = true;
    }
  }
}

/**
 * Gets the distance of the object in front of the ultrasonic sensor
*/
float get_distance() {
  // trigger a reading
  digitalWrite(US_TRIG_PIN, LOW);
  delayMicroseconds(SIGNAL_INTEGRITY_DELAY_US);
  digitalWrite(US_TRIG_PIN, HIGH);
  delayMicroseconds(US_TRIGGER_TIME);
  digitalWrite(US_TRIG_PIN, LOW);

  pinMode(US_ECHO_PIN, INPUT);
  unsigned long duration = pulseIn(US_ECHO_PIN, HIGH, 1000000UL);
 
  return (duration/ROUND_TRIP_FACTOR) / PULSE_LENGTH_TO_CM_CONV_FACTOR;   
}

