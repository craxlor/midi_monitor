// $Id$
/*
 * Header file for ESP8266 driver
 *
 * ==========================================================================
 *
 *  Copyright (C) 2016 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#ifndef _ESP8266_H
#define _ESP8266_H

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////

//! support for direct send command? (! in terminal)
#ifndef ESP8266_TERMINAL_DIRECT_SEND_CMD
#define ESP8266_TERMINAL_DIRECT_SEND_CMD 0
#endif

//! it's recommended to assign the MIDIOUT mutex used by the application in mios32_config.h
#ifndef ESP8266_MUTEX_MIDIOUT_TAKE
#define ESP8266_MUTEX_MIDIOUT_TAKE { }
#endif
#ifndef ESP8266_MUTEX_MIDIOUT_GIVE
#define ESP8266_MUTEX_MIDIOUT_GIVE { }
#endif


/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

extern s32 ESP8266_Init(u32 mode);

extern s32 ESP8266_Periodic_mS(void);

extern s32 ESP8266_TerminalModeSet(u8 terminal_mode);

extern s32 ESP8266_UdpRxCallback_Init(s32 (*callback_rx)(u32 ip, u16 port, u8 *payload, u32 len));

extern s32 ESP8266_InitUart(mios32_midi_port_t port, u32 baudrate);
extern s32 ESP8266_DeInitUart(void);
extern mios32_midi_port_t ESP8266_UartGet(void);

extern s32 ESP8266_TerminalHelp(void *_output_function);
extern s32 ESP8266_TerminalParseLine(char *input, void *_output_function);

extern s32 ESP8266_SendCommand(const char* cmd);
extern s32 ESP8266_COM_SendUdpPacket(u32 ip, u16 port, u8 *payload, u16 len);


/////////////////////////////////////////////////////////////////////////////
// Export global variables
/////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif /* _ESP8266_H */
