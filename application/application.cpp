#include "application.h"
#include "visualization/text.h"
#include "visualization/keyboard.h"

void Application::setLastReceivedPackage(mios32_midi_package_t p)
{
    lastReceivedPackage = p;
}

mios32_midi_package_t Application::getLastReceivedPackage()
{
    return lastReceivedPackage;
}

void Application::changeVisualizationMode()
{
    if (visualizationmode > 1)
    {
        visualizationmode = 0;
    }
    else
    {
        visualizationmode++;
    }
    MIOS32_MIDI_SendDebugMessage("Visualizationmode:%s", getVisualizationModeAsString());
}

int Application::getVisualizationMode()
{
    return visualizationmode;
}

const char *Application::getVisualizationModeAsString()
{
    switch (visualizationmode)
    {
    case 0:
        return "TEXT";
    case 1:
        return "PIANO";
    case 2:
        return "CHORD";
    default:
        return "Visualization-Mode is not defined";
    }
}

void Application::draw()
{
    // last received midi package
    mios32_midi_package_t package = Application::getInstance().getLastReceivedPackage();
    // clear all displays
    MIOS32_LCD_Clear();
    switch (visualizationmode)
    {
    case 0:
        Text::draw(package);
        break;
    case 1:
        Keyboard::drawKeyboard();
        Keyboard::drawKeyPress(package);
        break;
    case 2:
        /*TODO implement accord visualization*/
        break;
    }
}