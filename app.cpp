// $Id$
/*
 * MIOS32 Application Template
 *
 * ==========================================================================
 *
 *  Copyright (C) <year> <your name> (<your email address>)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 *
 * ==========================================================================
 */

/////////////////////////////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////////////////////////////

#include <mios32.h>
#include <app.h>
#include <string>
#include "midipackage.h"
#include "application/application.h"

#define APP Application::getInstance()

/////////////////////////////////////////////////////////////////////////////
// This hook is called after startup to initialize the application
// It's delcared as "extern C" so that the MIOS32 programming model can
// access this function - you can safely write your own functions in C++
// In other words: there is no need to add "extern C" to your own functions!
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_Init(void)
{
  MIOS32_MIDI_SendDebugString("initialisiere keybaord...");
  APP.visualization.initKeyboard();
}

/////////////////////////////////////////////////////////////////////////////
// This task is running endless in background
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_Background(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called each mS from the main task which also handles DIN, ENC
// and AIN events. You could add more jobs here, but they shouldn't consume
// more than 300 uS to ensure the responsiveness of buttons, encoders, pots.
// Alternatively you could create a dedicated task for application specific
// jobs as explained in $MIOS32_PATH/apps/tutorials/006_rtos_tasks
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_Tick(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called each mS from the MIDI task which checks for incoming
// MIDI events. You could add more MIDI related jobs here, but they shouldn't
// consume more than 300 uS to ensure the responsiveness of incoming MIDI.
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_MIDI_Tick(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called when a MIDI package has been received
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_MIDI_NotifyPackage(mios32_midi_port_t port, mios32_midi_package_t midi_package)
{
  APP.setLastReceivedPackage(midi_package);
  APP.visualization.draw();

  // for debugging which packages are being received
  MidiPackage package(midi_package);
  MIOS32_MIDI_SendDebugMessage("package content: note %02s, velo %d, cc %s, chan %d, type %s",
                               package.getNote(), package.getVelocity(), package.getCCs(), package.getChannel(), package.getType());
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called before the shift register chain is scanned
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_SRIO_ServicePrepare(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called after the shift register chain has been scanned
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_SRIO_ServiceFinish(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called when a button has been toggled
// pin_value is 1 when button released, and 0 when button pressed
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_DIN_NotifyToggle(u32 pin, u32 pin_value)
{

  switch (pin) // determine button
  {
  case 2:               // encoder button
    if (pin_value == 0) // has been pressed
    {
      APP.visualization.changeVisualizationMode();
    }
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called when an encoder has been moved
// incrementer is positive when encoder has been turned clockwise, else
// it is negative
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_ENC_NotifyChange(u32 encoder, s32 incrementer)
{
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called when a pot has been moved
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_AIN_NotifyChange(u32 pin, u32 pin_value)
{
}
