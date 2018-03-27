#include <IRremote.h> // Required for IR system
#include <NewTone.h> // Required for compatibility with IRremote

// IRremote Setting
int RECV_PIN = 12; // IR receiver
IRrecv irrecv(RECV_PIN);
decode_results results;
IRsend irsend;

// Digital Input & Output
int triggerPin = 2; // Trigger for firing
int speakerPin = 4; // Peizo Sounder
int hitPin = 7; // LED output used to indicated when the player is hit
int ammoPin = 6; // LED output for indicating remaining ammo
int lifePin = 5; // LED output for indicating remaining life
int motorPin = 10; // Vibrating motor output for physical feedback

// Game Attributes
double currentAmmo = 0.0;
double currentLife = 0.0;
int triggerReading = 0;
int lastTriggerReading = 0;
int teamNumber = 0;

void setup() {
    Serial.begin(9600);
    irrecv.enableIRIn();
    pinMode(triggerPin, INPUT_PULLUP);
    pinMode(speakerPin, OUTPUT);
    pinMode(hitPin, OUTPUT);
    pinMode(ammoPin, OUTPUT);
    pinMode(lifePin, OUTPUT);
    pinMode(motorPin, OUTPUT);
    analogWrite(ammoPin,255);
    analogWrite(lifePin,255);
    
    configure();
}

// Regularly checks IR signal and trigger status
void loop() {
    receiveIR();
    delay(10);
    checkTriggerStatus();
}

// Change the values based on preference
void configure() {
    Serial.println("Configured");
    currentAmmo = 30;
    currentLife = 5;
    teamNumber = 1;
    for (int i = 1;i < 254;i++) {
        playTone((100+9*i), 2);
    }
}

// Receive IR signal and differenciate it
void receiveIR() {
    if (irrecv.decode(&results)) {
        switch(results.value) {
            case 2704: // Sony, 0
                irrecv.resume();
                if (teamNumber == 0) {
                  Serial.println("Same team");
                } else {
                  playerHit();
                }
                break;
            case 1524048797: // NEC, 1
                irrecv.resume();
                if (teamNumber == 1) {
                  Serial.println("Same team");
                } else {
                  playerHit();
                }
                break;
            default:
                Serial.println(results.value);
                irrecv.resume();
        }
    }
    delay(100);
}

// Update currentLife value and pins
void playerHit() {
    if (currentLife == 0) {
        Serial.println("Dead");
        for (int i = 254;i > 1;i--) {
            playTone((100+9*i), 2);
        }
    } else {
        currentLife--;
        Serial.println(currentLife);
        analogWrite(lifePin, (currentLife/30.0)*255);
        digitalWrite(motorPin, HIGH);
        for (int i = 0; i < 3; i++) {
          digitalWrite(hitPin, HIGH);
          delay(50);
          digitalWrite(hitPin, LOW);
          delay(200);
        }
        digitalWrite(motorPin, LOW);
    }
}

// Monitor trigger status and calls shooting method when approperiate
void checkTriggerStatus() {
    lastTriggerReading = triggerReading;
    triggerReading = digitalRead(triggerPin);
    if(triggerReading != lastTriggerReading && triggerReading == LOW) {
        if(currentAmmo * currentLife != 0) {
            shoot();
        }
    }
}

// Send signal that corresponds to teamNumber
void shoot() {
    Serial.println("Shoot!");
    for (int i = 0; i < 3; i++) {
      switch(teamNumber) {
        case 0: // Sony
            irsend.sendSony(0xa90, 12);
            delay(5);
            break;
        case 1: // NEC
            irsend.sendNEC(0xa90, 12);
            delay(5);
            break;
        default:
            delay(5);
       }
       digitalWrite(motorPin, HIGH);
       delay(10);
       digitalWrite(motorPin, LOW);
    }
    playTone(200,200);
    currentAmmo--;
    analogWrite(ammoPin, (currentAmmo/30.0)*255);
    irrecv.enableIRIn(); 
    delay(10);
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
