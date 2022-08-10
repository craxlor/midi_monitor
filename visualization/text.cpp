/**
 * @file text.cpp
 * @authors Arnaud Kalthoff
 * @brief implements all methods of text.h
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "text.h"
#include "midihelper/midihelper.h"
#include <string>

void Text::draw(mios32_midi_package_t package)
{
    const char *prefix1 = "value1";
    const char *prefix2 = "value2";
    /**
     * 1st display
     * pretty print
     *
     * 4th display
     * raw sysex
     */

    // display 1
    MIOS32_LCD_DeviceSet(0);
    switch (package.type)
    {
    case NoteOn:
    case NoteOff:
        prefix1 = "note";
        prefix2 = "velocity";
        break;
    case CC:
        prefix1 = "cc";
        prefix2 = "value";
        break;
    case PitchBend:
    case Aftertouch:
    case ProgramChange:
        prefix1 = "value";
        prefix2 = nullptr;
        break;
    case PolyPressure:
        prefix1 = "note";
        prefix2 = "value";
        break;
    }
    MIOS32_LCD_CursorSet(0, 0); // X, Y
    MIOS32_LCD_PrintString("Midi-Message-Text");
    MIOS32_LCD_CursorSet(0, 2); // X, Y
    MIOS32_LCD_PrintFormattedString("channel: %d", MidiHelper::getChannel(package.chn));
    MIOS32_LCD_CursorSet(0, 3); // X, Y
    MIOS32_LCD_PrintFormattedString("type: %s", MidiHelper::getType(package.type));
    MIOS32_LCD_CursorSet(0, 4); // X, Y

    if (prefix1 == std::string("note"))
        MIOS32_LCD_PrintFormattedString("%s: %s", prefix1, MidiHelper::getNote(package.note));
    else if (package.type == PitchBend)
    {
        MIOS32_LCD_PrintFormattedString("%s: %d", prefix1, MidiHelper::getPitchBend(package.value1, package.value2));
    }
    else if (package.type == ProgramChange)
    {
        MIOS32_LCD_PrintFormattedString("%s: %d", prefix1, MidiHelper::getProgramChange(package.value1));
    }
    else
        MIOS32_LCD_PrintFormattedString("%s: %d", prefix1, package.value1);

    if (prefix2 != nullptr)
    {
        MIOS32_LCD_CursorSet(0, 5); // X, Y
        MIOS32_LCD_PrintFormattedString("%s: %d", prefix2, package.value2);
    }

    // display 4
    MIOS32_LCD_DeviceSet(3);
    MIOS32_LCD_CursorSet(0, 0); // X, Y
    MIOS32_LCD_PrintFormattedString("Midi-Message-RAW");
    MIOS32_LCD_CursorSet(0, 2); // X, Y
    MIOS32_LCD_PrintFormattedString("type: %02x", package.type);
    MIOS32_LCD_CursorSet(0, 3); // X, Y
    MIOS32_LCD_PrintFormattedString("event0: %02x", package.evnt0);
    MIOS32_LCD_CursorSet(0, 4); // X, Y
    MIOS32_LCD_PrintFormattedString("event1: %02x", package.evnt1);
    MIOS32_LCD_CursorSet(0, 5); // X, Y
    MIOS32_LCD_PrintFormattedString("event2: %02x", package.evnt2);
}