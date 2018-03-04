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
//int motorPin = 10; // Vibrating motor output for physical feedback

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
    currentAmmo = 10;
    currentLife = 10;
    teamNumber = 0; // 0 or 1
    for (int i = 1;i < 254;i++) {
        playTone((100+9*i), 2);
    }
    Serial.println("Configuration Completed");
}

// Receive IR signal and differenciate it
void receiveIR() {
    if (irrecv.decode(&results)) {
        switch(results.value) {
            case 2704: // Sony
                irrecv.resume();
                playerHit(0);
                break;
            case 1524048797: // NEC
                irrecv.resume();
                playerHit(1);
                break;
            default:
                Serial.println(results.value); // Decode the code and display the code, if not registered
                irrecv.resume();
        }
    }
    
    delay(100); // To stop geting too much hit
}

// Update value of currentLife and make noise after being hit
void playerHit(int typeOfSignal) {
    if (teamNumber == typeOfSignal) {
        Serial.println("Signal from same team");
    } else {
        if (currentLife == 0) {
            Serial.println("Player is Dead");
            
            // Plays death melody
            for (int i = 254;i > 1;i--) {
                playTone((100+9*i), 2);
            }
        } else {
            // Subtract 1 life and reconfigure the pins
            currentLife--;
            playTone(400, 200);
            Serial.println(currentLife);
            
            digitalWrite(hitPin, HIGH);
            digitalWrite(motorPin, HIGH);
            delay(200);
            digitalWrite(hitPin, LOW);
            digitalWrite(motorPin, LOW);
            
            analogWrite(lifePin, (currentLife/30.0)*255);
        }
    }
}

// Monitor trigger status and calls shooting method when approperiate
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

// Send Sony or NEC remote signal depending on teamNumber
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
    }
    playTone(400,200);
    currentAmmo--;
    analogWrite(ammoPin, (currentAmmo/30.0)*255);
    delay(50);
    irrecv.enableIRIn(); // IReciever needs to be started again after shooting
}

// Reload currentAmmo
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
