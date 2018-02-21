#include <Arduino.h>
#include <Wtv020sd16p.h>
#include <MobaTools.h>
#include <Bounce2.h>


/*
Create an instance of the Wtv020sd16p class.
1st parameter: Reset pin number.
2nd parameter: Clock pin number.
3rd parameter: Data pin number.
4th parameter: Busy pin number.
*/

int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;   // The pin number of the data pin.
int busyPin = 5;   // The pin number of the busy pin.

Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

#define ANZAHL  8       // Anzahl der Blinker
#define NUMBUTTONS 3    // Anzahl der Eingänge


const byte blinkerP[ANZAHL]   =  {6,7,8,9,10,11,12,13};               // Pins für die Led's
const int  blinkOn[ANZAHL]    =  {100,106,102,103,108,101,105,107};   // Ein der Blinklampen (ms) 2Hz Tastverhätnis 1:4 
const int  blinkOff[ANZAHL]   =  {400,424,408,412,432,404,420,428};   // Pausezeit (ms) 

Bounce debouncer[NUMBUTTONS];                   // Mehrere Bounce-Instanzen erstellen
const byte inputPin[NUMBUTTONS] =  {14,15,16};  // Input pins A0=14; A1=15 etc
bool buttonState[NUMBUTTONS];                   // Speichert den aktuellen HIGH/LOW Status der Pins
byte buttonChange[NUMBUTTONS];                  // Speichert die Zustandswechsel an den Pins
bool roStarted[3] = {0,0,0};

EggTimer Blinkzeit[ANZAHL];     // Die Zeitgeber lassen sich auch wie ein Array definieren
EggTimer Ablauf;

int Ro = 0;                     // Ro1 oder Ro3

byte i;                         // Zählvariable



void setup() {
    for(int i = 0; i < NUMBUTTONS; i++) {
        pinMode(inputPin[i], INPUT_PULLUP);
        debouncer[i].attach(inputPin[i]);
        debouncer[i].interval(10);
    }
    for ( i=0; i<ANZAHL; i++ ) {
        pinMode(blinkerP[i], OUTPUT); 
        digitalWrite(blinkerP[i], HIGH);
    }
    wtv020sd16p.reset();
}

void loop() {
    for(int i = 0; i < NUMBUTTONS; i++) {
        debouncer[i].update();
    }
    if ( ( debouncer[0].fell() == true && Ablauf.running() == false ) || debouncer[1].fell() == true ) {
        wtv020sd16p.stopVoice();
        Ablauf.setTime(6000);
        for ( i=0; i<ANZAHL; i++ ) {
            Blinkzeit[i].setTime(random(400));
        }
        if (debouncer[1].fell() == true && debouncer[2].read()) {Ro = 3;}
        else if (debouncer[2].read()) {Ro = 1;}
        else {Ro = 0;}
        wtv020sd16p.asyncPlayVoice(Ro);
    }
    if ( ( !debouncer[0].read() || !debouncer[1].read() ) || Ablauf.running() == true) {
        for ( i=0; i<ANZAHL; i++ ){
            if ( Blinkzeit[i].running()== false ) {
                // Blinkzeit abgelaufen, Ausgang toggeln und
                // Zeit neu aufziehen
                if ( digitalRead( blinkerP[i] ) == HIGH ) {
                    digitalWrite( blinkerP[i], LOW );
                    Blinkzeit[i].setTime( blinkOn[i] );
                } else {
                    digitalWrite( blinkerP[i], HIGH );
                    Blinkzeit[i].setTime( blinkOff[i] );
                }
            }
        }
    } else {
        for ( i=0; i<ANZAHL; i++ ) {
            digitalWrite(blinkerP[i], HIGH); 
        }
    }
}

