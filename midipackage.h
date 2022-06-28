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
#ifndef MIDIPACKAGE_H
#define MIDIPACKAGE_H

#include <mios32_midi.h>
#include <string>

class MidiPackage
{
private:
    mios32_midi_package_t midi_package;

public:
    MidiPackage(mios32_midi_package_t package)
    {
        midi_package = package;
    }

    int getChannel()
    {
        return midi_package.chn + 1;
    }

    int getVelocity()
    {
        return midi_package.velocity;
    }

    const char *getType()
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
            return "event is not defined";
        }
    }

    const char *getNote()
    {
        std::string note;
        int value = midi_package.note;
        int letter = value % 12;
        // black notes  1 3   6 8 10    c# d#   f# g# a#
        // white notes 0 2 4 5 7 9  11 c  d  e f  g  a  b
        switch (letter)
        {
        case 0:
            note = "c";
            break;
        case 1:
            note = "c#";
            break;
        case 2:
            note = "d";
            break;
        case 3:
            note = "d#";
            break;
        case 4:
            note = "e";
            break;
        case 5:
            note = "f";
            break;
        case 6:
            note = "f#";
            break;
        case 7:
            note = "g";
            break;
        case 8:
            note = "g#";
            break;
        case 9:
            note = "a";
            break;
        case 10:
            note = "a#";
            break;
        case 11:
            note = "a";
            break;
        default:
            break;
        }

        // number
        if (value > 119)
        {
            note.append("8");
        }
        else if (value > 107)
        {
            note.append("7");
        }
        else if (value > 95)
        {
            note.append("6");
        }
        else if (value > 83)
        {
            note.append("5");
        }
        else if (value > 71)
        {
            note.append("4");
        }
        else if (value > 59)
        {
            note.append("3");
        }
        else if (value > 47)
        {
            note.append("2");
        }
        else if (value > 35)
        {
            note.append("1");
        }
        else if (value > 23)
        {
            note.append("0");
        }
        else if (value > 11)
        {
            note.append("-1");
        }
        else
        {
            note.append("-2");
        }

        return note.c_str();
    }

    // TODO missing implementation
    const char *getCCs();
};

#endif