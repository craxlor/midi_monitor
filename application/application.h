#ifndef APPLICATION_H
#define APPLICATION_H

#include "application/visualization.h"
#include <mios32.h>
class Application
{
private:
    // private constructor
    Application() {}
    
    mios32_midi_package_t lastReceivedPackage;

public:
    Visualization visualization;

    static Application &getInstance()
    {
        static Application INSTANCE;
        return INSTANCE;
    }
    Application(Application const &) = delete;
    void operator=(Application const &) = delete;

    void setLastReceivedPackage(mios32_midi_package_t p);
    mios32_midi_package_t getLastReceivedPackage();
};

#endif