/**
 * @file application.h
 * @authors Arnaud Kalthoff
 * @brief The Application class is implemented as a singleton.
 *
 * @copyright Copyright (c) 2022
 */
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
     * true  = TEXT
     * false = KEYBOARD
     */
    bool visualizationmode = true;
    //holds the midichannel which shall be monitored
    int selectedChannel = 0;
    /**
     * array of size 17
     * 0-15 save the last received midi package of the accoring midichannel
     * 16 save the last received midi package independent of the midichannel
     */
    mios32_midi_package_t lastReceivedPackage[17];
    // save played notes of the accoring midichannel
    notestack_t notestack[16];
    /**
     * 2 dimensional array
     * 1st dimension describes the size of notestack
     * 2nd dimension stands for each midichannel
     */
    notestack_item_t notestack_items[NOTESTACK_SIZE][16];

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

    void changeSelectedChannel(bool directionUp);

    void draw();

    notestack_t *getNotestack();
};

#endif