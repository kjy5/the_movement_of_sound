/**
 * The Movement of Sound
 *
 * DXARTS 470 final project
 * @author Kenneth J. Yang
 * @copyright Nov. 2024
 **/

// Libraries.
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
#include <Servo.h>

// My libraries.
#include "main.h"
#include "pins.h"
#include "constants.h"

// Use SoftwareSerial to communicate with the DFPlayerMini.
SoftwareSerial softSerial(SPEAKER_RX, SPEAKER_TX);
#define FPSerial softSerial

// Declare DFPlayerMini.
DFRobotDFPlayerMini player;

// Declare servos.
Servo left_servo;
Servo right_servo;

// Last volume update time.
unsigned long last_volume_update = 0;

// Last rotation time.
unsigned long last_rotation = 0;

// Rotate state.
bool rotate_state = false;
int rotation_direction = 1;
bool need_detach = false;
unsigned long detach_time = 0;

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

    // Setup pins.
    pinMode(NEXT_BUTTON, INPUT_PULLUP);
    pinMode(PREV_BUTTON, INPUT_PULLUP);
    pinMode(ROTATE_BUTTON, INPUT_PULLUP);
    pinMode(VOLUME_POT, INPUT);

    // Reset servos.
    setServos(90);

    // Set default volume and play the first track.
    player.volume(50);
    player.play();
    player.next();
}

void loop() {
    // Get states.
    const uint8_t volume = map(analogRead(VOLUME_POT), 0, 1023, 0, 100);
    const bool next_pressed = digitalRead(NEXT_BUTTON) == HIGH;
    const bool prev_pressed = digitalRead(PREV_BUTTON) == HIGH;
    const bool rotate_pressed = digitalRead(ROTATE_BUTTON) == HIGH;

    // Advance track on next button press.
    if (next_pressed) {
        player.next();
        delay(250);
    }

    // Go back a track on prev button press.
    if (prev_pressed) {
        player.previous();
        delay(250);
    }

    // Toggle rotate state on rotate button press.
    if (rotate_pressed) {
        rotate_state = !rotate_state;
        delay(250);
    }

    // Update volume if time has passed.
    if (millis() - last_volume_update > VOLUME_UPDATE_INTERVAL) {
        player.volume(volume);
        last_volume_update = millis();
    }

    // Rotate servos if time has passed.
    if (rotate_state && millis() - last_rotation > ROTATION_INTERVAL) {
        setServos(90 + rotation_direction * ROTATION_AMOUNT);
        rotation_direction *= -1;
        last_rotation = millis();
    }

    // Detach servo if time has passed.
    if (need_detach && millis() > detach_time) {
        // Reset to 90 degrees.
        left_servo.write(90);
        right_servo.write(90);

        // Detach.
        detachServos();
    }

    // Print player state.
    // if (player.available()) {
    //     printDetail(player.readType(), player.read());
    // }
}

void setServos(const uint8_t angle) {
    // Attach.
    left_servo.attach(LEFT_SERVO);
    right_servo.attach(RIGHT_SERVO);

    // Write.
    left_servo.write(angle);
    right_servo.write(angle);

    // Set detach time.
    detach_time = millis() + 250;
    need_detach = true;
}

void detachServos() {
    left_servo.detach();
    right_servo.detach();
    need_detach = false;
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
