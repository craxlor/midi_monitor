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

const char *MidiHelper::getProgramChange(u8 value)
{
    std::string name = "undefined";
    switch (value)
    {
        // PIANO
    case 0:
        name = "Acoustic grand piano";
        break;
    case 1:
        name = "Bright acoustic piano";
        break;
    case 2:
        name = "Electric grand piano";
        break;
    case 3:
        name = "Honky tonk piano";
        break;
    case 4:
        name = "Electric piano 1";
        break;
    case 5:
        name = "Electric piano 2";
        break;
    case 6:
        name = "Harpsicord";
        break;
    case 7:
        name = "Clavinet";
        break;
        // Chromatic percussion
    case 8:
        name = "Celesta";
        break;
    case 9:
        name = "Glockenspiel";
        break;
    case 10:
        name = "Music box";
        break;
    case 11:
        name = "Vibraphone";
        break;
    case 12:
        name = "Marimba";
        break;
    case 13:
        name = "Xylophone";
        break;
    case 14:
        name = "Tubular bell";
        break;
    case 15:
        name = "Dulcimer";
        break;
        // Organ
    case 16:
        name = "Hammond / drawbar organ";
        break;
    case 17:
        name = "Percussive organ";
        break;
    case 18:
        name = "Rock organ";
        break;
    case 19:
        name = "Church organ";
        break;
    case 20:
        name = "Reed organ";
        break;
    case 21:
        name = "Accordion";
        break;
    case 22:
        name = "Harmonica";
        break;
    case 23:
        name = "Tango accordion";
        break;
        // Guitar
    case 24:
        name = "Nylon string acoustic guitar";
        break;
    case 25:
        name = "Steel string acoustic guitar";
        break;
    case 26:
        name = "Jazz electric guitar";
        break;
    case 27:
        name = "Clean electric guitar";
        break;
    case 28:
        name = "Muted electric guitar";
        break;
    case 29:
        name = "Overdriven guitar";
        break;
    case 30:
        name = "Distortion guitar";
        break;
    case 31:
        name = "Guitar harmonics";
        break;
        // Bass
    case 32:
        name = "Acoustic bass";
        break;
    case 33:
        name = "Fingered electric bass";
        break;
    case 34:
        name = "Picked electric bass";
        break;
    case 35:
        name = "Fretless bass";
        break;
    case 36:
        name = "Slap bass 1";
        break;
    case 37:
        name = "Slap bass 2";
        break;
    case 38:
        name = "Synth bass 1";
        break;
    case 39:
        name = "Synth bass 2";
        break;
        // Strings
    case 40:
        name = "Violin";
        break;
    case 41:
        name = "Viola";
        break;
    case 42:
        name = "Cello";
        break;
    case 43:
        name = "Contrabass";
        break;
    case 44:
        name = "Tremolo strings";
        break;
    case 45:
        name = "Pizzicato strings";
        break;
    case 46:
        name = "Orchestral strings / harp";
        break;
    case 47:
        name = "Timpani";
        break;
        // Ensemble
    case 48:
        name = "String ensemble 1";
        break;
    case 49:
        name = "String ensemble 2 / slow strings";
        break;
    case 50:
        name = "Synth strings 1";
        break;
    case 51:
        name = "Synth strings 2";
        break;
    case 52:
        name = "Choir aahs";
        break;
    case 53:
        name = "Voice oohs";
        break;
    case 54:
        name = "Synth choir / voice";
        break;
    case 55:
        name = "Orchestra hit";
        break;
        // Brass
    case 56:
        name = "Trumpet";
        break;
    case 57:
        name = "Trombone";
        break;
    case 58:
        name = "Tuba";
        break;
    case 59:
        name = "Muted trumpet";
        break;
    case 60:
        name = "French horn";
        break;
    case 61:
        name = "Brass ensemble";
        break;
    case 62:
        name = "Synth brass 1";
        break;
    case 63:
        name = "Synth brass 2";
        break;
        // Reed
    case 64:
        name = "Soprano sax";
        break;
    case 65:
        name = "Alto sax";
        break;
    case 66:
        name = "Tenor sax";
        break;
    case 67:
        name = "Baritone sax";
        break;
    case 68:
        name = "Oboe";
        break;
    case 69:
        name = "English horn";
        break;
    case 70:
        name = "Bassoon";
        break;
    case 71:
        name = "Clarinet";
        break;
        // Pipe
    case 72:
        name = "Piccolo";
        break;
    case 73:
        name = "Flute";
        break;
    case 74:
        name = "Recorder";
        break;
    case 75:
        name = "Pan flute";
        break;
    case 76:
        name = "Bottle blow / blown bottle";
        break;
    case 77:
        name = "Shakuhachi";
        break;
    case 78:
        name = "Whistle";
        break;
    case 79:
        name = "Ocarina";
        break;
        // Synth lead
    case 80:
        name = "Synth square wave";
        break;
    case 81:
        name = "Synth saw wave";
        break;
    case 82:
        name = "Synth calliope";
        break;
    case 83:
        name = "Synth chiff";
        break;
    case 84:
        name = "Synth charang";
        break;
    case 85:
        name = "Synth voice";
        break;
    case 86:
        name = "Synth fifths saw";
        break;
    case 87:
        name = "Synth brass and lead";
        break;
        // Synth pad
    case 88:
        name = "Fantasia / new age";
        break;
    case 89:
        name = "Warm pad";
        break;
    case 90:
        name = "Polysynth";
        break;
    case 91:
        name = "Space vox / choir";
        break;
    case 92:
        name = "Bowed glass";
        break;
    case 93:
        name = "Metal pad";
        break;
    case 94:
        name = "Halo pad";
        break;
    case 95:
        name = "Sweep pad";
        break;
        // Synth effects
    case 96:
        name = "Ice rain";
        break;
    case 97:
        name = "Soundtrack";
        break;
    case 98:
        name = "Crystal";
        break;
    case 99:
        name = "Atmosphere";
        break;
    case 100:
        name = "Brightness";
        break;
    case 101:
        name = "Goblins";
        break;
    case 102:
        name = "Echo drops / echoes";
        break;
    case 103:
        name = "Sci fi";
        break;
        // Ethnic
    case 104:
        name = "Sitar";
        break;
    case 105:
        name = "Banjo";
        break;
    case 106:
        name = "Shamisen";
        break;
    case 107:
        name = "Koto";
        break;
    case 108:
        name = "Kalimba";
        break;
    case 109:
        name = "Bag pipe";
        break;
    case 110:
        name = "Fiddle";
        break;
    case 111:
        name = "Shanai";
        break;
        // Percussive
    case 112:
        name = "Tinkle bell";
        break;
    case 113:
        name = "Agogo";
        break;
    case 114:
        name = "Steel drums";
        break;
    case 115:
        name = "Woodblock";
        break;
    case 116:
        name = "Taiko drum";
        break;
    case 117:
        name = "Melodic tom";
        break;
    case 118:
        name = "Synth drum";
        break;
    case 119:
        name = "Reverse cymbal";
        break;
        // Sound effects
    case 120:
        name = "Guitar fret noise";
        break;
    case 121:
        name = "Breath noise";
        break;
    case 122:
        name = "Seashore";
        break;
    case 123:
        name = "Bird tweet";
        break;
    case 124:
        name = "Telephone ring";
        break;
    case 125:
        name = "Helicopter";
        break;
    case 126:
        name = "Applause";
        break;
    case 127:
        name = "Gunshot";
        break;
    }
    std::string result = value + " - " + name;
    return result.c_str();
}

int MidiHelper::getPitchBend(u8 value1, u8 value2)
{
    return ((value2 << 7) | (value1 & 0x7f)) - 8192;
}