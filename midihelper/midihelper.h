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

    static const char *getCC(u8 cc_number);

    static bool isFlat(int note_value);

    static const char *getProgramChange(u8 value);

    static int getPitchBend(u8 value1, u8 value2);
};

#endif