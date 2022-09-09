#include "keyboard.h"
#include "midihelper/midihelper.h"

u8 piano_bitmap[] = { // 126x32px
    0xfe, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe,
    0xfe, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0xfe, 0xfe, 0xfe, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xfe, 0xfe, 0xfe, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0xff, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xf8,
    0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
    0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0x00,
    0xf8, 0xf8, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xf8,
    0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8,
    0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xff,
    0xff, 0x00, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8,
    0xff, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00,
    0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f,
    0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f,
    0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00,
    0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f,
    0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f,
    0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00,
    0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f,
    0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00};

const mios32_lcd_bitmap_t bitmap = MIOS32_LCD_BitmapInit(piano_bitmap, 126, 32, 126, 1);

void Keyboard::drawKeyboard()
{

    const char *octave = "";
    // draw keyboard on first 4 displays
    for (size_t i = 0; i < 4; i++)
    {
        MIOS32_LCD_DeviceSet(i);
        MIOS32_LCD_CursorSet(0, 0);
        MIOS32_LCD_BitmapPrint(bitmap);
        // show 1st octave of display
        MIOS32_LCD_CursorSet(0, 5); // 0,5 because 0,6 contains selected channel information
        switch (i)
        {
        case 0:
            octave = "C-2";
            break;
        case 1:
            octave = "C1";
            break;
        case 2:
            octave = "C4";
            break;
        case 3:
            octave = "C7";
            break;
        }
        MIOS32_LCD_PrintString(octave);
    }
}

void Keyboard::drawNotestack(notestack_t notestack)
{
    u8 byteToDraw;
    for (size_t i = 0; i < notestack.len; i++)
    {
        int note, height, pixelOffset, pixelColumnIndex;
        // check if the note is pressed
        if (notestack.note_items[i].depressed)
            continue;

        /**
         * select correct display
         * 12 notes per octave, 3 octaves per display --> 12*3 notes per display --> 36 notes
         */
        note = notestack.note_items[i].note % 36;
        MIOS32_LCD_DeviceSet(notestack.note_items[i].note / 36);

        // start of drawNote() sourcecode
        int octave;
        // determine if note is a black or white key
        if (MidiHelper::isFlat(note)) // black key
        {
            height = 15;
            byteToDraw = 0x7F;
        }
        else // white key
        {
            height = 26;
            byteToDraw = 0x00;
        }

        int standardKeyOffset = 3;
        // includes the keygap between octaves
        int octaveOffset = standardKeyOffset * 14;
        // starting from 1
        int currentKey = (note % 12 + 1);
        int keyGapOffset = currentKey > 5 ? standardKeyOffset : 0;
        // locates the penultimate pixel (width) of a key
        pixelColumnIndex = (currentKey * standardKeyOffset) + keyGapOffset + note / 12 * octaveOffset;

        MIOS32_LCD_GCursorSet(pixelColumnIndex - 2, height);
        MIOS32_LCD_Data(byteToDraw);
        MIOS32_LCD_GCursorSet(pixelColumnIndex - 1, height);
        MIOS32_LCD_Data(byteToDraw);
        MIOS32_LCD_GCursorSet(pixelColumnIndex, height);
        MIOS32_LCD_Data(byteToDraw);
    }
}