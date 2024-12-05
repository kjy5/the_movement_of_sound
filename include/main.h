//
// Created by Kenneth Yang on 12/4/24.
//

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>


/**
 * Set the servo to a specific angle then detach.
 * 
 * @param angle Angle to set the servo to (0 - 180).
 */
void setServos(uint8_t angle);

/**
 * Detach the servos.
 */
void detachServos();

/**
 * Interpret player state values and print to serial.
 * 
 * @param type Player state type.
 * @param value Player state value.
 */
void printDetail(uint8_t type, uint16_t value);
#endif //MAIN_H
