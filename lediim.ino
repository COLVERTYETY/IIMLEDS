#include <FastLED.h>


#define NUM_LEDS 60 //total amount of leds
#define DATA_PIN 3
int trigPin = 8;    // Trigger
int  echoPin = 9;    // Echo
CHSV colora = CHSV(0, 255, 255);//format de couleur hsv du style (couleur,saturation,intensité) utilise just un picker en ligne
CHSV colorb = CHSV(50, 255, 255);
CHSV colorc = CHSV(128, 255, 255);
CHSV oldcolor = CHSV(0, 100, 0);
int stepduration = 1; // duration de chaque étape de la transition, y a genre 255 étape en tous pas trop sure, min = 1 c est en miliseconde
long duration, cm, inches;
int mindist= 10; //ici  c est la distance minimal en cm pas trop précis mais ca va
int state = 1;//couleur de départ
// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  if (cm<=mindist){ //if sensor is less then give, distance
    state ++; //increase state
    if (state>3){ //loop through states
      state = 1;//first state
    }
    CHSV activecolor = CHSV(0,0,0); //these colors are used for thz animation, not the best but works
    CHSV targetcolor = CHSV(0, 0, 0);
    switch (state){//switch case which links state to color
      case 1:
        targetcolor = colora;
        break;
      case 2:
        targetcolor = colorb;
        break;
      case 3:
        targetcolor = colorc;
        break;
    }
    for( int i=0;i<255;i++){ //this is the main animation loop
      activecolor = blend(oldcolor,targetcolor , i , SHORTEST_HUES);//calculate the inbetween color 
      fill_solid( leds, NUM_LEDS, activecolor);//fill all leds with the same color 
      FastLED.show();//display the change
      delay(stepduration);//wait for a given time
    }
    oldcolor = targetcolor; 
    Serial.println(state);//display the current state only needed for debugging
  }
}
