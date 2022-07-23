#ifndef APPLICATION_H
#define APPLICATION_H

#include <mios32.h>
#include <notestack.h>

#define NOTESTACK_SIZE 16

class Application
{
private:
    // private constructor
    Application();
    /**
            TEXT = true,
            PIANO = false,
        */
    bool visualizationmode = true;

    int selectedChannel = 0;
    mios32_midi_package_t lastReceivedPackage;
    notestack_t notestack[16];
    notestack_item_t notestack_items[NOTESTACK_SIZE];

public:
    static Application &getInstance()
    {
        static Application INSTANCE;
        return INSTANCE;
    }
    Application(Application const &) = delete;
    void operator=(Application const &) = delete;

    void setLastReceivedPackage(mios32_midi_package_t p);

    void changeVisualizationMode();

    const char *getVisualizationModeAsString();

    void changeSelectedChannel(bool directionUp);

    void draw();

    notestack_t *getNotestack();
};

#endif