/**
 * The Movement of Sound
 *
 * DXARTS 470 final project
 * @author Kenneth J. Yang
 * @copyright Nov. 2024
 **/

// Libraries.
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

// My libraries.
#include "pins.h"

// Use SoftwareSerial to communicate with the DFPlayerMini.
SoftwareSerial softSerial(SPEAKER_RX, SPEAKER_TX);
#define FPSerial softSerial

// Declare DFPlayerMini.
DFRobotDFPlayerMini player;

void printDetail(uint8_t type, uint16_t value);

void setup() {
    // Initialize serial.
    FPSerial.begin(9600);
    Serial.begin(115200);

    // Initialize DFPlayerMini.
    Serial.println("Initializing DFPlayerMini...");
    if (!player.begin(FPSerial)) {
        Serial.println("Unable to begin.");
        return;
    }
    Serial.println("DFPlayerMini initialized.");

    // Set default volume and play the first track.
    player.volume(30);
    player.play();
    player.next();
}

void loop() {
    // Print player state.
    if (player.available()) {
        printDetail(player.readType(), player.read());
    }
}

void printDetail(uint8_t type, uint16_t value) {
    switch (type) {
        case TimeOut:
            Serial.println("Time Out!");
            break;
        case WrongStack:
            Serial.println("Stack Wrong!");
            break;
        case DFPlayerCardInserted:
            Serial.println("Card Inserted!");
            break;
        case DFPlayerCardRemoved:
            Serial.println("Card Removed!");
            break;
        case DFPlayerCardOnline:
            Serial.println("Card Online!");
            break;
        case DFPlayerUSBInserted:
            Serial.println("USB Inserted!");
            break;
        case DFPlayerUSBRemoved:
            Serial.println("USB Removed!");
            break;
        case DFPlayerError:
            Serial.print("DFPlayerError: ");
            switch (value) {
                case Busy:
                    Serial.println("Card not found!");
                    break;
                case Sleeping:
                    Serial.println("Sleeping!");
                    break;
                case SerialWrongStack:
                    Serial.println("Got Wrong Stack");
                    break;
                case CheckSumNotMatch:
                    Serial.println("Check Sum Not Match");
                    break;
                case FileIndexOut:
                    Serial.println("File Index Out of Bound");
                    break;
                case FileMismatch:
                    Serial.println("Cannot Find File");
                    break;
                case Advertise:
                    Serial.println("In Advertise");
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
