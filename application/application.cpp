/**
 * @file application.cpp
 * @authors Arnaud Kalthoff, DOminic Jabs, Dave Jindal
 * @brief implements all methods of application.h
 * 
 * @copyright Copyright (c) 2022
 */
#include "application.h"
#include "visualization/text.h"
#include "visualization/keyboard.h"

Application::Application()
{
    // notestack init
    for (size_t i = 0; i < 16; i++)
    {
        NOTESTACK_Init(&notestack[i], NOTESTACK_MODE_PUSH_TOP, notestack_items[i], NOTESTACK_SIZE);
    }
}

void Application::setLastReceivedPackage(mios32_midi_package_t p)
{
    lastReceivedPackage[p.chn] = p; // store package according to its channel
    lastReceivedPackage[16] = p;

    if (p.type == NoteOn)
    {
        NOTESTACK_Push(&notestack[p.chn], p.note, p.velocity);
    }
    if (p.type == NoteOff)
    {
        NOTESTACK_Pop(&notestack[p.chn], p.note);
    }
}

void Application::changeVisualizationMode()
{
    visualizationmode = !visualizationmode;
}

void Application::changeSelectedChannel(bool directionUp)
{
    if (directionUp)
    {
        if (selectedChannel >= 16)
            selectedChannel = 0;
        else
            selectedChannel++;
    }
    else
    {
        if (selectedChannel <= 0)
            selectedChannel = 16;
        else
            selectedChannel--;
    }
}

void Application::draw()
{
    //selected channel depending on selected mode
    int contextSelectedChannel;
    if (!visualizationmode && selectedChannel == 16)
        contextSelectedChannel = 15;
    else
        contextSelectedChannel = selectedChannel;
        
    
    // clear all displays
    MIOS32_LCD_Clear();
    if (visualizationmode)
        Text::draw(lastReceivedPackage[contextSelectedChannel]);
    else
    {   
        Keyboard::drawKeyboard();
        Keyboard::drawNotestack(notestack[contextSelectedChannel]);
    }
    // Print selected channel on screen
    MIOS32_LCD_DeviceSet(0);
    MIOS32_LCD_CursorSet(0, 6); // X, Y

    if (contextSelectedChannel > 15)
        MIOS32_LCD_PrintString("selected channel: all");
    // MIDI-Channel 1 - 16
    else
        MIOS32_LCD_PrintFormattedString("selected channel: %d", (contextSelectedChannel + 1));
}

notestack_t *Application::getNotestack()
{
    return &notestack[selectedChannel];
}