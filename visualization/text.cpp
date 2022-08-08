#include "text.h"
#include "midihelper/midihelper.h"
#include <string>

void Text::draw(mios32_midi_package_t package)
{
    // display 1: pretty print
    MIOS32_LCD_DeviceSet(0);
    MIOS32_LCD_CursorSet(0, 0); // X, Y
    MIOS32_LCD_PrintString("Midi-Message-Text");
    MIOS32_LCD_CursorSet(0, 2); // X, Y
    MIOS32_LCD_PrintFormattedString("channel: %d", MidiHelper::getChannel(package.chn));
    MIOS32_LCD_CursorSet(0, 3); // X, Y
    MIOS32_LCD_PrintFormattedString("type: %s", MidiHelper::getType(package.type));
    MIOS32_LCD_CursorSet(0, 4); // X, Y
    // values
    switch (package.type)
    {
    case NoteOn:
    case NoteOff:
        MIOS32_LCD_PrintFormattedString("note: %s", MidiHelper::getNote(package.note));
        MIOS32_LCD_CursorSet(0, 5); // X, Y
        MIOS32_LCD_PrintFormattedString("velocity: %d", package.value2);
        break;
    case CC:
        MIOS32_LCD_PrintFormattedString("cc: %s", MidiHelper::getCCs(package.value1));
        MIOS32_LCD_CursorSet(0, 5); // X, Y
        MIOS32_LCD_PrintFormattedString("value: %d", package.value2);
        break;
    case PitchBend:
        MIOS32_LCD_PrintFormattedString("value: %d", MidiHelper::getPitchBend(package.value1, package.value2));
        break;
    case Aftertouch:
        MIOS32_LCD_PrintFormattedString("value: %d", package.value1);
        break;
    case ProgramChange:
        MIOS32_LCD_PrintFormattedString("value: %d", MidiHelper::getProgramChange(package.value1));
        break;
    case PolyPressure:
        MIOS32_LCD_PrintFormattedString("note: %s", MidiHelper::getNote(package.note));
        MIOS32_LCD_CursorSet(0, 5); // X, Y
        MIOS32_LCD_PrintFormattedString("value: %d", package.value2);
        break;
    }

    // display 4: hex format
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