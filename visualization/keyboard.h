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
private:
    static u8 piano_bitmap[];
    static mios32_lcd_bitmap_t bitmap;
    static void drawNote(u8 note, u8 type);

public:
    static void drawKeyboard();
    static void drawNotestack(notestack_t notestack);
};

#endif