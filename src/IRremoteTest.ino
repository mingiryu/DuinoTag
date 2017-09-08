/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 11;

long testRemote = 1641445511;
long testGun = 3389625022;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == testRemote) {
      Serial.println("Remote");
    }
    else if (results.value == testGun) {
      Serial.println("Gun");
    }
    else {
      Serial.println(results.value);
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
