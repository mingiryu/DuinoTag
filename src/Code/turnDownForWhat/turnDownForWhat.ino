/*
 turnDownForwhat
 Plays a melody of Turn Down For What
 circuit:
 * 8-ohm speaker on digital pin 4
 */
#include <IRremote.h> // IRremote library is included for compatibility check
#include <NewTone.h> // Required for compatibility with IRremote
#include "pitches.h"

int speakerPin             = 4;

void setup() {

  pinMode(speakerPin, OUTPUT);

  turnDownForWhat();

}


// Main loop most of the code is in the sub routines
void loop(){

}

void turnDownForWhat() {

  // notes in the melody:
  int melody[] = {
    0      , 0      , NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,

    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,
    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,

    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,
    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,
  };
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    1, 4, 4, 4, 8, 8,

    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,

    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    };
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 38; thisNote++) {

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
