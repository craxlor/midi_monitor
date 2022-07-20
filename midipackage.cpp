/**
 * @file midipackage.h
 * @author Arnaud Kalthoff
 * @brief
 * @version 0.1
 * @date 2022-06-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <mios32.h>
#include <string>
#include "midipackage.h"

MidiPackage::MidiPackage(mios32_midi_package_t package)
{
    midi_package = package;
}

int MidiPackage::getChannel()
{
    return midi_package.chn + 1;
}

int MidiPackage::getVelocity()
{
    return midi_package.velocity;
}

const char *MidiPackage::getType()
{
    switch (midi_package.type)
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

const char *MidiPackage::getNote()
{
    std::string note;
    int value = midi_package.note;
    int letter = value % 12;
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

const char *MidiPackage::getCCs()
{
    std::string cc_event = std::to_string(midi_package.cc_number) + " = " + std::to_string(midi_package.value);
    return cc_event.c_str();
}
