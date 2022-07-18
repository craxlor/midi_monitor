/**
 * @file visualization.cpp
 * @author Arnaud Kalthoff
 * @brief
 * @version 0.1
 * @date 2022-07-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "application/visualization.h"
#include "application/application.h"
#include "midipackage.h"

u8 Visualization::piano_bitmap[] = { // 126x32px
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

void Visualization::draw()
{
    // last received midi package
    mios32_midi_package_t p = Application::getInstance().getLastReceivedPackage();
    MidiPackage package(p);

    switch (visualizationmode)
    {
    case 0:
        /*TODO implement raw visualization*/
        break;
    case 1:
        /**
         * 1st display
         * midi channel, note on/off, tone, velocity
         * 2nd display
         * cc information
         * 3rd display
         * raw sysex
         */
        MIOS32_LCD_DeviceSet(0);
        MIOS32_LCD_Clear();
        MIOS32_LCD_CursorSet(0, 0); // X, Y
        MIOS32_LCD_PrintFormattedString("Midi-Message-Text");
        MIOS32_LCD_CursorSet(0, 2); // X, Y
        MIOS32_LCD_PrintFormattedString("Channel: %d", package.getChannel());
        MIOS32_LCD_CursorSet(0, 3); // X, Y
        MIOS32_LCD_PrintFormattedString("Event: %s", package.getType());
        MIOS32_LCD_CursorSet(0, 4); // X, Y
        MIOS32_LCD_PrintFormattedString("Note: %02s", package.getNote());
        MIOS32_LCD_CursorSet(0, 5); // X, Y
        MIOS32_LCD_PrintFormattedString("Velocity: %d", package.getVelocity());

        MIOS32_LCD_DeviceSet(1);
        MIOS32_LCD_CursorSet(0, 0); // X, Y
        MIOS32_LCD_PrintFormattedString("Midi-CCs");
        MIOS32_LCD_CursorSet(0, 1); // X, Y
        MIOS32_LCD_PrintFormattedString("CC#: %s",package.getCCs());
        

        MIOS32_LCD_DeviceSet(2);
        MIOS32_LCD_CursorSet(0, 0); // X, Y
        MIOS32_LCD_PrintFormattedString("Midi-Message-RAW");
        MIOS32_LCD_CursorSet(0, 2); // X, Y
        MIOS32_LCD_PrintFormattedString("Type: %02x", p.type);
        MIOS32_LCD_CursorSet(0, 3); // X, Y
        MIOS32_LCD_PrintFormattedString("Event0: %02x", p.evnt0);
        MIOS32_LCD_CursorSet(0, 4); // X, Y
        MIOS32_LCD_PrintFormattedString("Event1: %02x", p.evnt1);
        MIOS32_LCD_CursorSet(0, 5); // X, Y
        MIOS32_LCD_PrintFormattedString("Event2: %02x", p.evnt2);
        break;
    case 2:
        drawKeyboard(p);
        break;
    case 3:
        /*TODO implement accord visualization*/
        break;
    }
}

void Visualization::initKeyboard()
{
    mios32_lcd_bitmap_t bitmap = MIOS32_LCD_BitmapInit(piano_bitmap, 126, 32, 126, 1);
    MIOS32_LCD_DeviceSet(1);
    MIOS32_LCD_CursorSet(0, 0);
    MIOS32_LCD_BitmapPrint(bitmap);
}

void Visualization::drawKeyboard(mios32_midi_package_t midi_package)
{
    MIOS32_LCD_DeviceSet(1);
    MIOS32_MIDI_SendDebugString("updating keyboard");
    int value = midi_package.note;
    int octave;

    u8 byteToDraw;
    int height;
    if (isFlat(value)) // black key
    {
        height = 15;
        if (midi_package.type == NoteOn)
        {
            byteToDraw = 0x7F;
        }
        else
        {
            byteToDraw = 0x00;
        }
    }
    else // white key
    {
        height = 26;
        if (midi_package.type == NoteOn)
        {
            byteToDraw = 0x00;
        }
        else
        {
            byteToDraw = 0x7F;
        }
    }

    if (value > 23 && value <= 35)
    {
        octave = 84;
    }
    else if (value > 11)
    {
        octave = 42;
    }
    else
    {
        octave = 0;
    }

    int pixelColumnIndex = ((value % 12) + 1) * 3 + octave;
    MIOS32_MIDI_SendDebugString("zeichne die bytes...");
    MIOS32_MIDI_SendDebugMessage("column index: %d, zeilenindex: %d, byte: %x ", pixelColumnIndex, height, byteToDraw);
    MIOS32_LCD_GCursorSet(pixelColumnIndex - 2, height);
    MIOS32_LCD_Data(byteToDraw);
    MIOS32_LCD_GCursorSet(pixelColumnIndex - 1, height);
    MIOS32_LCD_Data(byteToDraw);
    MIOS32_LCD_GCursorSet(pixelColumnIndex, height);
    MIOS32_LCD_Data(byteToDraw);
}

bool Visualization::isFlat(int note_value)
{
    int value = note_value % 12;
    switch (value)
    {
    case 1:
    case 3:
    case 6:
    case 8:
    case 10:
        return true;
    default:
        return false;
    }
}

void Visualization::changeVisualizationMode()
{
    if (visualizationmode > 2)
    {
        visualizationmode = 0;
    }
    else
    {
        visualizationmode++;
    }
    MIOS32_MIDI_SendDebugMessage("Visualizationmode:%s", getVisualizationModeAsString());
}

int Visualization::getVisualizationMode()
{
    return visualizationmode;
}

const char *Visualization::getVisualizationModeAsString()
{
    switch (visualizationmode)
    {
    case 0:
        return "RAW";
    case 1:
        return "TEXT";
    case 2:
        return "PIANO";
    case 3:
        return "CHORD";
    default:
        return "Visualization-Mode is not defined";
    }
}
