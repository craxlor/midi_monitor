/**
 * @file visualization.cpp
 * @author Arnaud Kalthoff
 * @brief
 * @version 0.1
 * @date 2022-07-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "application/visualization.h"
#include "application/application.h"
#include "midipackage.h"

void Visualization::draw()
{
    // last received midi package
    mios32_midi_package_t p = Application::getInstance().getLastReceivedPackage();
    MidiPackage package(p);

    switch (visualizationmode)
    {
    case 0:
        /*TODO implement raw visualization*/
        break;
    case 1:
        /*TODO implement text visualization*/
        break;
    case 2:
        /*TODO implement piano visualization*/
        drawKeyboard(p);
        break;
    case 3:
        /*TODO implement accord visualization*/
        break;
    }
}

void Visualization::drawKeyboard(mios32_midi_package_t midi_package)
{
    int value = midi_package.note;
    int octave;

    u8 byteToDraw;
    int height;
    if (isFlat(value)) // black key
    {
        height = 15;
        if (midi_package.type == NoteOn)
        {
            byteToDraw = 0x7F;
        }
        else
        {
            byteToDraw = 0x00;
        }
    }
    else // white key
    {
        height = 26;
        if (midi_package.type == NoteOn)
        {
            byteToDraw = 0x00;
        }
        else
        {
            byteToDraw = 0xFF;
        }
    }

    if (value > 23 && value <= 35)
    {
        octave = 84;
    }
    else if (value > 11)
    {
        octave = 42;
    }
    else
    {
        octave = 0;
    }

    int pixelColumnIndex = ((value % 12) + 1) * 3 + octave;

    MIOS32_LCD_GCursorSet(pixelColumnIndex - 2, height);
    MIOS32_LCD_Data(byteToDraw);
    MIOS32_LCD_GCursorSet(pixelColumnIndex - 1, height);
    MIOS32_LCD_Data(byteToDraw);
    MIOS32_LCD_GCursorSet(pixelColumnIndex, height);
    MIOS32_LCD_Data(byteToDraw);
}

bool Visualization::isFlat(int note_value)
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

void Visualization::changeVisualizationMode()
{
    if (visualizationmode > 2)
    {
        visualizationmode = 0;
    }
    else
    {
        visualizationmode++;
    }
    MIOS32_MIDI_SendDebugMessage("Visualizationmode:%s", getVisualizationModeAsString());
}

int Visualization::getVisualizationMode()
{
    return visualizationmode;
}

const char *Visualization::getVisualizationModeAsString()
{
    switch (visualizationmode)
    {
    case 0:
        return "RAW";
    case 1:
        return "TEXT";
    case 2:
        return "PIANO";
    case 3:
        return "ACCORD";
    default:
        return "visualizationmode is not defined";
    }
}
