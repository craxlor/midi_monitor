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

#include <mios32.h>
#include <string>

class MidiPackage
{
    private:
        mios32_midi_package_t midi_package;

    public:
        MidiPackage(mios32_midi_package_t package);
        
        int getChannel();
        
        const char *getVelocity();

        const char *getType();

        const char *getNote();

        const char *getCCs();
};

#endif