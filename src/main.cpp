#include <Arduino.h>
#include <Wtv020sd16p.h>
#include <MobaTools.h>

int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;  // The pin number of the data pin.
int busyPin = 5;  // The pin number of the busy pin.

/*
Create an instance of the Wtv020sd16p class.
1st parameter: Reset pin number.
2nd parameter: Clock pin number.
3rd parameter: Data pin number.
4th parameter: Busy pin number.
*/
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

#define ANZAHL  8   // Anzahl der Blinker

const byte blinkerP[ANZAHL] =  {6,7,8,9,10,11,12,13};           // Pins für die Led's
const int  blinkOn[ANZAHL]  =  {211,222,233,241,214,226,227,217};   // Einschaltzeit der Blinklampen (ms)
const int  blinkOff[ANZAHL] =  {214,229,227,220,218,222,219,213};   // Pausezeit ( in ms)

EggTimer Blinkzeit[ANZAHL]; // Die Zeitgeber lassen sich auch wie ein Array definieren
EggTimer Anfang;
EggTimer Ablauf;


byte i;     // Zählvariable



void setup() {
    //Initializes the module.
    wtv020sd16p.reset();
    for ( i=0; i<ANZAHL; i++ ) {
        pinMode(blinkerP[i], OUTPUT); 
        digitalWrite(blinkerP[i], HIGH); 
    }
    Anfang.setTime(2000);
    Ablauf.setTime(40000);
    delay(10000);
    for ( i=0; i<ANZAHL; i++ ) {
        Blinkzeit[i].setTime( blinkOff[i] );
    }
}

void loop() {
    // -------- Verwalten der 4 Blinker in einer Schleife ------------------
    for ( i=0; i<ANZAHL; i++ ){
        if ( Blinkzeit[i].running()== false ) {
            // Blinkzeit abgelaufen, Ausgang toggeln und
            // Zeit neu aufziehen
            if ( digitalRead( blinkerP[i] ) == HIGH ) {
                digitalWrite( blinkerP[i], LOW );
                Blinkzeit[i].setTime( blinkOff[i] );
            } else {
                digitalWrite( blinkerP[i], HIGH );
                Blinkzeit[i].setTime( blinkOn[i] );
            }
        }
    } // Ende for-Schleife
    if ( Anfang.running() == false) {
        //Plays synchronously an audio file. Busy pin is used for this method.
        wtv020sd16p.asyncPlayVoice(0);
        Anfang.setTime(300000);
    }
    if ( Ablauf.running() == false) {
        for ( i=0; i<ANZAHL; i++ ) {
            digitalWrite(blinkerP[i], HIGH); 
        }
        delay(1000000);
    }
 }

