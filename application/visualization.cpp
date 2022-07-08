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
    MidiPackage package(Application::getInstance().getLastReceivedPackage());

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
        break;
    case 3:
        /*TODO implement accord visualization*/
        break;
    }
}

void Visualization::changeVisualizationMode()
{
    if (visualizationmode == 3)
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
