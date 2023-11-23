#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
#define Green_LED 2
#define Red_LED 3
#define Speaker 4
#define phresPin 12
#define photores A0 
#define buttonPin 5
#define solenoidPin 8 
int buttonVoltage;
int prVoltage;
int thresholddark = 100;
int thresholdmedium = 300;
int thresholdbright=750;


void setup() {
  pinMode(correctLEDpin,OUTPUT);
  pinMode(incorrectLEDpin,OUTPUT);
  pinMode(buttonPIN, INPUT_PULLUP); 
  pinMode(speaker, OUTPUT); 
  Serial.begin(9600);
  pinMode(solenoidPin, OUTPUT)

}

void loop() {
 buttonVoltage= digitalRead(buttonPin);
  if(buttonVoltage == LOW) {
      digitalWrite(solenoidPin, LOW);//make sure door isnt deadbolted 
      digitalWrite(greenLED, HIGH);//turn on LED to indicate the system is on 
    //laser movement and such  
    //if laser is broken
      digitalWrite(solenoidPin, HIGH);      //Switch Solenoid ON
      digitalWrite(greenLED, LOW);
      digitalWrite (redLED, HIGH);
      tone (speaker, 400, 300);
      digitalWrite (redLED, LOW);
      tone (speaker, 400, 300);
    
      }
    else{
      delay(100);
    }
    }
   
     
      }
   

}
