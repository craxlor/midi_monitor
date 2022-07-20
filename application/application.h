#ifndef APPLICATION_H
#define APPLICATION_H

#include <mios32.h>
class Application
{
private:
    // private constructor
    Application() {}
    /**
            TEXT = 0,
            PIANO = 1,
            ACCORD = 2
        */
    int visualizationmode;

    mios32_midi_package_t lastReceivedPackage;

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
};

#endif