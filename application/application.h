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
            TEXT = 0,
            PIANO = 1,
            ACCORD = 2
        */
    int visualizationmode;

    mios32_midi_package_t lastReceivedPackage;
    notestack_t notestack;
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
    mios32_midi_package_t getLastReceivedPackage();

    void changeVisualizationMode();

    int getVisualizationMode();
    const char *getVisualizationModeAsString();

    void draw();

    notestack_t *getNotestack();
};

#endif