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
#include <mios32.h>

// TODO retrieve midi package from ring buffer in application instance
void Visualization::draw()
{
    switch (visualizationmode)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
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
