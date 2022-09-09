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
#include "midihelper/midihelper.h"
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
  mios32_enc_config_t enc_config;
  enc_config.cfg.sr = 2;
  enc_config.cfg.type = DETENTED2;
  enc_config.cfg.speed = FAST;
  enc_config.cfg.speed_par = 0;
  enc_config.cfg.pos = 0;
  MIOS32_ENC_ConfigSet(0, enc_config);
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called when a MIDI package has been received
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_MIDI_NotifyPackage(mios32_midi_port_t port, mios32_midi_package_t midi_package)
{
  APP.setLastReceivedPackage(midi_package);
  APP.draw();
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called when a button has been toggled
// pin_value is 1 when button released, and 0 when button pressed
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_DIN_NotifyToggle(u32 pin, u32 pin_value)
{
  switch (pin) // determine button
  {
  case 17:               // encoder button
    if (pin_value == 0) // has been pressed
    {
      APP.changeVisualizationMode();
    }
    break;
  }
  APP.draw();
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called when an encoder has been moved
// incrementer is positive when encoder has been turned clockwise, else
// it is negative
/////////////////////////////////////////////////////////////////////////////
extern "C" void APP_ENC_NotifyChange(u32 encoder, s32 incrementer)
{
  mios32_enc_config_t encoder_cfg = MIOS32_ENC_ConfigGet(encoder);
  if (encoder_cfg.cfg.pos == 0 && encoder_cfg.cfg.sr == 2) {
    //incrementer negative -> clockwise turn
    APP.changeSelectedChannel(incrementer < 0);
  }
}
