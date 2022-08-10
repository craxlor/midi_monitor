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