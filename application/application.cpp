#include "application.h"
#include "visualization/text.h"
#include "visualization/keyboard.h"

Application::Application()
{
    MIOS32_MIDI_SendDebugString("called application constructor");

    // notestack init
    for (size_t i = 0; i < 16; i++)
    {
        notestack_item_t nodestackItems[NOTESTACK_SIZE];
        NOTESTACK_Init(&notestack[i], NOTESTACK_MODE_PUSH_TOP, nodestackItems, NOTESTACK_SIZE);
    }

    // NOTESTACK_Init(, NOTESTACK_MODE_PUSH_TOP, &notestack_items[0], NOTESTACK_SIZE);
}

// combination of channel switch and holding notes in mios studio seems to mess with values
// could also be an issue with higher notes, than our display can show. ??? INVESTIGATE!
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
    MIOS32_MIDI_SendDebugMessage("visualizationmode: %s", getVisualizationModeAsString());
}

void Application::changeSelectedChannel()
{
    if (selectedChannel > 15)
        selectedChannel = 0;
    else
        selectedChannel++;
    MIOS32_MIDI_SendDebugMessage("channel: %d", (selectedChannel + 1));
}

const char *Application::getVisualizationModeAsString()
{
    if (visualizationmode)
        return "TEXT";
    else
        return "KEYBOARD";
}

void Application::draw()
{
    // clear all displays
    MIOS32_LCD_Clear();
    if (visualizationmode)
        Text::draw(lastReceivedPackage[selectedChannel]);
    else
    {
        Keyboard::drawKeyboard();
        Keyboard::drawNotestack(notestack[selectedChannel]);
    }
    // show selected channel on screen
    MIOS32_LCD_DeviceSet(0);
    MIOS32_LCD_CursorSet(0, 6); // X, Y
    if (selectedChannel > 15)
        MIOS32_LCD_PrintString("selected channel: all");
    else
        MIOS32_LCD_PrintFormattedString("selected channel: %d", (selectedChannel + 1));
}

notestack_t *Application::getNotestack()
{
    return &notestack[selectedChannel];
}