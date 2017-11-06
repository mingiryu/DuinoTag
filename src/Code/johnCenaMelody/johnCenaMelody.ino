/*
 johnCenaMelody
 Plays a melody of John Cena Entrance Song
 circuit:
 * 8-ohm speaker on digital pin 4
 */
#include <IRremote.h> // IRremote library is included for compatibility check
#include <NewTone.h> // Required for compatibility with IRremote
#include "pitches.h"

int speakerPin             = 4;

void setup() {

  pinMode(speakerPin, OUTPUT);

  johnCena();

}


// Main loop most of the code is in the sub routines
void loop(){

}

void johnCena() { // plays john cena song

  // notes in the melody:
  int melody[] = {
    NOTE_A4, NOTE_B4, NOTE_G4, 0, NOTE_A4, 0,NOTE_C5, NOTE_B4, NOTE_G4, 0, NOTE_A4
  };

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    4, 6, 6, 8, 1, 4, 4, 6, 6, 8, 1
  };
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 11; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    NewTone(4, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noNewTone(4);
  }
}
