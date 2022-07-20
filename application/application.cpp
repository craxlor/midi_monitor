#include "application/application.h"

void Application::setLastReceivedPackage(mios32_midi_package_t p)
{
    lastReceivedPackage = p;
}

mios32_midi_package_t Application::getLastReceivedPackage()
{
    return lastReceivedPackage;
}
