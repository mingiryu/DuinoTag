#include <IRremote.h>
#include "pitches.h"

// IRremote Setting
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

IRsend irsend;

// Digital Input & Output
int triggerPin = 3; // trigger for firing
int speakerPin = 4; // Peizo Sounder
int hitPin = 7; //LED output used to indicated when the player is hit


// Player Status
int ammo = 0; // Current ammo, initialised in configue()
int life = 0; // Current life, initialised in configue()
int TR = 0; // Trigger reading, used for trigger()
int LTR = 0; // Last trigger reading, used for trigger()

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  //need the transmitter

  pinMode(triggerPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(hitPin, OUTPUT);

  configure();
}

void loop() {
  receiveIR();
  trigger();
}

void configure() {
  ammo = 30;
  life = 3;
}

// Receive IR signal and differenciate it
void receiveIR() {
  double remote = 1641445511;
  double longRemote = 4294967295;
  double instructableGun = 3389625022;
  if (irrecv.decode(&results)) { // checks if it is decoded
    if (results.decode_type == SONY){ // Checks for SONY
      Serial.println("SONY");
      irrev,resume();
    }
    else if (results.value == remote) { // remote
      Serial.println("Remote"); // print type of signal on monitor
      irrecv.resume(); // reinitialize the reciever
    }
    else if (results.value == longRemote) { // remote (long press)
      Serial.println("Remote (Long Press)");
      irrecv.resume();
    }
    else if (results.value == instructableGun) { // instructable gun
      Serial.println("Gun");
      irrecv.resume();
      hit();
    }
    else {
      Serial.println(results.value); // decode the code if not registered
      irrecv.resume();
    }
  }
  delay(500); // To stop geting too much hit
}

void hit() {
  if(life == 0) {
    Serial.println("Dead");
  } else {
    life = life - 1;
    Serial.println("Current life: " + life);
    playTone(400, 200);
  }
}

// recieve trigger signal and initiate shooting procedure
void trigger() {
  LTR = TR;
  TR = digitalRead(triggerPin);
  if(TR != LTR && TR == LOW){ // checks if trigger is in right condition
    if(ammo > 0 && life > 0) { // checks for player status
      shoot(); // intiate shooting
    }
  }
}

void shoot() {
  irsend.sendSony(0xa90, 12); // send sony, not sure about the args(HEX, int)
  delay(40);
  playTone(400,200);
}

// Standard sound generator for peizo
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
}
