#ifndef TEXT_H
#define TEXT_H

#include <mios32.h>
/**
 * @brief The Text class contains all the attributes & methods needed to implement the text visualisation.
 * @authors Arnaud Kalthoff
 */
class Text
{
public:
    static void draw(mios32_midi_package_t package);
};

#endif