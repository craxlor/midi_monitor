#include "text.h"
#include "midihelper/midihelper.h"
#include <string>

void Text::draw(mios32_midi_package_t package)
{
    const char *prefix1 = "value1";
    const char *prefix2 = "value2";
    u16 pitchbendValue = 0;
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
        prefix1 = "Note";
        prefix2 = "Velocity";
        break;
    case CC:
        prefix1 = "CC";
        prefix2 = "value";
        break;
    case PitchBend:
        prefix1 = "value";
        prefix2 = nullptr;
        pitchbendValue = ((package.value2 << 7) | (package.value1 & 0x7f)) - 8192; // calculate pitchbend value
        break;
    case Aftertouch:
        prefix1 = "value";
        prefix2 = nullptr;
        break;
    case PolyPressure:
        prefix1 = "Note";
        prefix2 = "value";
        break;
    case ProgramChange:
        break;
    }
    MIOS32_LCD_CursorSet(0, 0); // X, Y
    MIOS32_MIDI_SendDebugString("Midi-Message-Text");
    MIOS32_LCD_CursorSet(0, 2); // X, Y
    MIOS32_MIDI_SendDebugMessage("Channel: %d", MidiHelper::getChannel(package.chn));
    MIOS32_LCD_CursorSet(0, 3); // X, Y
    MIOS32_MIDI_SendDebugMessage("Type: %s", MidiHelper::getType(package.type));
    MIOS32_LCD_CursorSet(0, 4); // X, Y

    if (prefix1 == std::string("Note"))
        MIOS32_MIDI_SendDebugMessage("%s: %s", prefix1, MidiHelper::getNote(package.note));
    else if (package.type == PitchBend)
        MIOS32_MIDI_SendDebugMessage("%s: %d", prefix1, pitchbendValue);
    else
        MIOS32_MIDI_SendDebugMessage("%s: %d", prefix1, package.value1);

    if (prefix2 != nullptr)
    {
        MIOS32_LCD_CursorSet(0, 5); // X, Y
        MIOS32_MIDI_SendDebugMessage("%s: %d", prefix2, package.value2);
    }

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