/**
 * @file application.cpp
 * @author Arnaud Kalthoff
 * @brief
 * @version 0.1
 * @date 2022-07-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "application/application.h"

void Application::setLastReceivedPackage(mios32_midi_package_t p)
{
    lastReceivedPackage = p;
}

mios32_midi_package_t Application::getLastReceivedPackage()
{
    return lastReceivedPackage;
}
