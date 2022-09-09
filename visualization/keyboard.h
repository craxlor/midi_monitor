/**
 * @file keyboard.h
 * @authors Arnaud Kalthoff
 * @brief The Keyboard class contains all the attributes & methods needed to implement the keyboard visualisation.
 *
 * @copyright Copyright (c) 2022
 */
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <mios32.h>
#include <notestack.h>
class Keyboard
{
public:
    static void drawKeyboard();
    static void drawNotestack(notestack_t notestack);
};

#endif