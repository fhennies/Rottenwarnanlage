#include <Arduino.h>
#include <Wtv020sd16p.h>
#include <MobaTools.h>

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

#define ANZAHL  8   // Anzahl der Blinker

const byte blinkerP[ANZAHL] =  {6,7,8,9,10,11,12,13};               // Pins für die Led's
const int  blinkOn[ANZAHL]  =  {100,106,102,103,108,101,105,107};   // Ein der Blinklampen (ms) 2Hz Tastverhätnis 1:4 
const int  blinkOff[ANZAHL] =  {400,424,408,412,432,404,420,428};   // Pausezeit (ms) 
const int  blinkUp[ANZAHL]  =  {0,260,40,200,80,120,180,290};   // Einschaltpause (ms) 


EggTimer Blinkzeit[ANZAHL];     // Die Zeitgeber lassen sich auch wie ein Array definieren
EggTimer Anfang;
EggTimer Ablauf;

int Ro = 1;     // Ro1 oder Ro3, = aus 

byte i;         // Zählvariable



void setup() {
    //Initializes the module.
    for ( i=0; i<ANZAHL; i++ ) {
        pinMode(blinkerP[i], OUTPUT); 
        digitalWrite(blinkerP[i], HIGH);
        Blinkzeit[i].setTime(6000000);
    }
    wtv020sd16p.reset();
    Anfang.setTime(2000);
    Ablauf.setTime(20000);
}

void loop() {
    // -------- Verwalten aller Blinker in einer Schleife ------------------

    if ( Anfang.running() == false ) {
        wtv020sd16p.asyncPlayVoice(Ro);
        Anfang.setTime(300000);
        Ablauf.setTime(20000);
        for ( i=0; i<ANZAHL; i++ ) {
            Blinkzeit[i].setTime(random(400));
        }
    }
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
    if ( Ablauf.running() == false) {
        for ( i=0; i<ANZAHL; i++ ) {
            digitalWrite(blinkerP[i], HIGH); 
            Blinkzeit[i].setTime(6000000);
        }
        if ( Ro == 1) {
            Ro = 3;
            Anfang.setTime(2000);
            Ablauf.setTime(200000);
        } else {
            while(1);
        }
    }
}

