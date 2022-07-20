#include <mios32.h>
#include <string>
#include "midihelper.h"

int MidiHelper::getChannel(u8 chn)
{
    return chn + 1;
}

const char *MidiHelper::getType(u8 type)
{
    switch (type)
    {
    case NoteOff:
        return "NoteOff";
    case NoteOn:
        return "NoteOn";
    case PolyPressure:
        return "PolyPressure";
    case CC:
        return "CC";
    case ProgramChange:
        return "ProgramChange";
    case Aftertouch:
        return "Aftertouch";
    case PitchBend:
        return "PitchBend";
    default:
        return "unknown";
    }
}

const char *MidiHelper::getNote(u8 value)
{
    int letter = value % 12;
    std::string note;
    // black notes  1 3   6 8 10    c# d#   f# g# a#
    // white notes 0 2 4 5 7 9  11 c  d  e f  g  a  b
    switch (letter)
    {
    case 0:
        note = "C";
        break;
    case 1:
        note = "C#";
        break;
    case 2:
        note = "D";
        break;
    case 3:
        note = "D#";
        break;
    case 4:
        note = "E";
        break;
    case 5:
        note = "F";
        break;
    case 6:
        note = "F#";
        break;
    case 7:
        note = "G";
        break;
    case 8:
        note = "G#";
        break;
    case 9:
        note = "A";
        break;
    case 10:
        note = "A#";
        break;
    case 11:
        note = "B";
        break;
    }

    // number
    int number = value % 128;
    int output = number / 12;
    output -= 2;
    note.append(std::to_string(output));
    return note.c_str();
}

const char *MidiHelper::getCCs(u8 cc_number, u8 value)
{
    std::string cc_event = std::to_string(cc_number) + " = " + std::to_string(value);
    return cc_event.c_str();
}

bool MidiHelper::isFlat(int note_value)
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
