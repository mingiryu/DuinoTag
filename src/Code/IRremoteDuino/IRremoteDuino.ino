#include <IRremote.h>

// IRremote Setting
int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;
IRsend irsend;

// Digital Input & Output
int triggerPin = 2; // Trigger for firing
int speakerPin = 4; // Peizo Sounder
int hitPin = 7; // LED output used to indicated when the player is hit
int ammoPin = 6; // LED output for indicating remaining ammo

// Game Attributes
double currentAmmo = 0.0;
int currentLife = 0;
int triggerReading = 0;
int lastTriggerReading = 0;
int teamNumber = 0;

void setup() {
    Serial.begin(9600);
    irrecv.enableIRIn(); // Start the receiver
    
    // pin configuration
    pinMode(triggerPin, INPUT_PULLUP);
    pinMode(speakerPin, OUTPUT);
    pinMode(hitPin, OUTPUT);
    pinMode(ammoPin, OUTPUT);
    analogWrite(ammoPin,255);
    
    configure();
    Serial.println("Configured");
    playTone(400,200);
}

void loop() {
    receiveIR();
    checkTriggerStatus();
}

// teamNumber: 0 = Sony, 1 = Panasonic
void configure() {
    currentAmmo = 10;
    currentLife = 10;
    teamNumber = 0;
}

// Receive IR signal and differenciate it
void receiveIR() {
    if (irrecv.decode(&results)) {
        switch(results.value) {
            case 2704: // Sony
                Serial.println("SONY");
                irrecv.resume();
                playerHit(0);
                break;
            case 1111: // Panasonic, 1111 is a placeholder number
                Serial.println("Panasonic");
                irrecv.resume();
                playerHit(1);
                break;
            default:
                Serial.println(results.value); // decode the code if not registered
                irrecv.resume();
        }
    }
    
    delay(50); // To stop geting too much hit
}

// Update value of currentLife and make noise after being hit
void playerHit(int typeOfSignal) {
    if (teamNumber == typeOfSignal) {
        Serial.println("Signal from same team");
    } else {
        if (currentLife == 0) {
            Serial.println("Player is Dead");
            for (int i = 1;i < 254;i++) {
                playTone((1000+9*i), 2);
            }
        } else {
            currentLife--;
            Serial.println("Remaining life: " + currentLife);
            digitalWrite(hitPin, HIGH);
            playTone(400, 200);
            delay(200);
            digitalWrite(hitPin, LOW);
        }
    }
}

// monitor trigger status and calls shooting method when approperiate
void checkTriggerStatus() {
    lastTriggerReading = triggerReading;
    triggerReading = digitalRead(triggerPin);
    if(triggerReading != lastTriggerReading && triggerReading == LOW) {
        if(currentAmmo > 0 && currentLife > 0) {
            shoot();
        } else if (currentAmmo <= 0 && currentLife > 0) {
            reload();
        }
    }
}

// Send Sony or Panasonic remote signal depending on teamNumber
void shoot() {
    for (int i = 0; i<4; i++) {
        switch(teamNumber) {
            case 0: // Sony
                irsend.sendSony(0xa90, 12);
                delay(5);
                break;
            case 1: // Panasonic
                irsend.sendPanasonic(0x4004, 0x100BCBD);
                delay(5);
                break;
            default:
                delay(5);
        }
    }
    currentAmmo--;
    analogWrite(ammoPin, (currentAmmo/30.0)*255);
    delay(40);
    playTone(400,200);
    irrecv.enableIRIn(); // IReciever needs to be started again after shooting
}

void reload() {
  for (int i = 254;i < 1;i--) {
      playTone((1000+9*i), 2);
  }
  delay(40);
  currentAmmo = 10;
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
