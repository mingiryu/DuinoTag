#include <IRremote.h>
#include "pitches.h"

// IRremote Setting
int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);
decode_results results;

// Digital Input & Output
int triggerPin = 3; // Push button for primary fire

// Player Status
int ammo = 0; // Current ammo
int life = 0; // Current life
int TR = 0; // Trigger reading
int LTR = 0; // Last trigger reading

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(triggerPin, INPUT);
  
  configure(); // configure player status
}

void loop() {
  receiveIR();
  trigger();
}

void configure() {
  ammo = 30;
  life = 3;
}

void receiveIR() { // Receive IR signal and differenciate it
  if (irrecv.decode(&results)) {
    switch(results.value) {
      case 1641445511: // signal from remote
        Serial.println("Remote"); // print type of signal on monitor
        irrecv.resume(); // reinitialize the reciever
        break;
      case 4294967295: // remote (long press)
        Serial.println("Remote (Long Press)");
        irrecv.resume();
        break;
      case 3389625022: // gun
        Serial.println("Gun");
        irrecv.resume();
        hit();
        break;
      default:
        Serial.println(results.value); // decode the code if not registered
        irrecv.resume();
    }
  }
  delay(500); // To stop geting too much hit
}

void hit() {
  life = life - 1;
  Serial.println("Current life" + life);
}

void trigger() { // recieve trigger signal and initiate shooting procedure
  LTR = TR;
  TR = digitalRead(triggerPin);
  if(TR != LTR && TR == LOW){ // checks if condition is met
    if(ammo > 0 && life > 0) { // checks for player status
      shoot(); // intiate shooting
    }
  }
}

void shoot() {
  
}







