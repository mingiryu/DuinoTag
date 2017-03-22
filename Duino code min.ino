

// Digital IO's
int triggerPin             = 3;      // Push button for primary fire. Low = pressed
int speakerPin             = 4;      // Direct output to piezo sounder/speaker
int lifePin                = 6;      // An analogue output (PWM) level corresponds to remaining life. Use PWM pin: 3,5,6,9,10 or 11. Can be used to drive LED bar graphs. eg LM3914N
int ammoPin                = 5;      // An analogue output (PWM) level corresponds to remaining ammunition. Use PWM pin: 3,5,6,9,10 or 11.
int hitPin                 = 7;      // LED output pin used to indicate when the player has been hit.
int IRtransmitPin          = 2;      // Primary fire mode IR transmitter pin: Use pins 2,4,7,8,12 or 13. DO NOT USE PWM pins!! More info: http://j44industries.blogspot.com/2009/09/arduino-frequency-generation.html#more
int IRreceivePin           = 12;     // The pin that incoming IR signals are read from
// Minimum gun requirements: trigger, receiver, IR led, hit LED.

// Player and Game details
int myTeamID               = 1;      // 1-7 (0 = system message)
int myPlayerID             = 1;      // Player ID
int myWeaponID             = 0;      // Deffined by gameType and configureGame subroutine.
int myWeaponHP             = 0;      // Deffined by gameType and configureGame subroutine.
int maxAmmo                = 0;      // Deffined by gameType and configureGame subroutine.
int maxLife                = 0;      // Deffined by gameType and configureGame subroutine.
int automatic              = 0;      // Deffined by gameType and configureGame subroutine. Automatic fire 0 = Semi Auto, 1 = Fully Auto.

//Incoming signal Details
int received[18];                    // Received data: received[0] = which sensor, received[1] - [17] byte1 byte2 parity (Miles Tag structure)
int check                  = 0;      // Variable used in parity checking

// Stats
int ammo                   = 0;      // Current ammunition
int life                   = 0;      // Current life

// Code Variables
int timeOut                = 0;      // Defined in frequencyCalculations (IRpulse + 50)
int FIRE                   = 0;      // 0 = don't fire, 1 = Primary Fire, 2 = Secondary Fire
int TR                     = 0;      // Trigger Reading
int LTR                    = 0;      // Last Trigger Reading

// Signal Properties
int IRpulse                = 600;    // Basic pulse duration of 600uS MilesTag standard 4*IRpulse for header bit, 2*IRpulse for 1, 1*IRpulse for 0.
int IRfrequency            = 38;     // Frequency in kHz Standard values are: 38kHz, 40kHz. Choose dependant on your receiver characteristics
int IRt                    = 0;      // LED on time to give correct transmission frequency, calculated in setup.
int IRpulses               = 0;      // Number of oscillations needed to make a full IRpulse, calculated in setup.
int header                 = 4;      // Header lenght in pulses. 4 = Miles tag standard
int maxSPS                 = 10;     // Maximum Shots Per Seconds. Not yet used.
int TBS                    = 0;      // Time between shots. Not yet used.

// Transmission data
int byte1[8];                        // String for storing byte1 of the data which gets transmitted when the player fires.
int byte2[8];                        // String for storing byte1 of the data which gets transmitted when the player fires.
int myParity               = 0;      // String for storing parity of the data which gets transmitted when the player fires.

// Received data
int memory                 = 10;     // Number of signals to be recorded: Allows for the game data to be reviewed after the game, no provision for transmitting / accessing it yet though.
int hitNo                  = 0;      // Hit number
// Byte1
int player[10];                      // Array must be as large as memory
int team[10];                        // Array must be as large as memory
// Byte2
int weapon[10];                      // Array must be as large as memory
int hp[10];                          // Array must be as large as memory
int parity[10];                      // Array must be as large as memory

void setup() {
  // Serial coms set up to help with debugging.
  Serial.begin(9600);
  // Pin declarations
  pinMode(triggerPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(lifePin, OUTPUT);
  pinMode(ammoPin, OUTPUT);
  pinMode(hitPin, OUTPUT);
  pinMode(IRtransmitPin, OUTPUT);
  pinMode(IRreceivePin, INPUT);

  frequencyCalculations();   // Calculates pulse lengths etc for desired frequency
  configureGame();           // Look up and configure game details

  digitalWrite(triggerPin, HIGH);      // Not really needed if your circuit has the correct pull up resistors already but doesn't harm

  for (int i = 1;i < 254;i++) { // Loop plays start up noise
    analogWrite(ammoPin, i);
    playTone((3000-9*i), 2);
  }

  // Next 4 lines initialise the display LEDs
  analogWrite(ammoPin, ((int) ammo));
  analogWrite(lifePin, ((int) life));
}

// Main loop most of the code is in the sub routines
void loop(){
  receiveIR();
  if(FIRE != 0){
    shoot();

  }
  triggers();
}

// SUB ROUTINES
void receiveIR() { // Void checks for an incoming signal and decodes it if it sees one.
  int error = 0;

  if(digitalRead(IRreceivePin) == LOW){    // If the receive pin is low a signal is being received.
    digitalWrite(hitPin,HIGH);
  hit();
  }
}

void shoot() {
  if(FIRE == 1){ // Has the trigger been pressed?
    sendPulse(IRtransmitPin, 4); // Transmit Header pulse, send pulse subroutine deals with the details
    delayMicroseconds(IRpulse);

    for(int i = 0; i < 8; i++) { // Transmit Byte1
      if(byte1[i] == 1){
        sendPulse(IRtransmitPin, 1);
        //Serial.print("1 ");
      }
      //else{Serial.print("0 ");}
      sendPulse(IRtransmitPin, 1);
      delayMicroseconds(IRpulse);
    }

    for(int i = 0; i < 8; i++) { // Transmit Byte2
      if(byte2[i] == 1){
        sendPulse(IRtransmitPin, 1);
       // Serial.print("1 ");
      }
      //else{Serial.print("0 ");}
      sendPulse(IRtransmitPin, 1);
      delayMicroseconds(IRpulse);
    }

    if(myParity == 1){ // Parity
      sendPulse(IRtransmitPin, 1);
    }
    sendPulse(IRtransmitPin, 1);
    delayMicroseconds(IRpulse);
  }

FIRE = 0;
ammo = ammo - 1;
}


void sendPulse(int pin, int length){ // importing variables like this allows for secondary fire modes etc.
// This void genertates the carrier frequency for the information to be transmitted
  int i = 0;
  int o = 0;
  while( i < length ){
    i++;
    while( o < IRpulses ){
      o++;
      digitalWrite(pin, HIGH);
      delayMicroseconds(IRt);
      digitalWrite(pin, LOW);
      delayMicroseconds(IRt);
    }
  }
}


void triggers() { // Checks to see if the triggers have been presses
  LTR = TR;       // Records previous state. Primary fire
  TR = digitalRead(triggerPin);      // Looks up current trigger button state
  // Code looks for changes in trigger state to give it a semi automatic shooting behaviour
  if(TR != LTR && TR == LOW){
    FIRE = 1;
  }
  if(TR == LOW && automatic == 1){
    FIRE = 1;
  }
  if(FIRE == 1){
    if(ammo < 1){FIRE = 0; noAmmo();}
    if(life < 1){FIRE = 0; dead();}
  }

}


void configureGame() {
  myWeaponID = 1;
  maxAmmo = 30;
  ammo = 30;
  maxLife = 3;
  life = 3;
  myWeaponHP = 1;
}


void frequencyCalculations() { // Works out all the timings needed to give the correct carrier frequency for the IR signal
  IRt = (int) (500/IRfrequency);
  IRpulses = (int) (IRpulse / (2*IRt));
  IRt = IRt - 4;
  // Why -4 I hear you cry. It allows for the time taken for commands to be executed.
  // More info: http://j44industries.blogspot.com/2009/09/arduino-frequency-generation.html#more

  timeOut = IRpulse + 50; // Adding 50 to the expected pulse time gives a little margin for error on the pulse read time out value
}

void playTone(int tone, int duration) { // A sub routine for playing tones like the standard arduino melody example
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }

}void dead() { // void determines what the tagger does when it is out of lives
  // Makes a few noises and flashes some lights
  for (int i = 1;i < 254;i++) {
    analogWrite(ammoPin, i);
    playTone((1000+9*i), 2);
  }
  analogWrite(ammoPin, ((int) ammo));
  analogWrite(lifePin, ((int) life));

  for (int i=0; i<10; i++) {
   analogWrite(ammoPin, 255);
   digitalWrite(hitPin,HIGH);
   delay (500);
   analogWrite(ammoPin, 0);
   digitalWrite(hitPin,LOW);
   delay (500);
  }
}

void noAmmo() { // Make some noise and flash some lights when out of ammo
  digitalWrite(hitPin,HIGH);
  playTone(500, 100);
  playTone(1000, 100);
  digitalWrite(hitPin,LOW);
}

void hit() { // Make some noise and flash some lights when you get shot
  digitalWrite(hitPin,HIGH);
  life = life - 1;
  playTone(500, 500);
  if(life <= 0){dead();}
  digitalWrite(hitPin,LOW);
}
