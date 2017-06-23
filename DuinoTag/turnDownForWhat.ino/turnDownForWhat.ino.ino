/*
  Melody
 Plays a melody
 circuit:
 * 8-ohm speaker on digital pin 8
 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe
This example code is in the public domain.
 http://www.arduino.cc/en/Tutorial/Tone
 */

 /*
https://www.youtube.com/watch?v=BGY2InN4R98
http://www.musicnotes.com/sheetmusic/mtd.asp?ppn=mn0145399
 */
#include "pitches.h"

int speakerPin             = 4;

void setup() {

  pinMode(speakerPin, OUTPUT);

  turnDownForWhat();

}


// Main loop most of the code is in the sub routines
void loop(){

}

void turnDownForWhat() { // plays john cena song

  // notes in the melody:
  int melody[] = {
    NOTE_E3, NOTE_E2, NOTE_E2, NOTE_F3, NOTE_E2, NOTE_E2, NOTE_FS3, NOTE_E2, NOTE_E2, NOTE_G3, NOTE_E2, NOTE_E2,
    NOTE_GS3, NOTE_E2, NOTE_E2, NOTE_A3, NOTE_E2, NOTE_E2, NOTE_AS3, NOTE_E2, NOTE_E2, NOTE_B3, NOTE_E2, NOTE_E2,

    NOTE_C4, NOTE_E2, NOTE_E2, NOTE_CS4, NOTE_E2, NOTE_E2, NOTE_D4, NOTE_E2, NOTE_E2, NOTE_DS4, NOTE_E2, NOTE_E2,
    NOTE_E4, NOTE_E2, NOTE_E2, NOTE_F4, NOTE_E2, NOTE_E2, NOTE_FS4, NOTE_E2, NOTE_E2, NOTE_G4, NOTE_E2, NOTE_E2,

    NOTE_GS4, NOTE_E2, NOTE_E2, NOTE_A4, NOTE_E2, NOTE_E2, NOTE_AS4, NOTE_E2, NOTE_E2, NOTE_B4, NOTE_E2, NOTE_E2,
    NOTE_C5, NOTE_E2, NOTE_E2, NOTE_CS5, NOTE_E2, NOTE_E2, NOTE_D5, NOTE_E2, NOTE_E2, NOTE_DS5, NOTE_E2, NOTE_E2,

    NOTE_E5, NOTE_E2, NOTE_E2, NOTE_F5, NOTE_E2, NOTE_E2, NOTE_FS5, NOTE_E2, NOTE_E2, NOTE_G5, NOTE_E2, NOTE_E2,
    0      , 0      , NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,

    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,
    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,
    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,
    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,

    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,
    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,
    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,
    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,

    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,
    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,
    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,
    NOTE_E5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_E3, 0,

    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,
    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,
    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,
    NOTE_E4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_E2, 0,

  };
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, //12
    4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, //24

    4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, //36
    4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, //48

    4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, //60
    4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, //72

    4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, //84
    1, 4, 4, 4, 8, 8, //90

    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,

    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,

    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,

    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,
    4, 8, 4, 4, 8, 4, 4, 8,


    };
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 218; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(4, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(4);
  }
}
