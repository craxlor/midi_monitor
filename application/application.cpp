#include "application.h"
#include "visualization/text.h"
#include "visualization/keyboard.h"

Application::Application()
{
    MIOS32_MIDI_SendDebugString("called application constructor");
    NOTESTACK_Init(&notestack, NOTESTACK_MODE_PUSH_TOP, &notestack_items[0], NOTESTACK_SIZE);
}

void Application::setLastReceivedPackage(mios32_midi_package_t p)
{
    if (p.type == NoteOn)
    {
        NOTESTACK_Push(&notestack, p.note, p.velocity);
    }
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

notestack_t *Application::getNotestack()
{
    return &notestack;
}