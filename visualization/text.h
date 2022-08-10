/**
 * @file text.h
 * @author Arnaud Kalthoff
 * @brief The Text class contains all the attributes & methods needed to implement the text visualisation.
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef TEXT_H
#define TEXT_H

#include <mios32.h>
class Text
{
public:
    static void draw(mios32_midi_package_t package);
};

#endif