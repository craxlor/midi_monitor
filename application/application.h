/**
 * @file application.h
 * @author Arnaud Kalthoff
 * @brief https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
 * @version 0.1
 * @date 2022-06-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef APPLICATION_H
#define APPLICATION_H

#include "application/visualization.h"
#include "application/circular_buffer.h"
#include <mios32.h>
class Application
{
private:
    // private constructor
    Application() {}

public:
    Visualization visualization;

    circular_buffer<mios32_midi_package_t> buffer(10);

    static Application &getInstance()
    {
        static Application INSTANCE;
        return INSTANCE;
    }
    Application(Application const &) = delete;
    void operator=(Application const &) = delete;
};

#endif