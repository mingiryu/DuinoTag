#include <IRremote.h>

// IRremote Setting
int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

IRsend irsend;

// Digital Input & Output
int triggerPin = 2; // trigger for firing
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

  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(hitPin, OUTPUT);

  configure();
  Serial.println("Configured");
  playTone(400,200);
}

void loop() {
  receiveIR();
  //delay(10);
  trigger();
  //delay(10);
}

void configure() {
  ammo = 30;
  life = 30;
}

// Receive IR signal and differenciate it
void receiveIR() { // Receive IR signal and differenciate it
  if (irrecv.decode(&results)) { // checks if it is decoded
    switch(results.value) {
      case 2704:
        Serial.println("SONY");
        irrecv.resume();
        hit();
        break;
      default:
        Serial.println(results.value); // decode the code if not registered
        irrecv.resume();
    }
  }

  delay(50); // To stop geting too much hit
}

void hit() {
  if(life == 0) {
    Serial.println("Dead");
    for (int i = 1;i < 254;i++) {
      playTone((1000+9*i), 2);
    }
  } else {
    life = life - 1;
    Serial.println(life);
    playTone(400, 200);
  }
}

// recieve trigger signal and initiate shooting procedure
void trigger() {
  LTR = TR;
  TR = digitalRead(triggerPin);
  if(TR != LTR && TR == LOW){ // checks if trigger is in right condition
    if(ammo > 0 && life > 0) { // checks for player status
      irsend.sendSony(0xa90, 12); // send sony, not sure about the args(HEX, int)
      ammo--;
      delay(40);
      playTone(400,200);
      irrecv.enableIRIn();
    }
  }
}

// Standard sound generator for peizo
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
