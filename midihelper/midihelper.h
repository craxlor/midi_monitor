#ifndef MIDIPACKAGE_H
#define MIDIPACKAGE_H

#include <mios32.h>
#include <string>

class MidiHelper
{
public:
    static int getChannel(u8 chn);

    static const char *getType(u8 type);

    static const char *getNote(u8 value);

    static const char *getCCs(u8 cc_number, u8 value);

    static bool isFlat(int note_value);
};

#endif