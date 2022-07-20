#include "text.h"
#include "midihelper/midihelper.h"

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
        prefix1 = "Note";
        prefix2 = "Velocity";
        break;
    case CC:
        prefix1 = "CC#";
        prefix2 = "CC value#";
        break;
    case PitchBend:
        prefix1 = "pitchbend";
        prefix2 = "value";
        break;
    }
    MIOS32_LCD_CursorSet(0, 0); // X, Y
    MIOS32_LCD_PrintFormattedString("Midi-Message-Text");
    MIOS32_LCD_CursorSet(0, 2); // X, Y
    MIOS32_LCD_PrintFormattedString("Channel: %d", MidiHelper::getChannel(package.chn));
    MIOS32_LCD_CursorSet(0, 3); // X, Y
    MIOS32_LCD_PrintFormattedString("Type: %s", MidiHelper::getType(package.type));
    MIOS32_LCD_CursorSet(0, 4); // X, Y
    if (package.type == NoteOn)
        MIOS32_LCD_PrintFormattedString("%s: %s", prefix1, MidiHelper::getNote(package.note));
    else
        MIOS32_LCD_PrintFormattedString("%s: %d", prefix1, package.value1);
    MIOS32_LCD_CursorSet(0, 5); // X, Y
    MIOS32_LCD_PrintFormattedString("%s: %d", prefix2, package.value2);

    // display 4
    MIOS32_LCD_DeviceSet(3);
    MIOS32_LCD_CursorSet(0, 0); // X, Y
    MIOS32_LCD_PrintFormattedString("Midi-Message-RAW");
    MIOS32_LCD_CursorSet(0, 2); // X, Y
    MIOS32_LCD_PrintFormattedString("Type: %02x", package.type);
    MIOS32_LCD_CursorSet(0, 3); // X, Y
    MIOS32_LCD_PrintFormattedString("Event0: %02x", package.evnt0);
    MIOS32_LCD_CursorSet(0, 4); // X, Y
    MIOS32_LCD_PrintFormattedString("Event1: %02x", package.evnt1);
    MIOS32_LCD_CursorSet(0, 5); // X, Y
    MIOS32_LCD_PrintFormattedString("Event2: %02x", package.evnt2);
}