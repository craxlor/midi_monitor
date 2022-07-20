#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <mios32.h>

class Keyboard
{
private:
    static u8 piano_bitmap[];

public:
    static void drawKeyboard();
    static void drawKeyPress(mios32_midi_package_t midi_package);
};

#endif