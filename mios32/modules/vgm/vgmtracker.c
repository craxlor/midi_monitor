/*
 * VGM Data and Playback Driver: Tracker system
 *
 * ==========================================================================
 *
 *  Copyright (C) 2016 Sauraen (sauraen@gmail.com)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#include "vgmtracker.h"

#include "vgmhead.h"
#include "vgmqueue.h"
#include "vgmtuning.h"
#include <genesis.h>

static VgmSource* qsource;
static VgmHead* qhead;

s8 trackervoicekeys[10*GENESIS_COUNT];
u8 trackerkeyonmodes[6*GENESIS_COUNT];

u8 GetKeyCommand(u8 on, u8 g, u8 chan, u8 ch3_op){
    ch3_op = (ch3_op-1) & 3; //Actual op if != 0, 3 if == 0
    u8 k=0, n, b;
    for(n=0; n<4; ++n){
        k >>= 1;
        switch((trackerkeyonmodes[(6*g)+chan] >> (2*n)) & 3){
            case 0:
                if(ch3_op == 3) b = on; //normal voices or ch3 normal
                else b = genesis[g].opn2.chan[chan].op[n].kon;
                break;
            case 1:
                b = 0;
                break;
            case 2:
                b = 1;
                break;
            default:
                if(n == ch3_op) b = on;
                else b = genesis[g].opn2.chan[chan].op[n].kon;
                break;
        }
        k |= (b << 3);
    }
    k <<= 4;
    k |= ((chan < 3) ? chan : chan+1);
    return k;
}

void VGM_Tracker_Init(){
    qsource = VGM_SourceQueue_Create();
    qsource->opn2clock = genesis_clock_opn2;
    qsource->psgclock = genesis_clock_psg;
    qhead = VGM_Head_Create(qsource, 0x1000, 0x1000, 0);
    qhead->playing = 1;
    u8 i;
    for(i=0; i<10*GENESIS_COUNT; ++i){
        trackervoicekeys[i] = -1;
    }
}

void VGM_Tracker_Enqueue(VgmChipWriteCmd cmd, u8 fixfreq){
    VGM_HeadQueue_Enqueue(qhead, cmd, fixfreq);
}

void VGM_ResetChipVoiceAsync(u8 g, u8 v){
    u8 chan = v <= 7 ? v-1 : v-8;
    u8 psgcmd = (g << 4);
    u8 opn2globcmd = psgcmd | 2;
    u8 opn2cmd = opn2globcmd | (v >= 4 && v <= 6);
    if(v == 0){
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x21, .data = 0x00 }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x22, .data = 0x00 }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2C, .data = 0x00 }, 0);
    }else if(v <= 6){
        //Key off
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x28, .data = ((chan < 3) ? chan : chan+1) }, 0);
        u8 subchan = chan % 3;
        u8 i = 0x80 | subchan;
        //Set release rate to full so EG states return to 0 while we're writing the other stuff
        for(; i<0x90; i+=4){
            VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = i, .data = 0xFF }, 0);
        }
        //Set all channel registers to 0, except output
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0xA4 | subchan, .data = 0x00, .data2 = 0x00 }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0xB0 | subchan, .data = 0x00 }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0xB4 | subchan, .data = 0xC0 }, 0);
        i = 0x30 | subchan;
        //Set all operator registers to 0
        for(; i<0xA0; i+=4){
            VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = i, .data = 0x00 }, 0);
        }
    }else if(v == 7){
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2B, .data = 0x00 }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2A, .data = 0x80 }, 0);
    }else if(v <= 10){
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = psgcmd, .data = (chan<<5)|0x8F, .data2 = 0x3F }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = psgcmd, .data = (chan<<5)|0x9F }, 0);
    }else if(v == 11){
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = psgcmd, .data = 0xE0 }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = psgcmd, .data = 0xFF }, 0);
    }
}

void VGM_PartialResetChipVoiceAsync(u8 g, u8 v){
    u8 chan = v <= 7 ? v-1 : v-8;
    u8 psgcmd = (g << 4);
    u8 opn2globcmd = psgcmd | 2;
    u8 opn2cmd = opn2globcmd | (v >= 4 && v <= 6);
    if(v == 0){
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x22, .data = 0x00 }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x21, .data = 0x00 }, 0);
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2C, .data = 0x00 }, 0);
    }else if(v <= 6){
        //Key off
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x28, .data = ((chan < 3) ? chan : chan+1) }, 0);
        u8 subchan = chan % 3;
        u8 i = 0x80 | subchan;
        //Set release rate to full so EG states return to 0
        for(; i<0x90; i+=4){
            VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = i, .data = 0xFF }, 0);
        }
    }else if(v == 7){
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2B, .data = 0x00 }, 0);
    }else if(v <= 10){
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = psgcmd, .data = (chan<<5)|0x9F }, 0);
    }else if(v == 11){
        VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = psgcmd, .data = 0xFF }, 0);
    }
}

void VGM_MidiToGenesis(mios32_midi_package_t midi_package, u8 g, u8 v, u8 ch3_op, u8 keyonmask){
    DBG("VGM_MidiToGenesis(event %X, cc %d, value %d, g%d, v%d)", midi_package.event, midi_package.cc_number, midi_package.value, g, v);
    //TODO implement keyonmask
    if(g >= GENESIS_COUNT) return;
    u8 isopn2 = (v >= 1 && v <= 6);
    u8 ispsg = (v >= 8 && v <= 11);
    u8 chan = isopn2 ? v-1 : v-8;
    u8 vlo = chan % 3;
    u8 psgcmd = (g << 4);
    u8 opn2globcmd = psgcmd | 2;
    u8 opn2cmd = opn2globcmd | (v >= 4 && v <= 6);
    u8 voicekeyidx = 10*g + chan + 6*ispsg;
    if(midi_package.event == 0xB){
        //CC
        u8 cc = midi_package.cc_number;
        u8 value = midi_package.value;
        if(ispsg){
            if(cc == 85){
                //Pitch Transposition
                //TODO
            }else if(cc == 83){
                //NTSC/PAL Tuning
                //TODO
            }
        }else if(isopn2){
            //OPN2
            u8 op;
            //Operator CCs
            if(cc >= 16 && cc <= 27){
                op = cc & 0x03;
                cc &= 0xFC;
            }else if(cc >= 39 && cc <= 62){
                op = (cc-39) & 0x03;
                cc = ((cc-39) & 0xFC) + 39;
            }else if(cc >= 70 && cc <= 73){
                op = cc-70;
                cc = 70;
            }else if(cc >= 90 && cc <= 93){
                op = cc-90;
                cc = 90;
            }else if(cc >= 100 && cc <= 113){
                op = cc-100; //actually for DAC custom wave, not op
                cc = 100;
            }else{
                op = 0;
            }
            u8 opaddr = ((op & 1) << 3) | ((op & 2) << 1) | vlo;
            switch(cc){
            case 85:
                //Pitch Transposition
                //TODO
                break;
            case 80:
                //Voice 3 Special Mode
                if(v != 3) return;
                genesis[g].opn2.ch3_mode = GENMDM_DECODE(value,1);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x27, 
                        .data = genesis[g].opn2.timerctrlreg }, 0);
                break;
            case 99: //originally 93, conflicts with SSG-EG
                //Ch3 CSM mode (rather, bit 7 of the register)
                if(v != 3) return;
                genesis[g].opn2.ch3_mode = (genesis[g].opn2.ch3_mode & 1) | (GENMDM_DECODE(value,1) << 1);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x27, 
                        .data = genesis[g].opn2.timerctrlreg }, 0);
                break;
            case 119: //new
                //Ch3 CSM frequency
                if(v != 3) return;
                genesis[g].opn2.timera_high = (genesis[g].opn2.timera_high & 1) | (value << 1);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x24, 
                        .data = genesis[g].opn2.timera_high }, 0);
                break;
            case 6:
                //Preset Instrument Setting Store in RAM
                //TODO
                break;
            case 9:
                //Preset Instrument Setting Recall from RAM
                //TODO
                break;
            case 81:
                //Pitch Bend Amount
                //TODO
                break;
            case 14:
                //FM Algorithm
                genesis[g].opn2.chan[chan].algorithm = GENMDM_DECODE(value,3);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0xB0 | vlo, 
                        .data = genesis[g].opn2.chan[chan].algfbreg }, 0);
                break;
            case 15:
                //FM Feedback
                genesis[g].opn2.chan[chan].feedback = GENMDM_DECODE(value,3);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0xB0 | vlo, 
                        .data = genesis[g].opn2.chan[chan].algfbreg }, 0);
                break;
            case 77:
                //Stereo Configuration
                value = GENMDM_DECODE(value,2);
                genesis[g].opn2.chan[chan].out_l = value & 1;
                genesis[g].opn2.chan[chan].out_r = value >> 1;
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0xB4 | vlo, 
                        .data = genesis[g].opn2.chan[chan].lfooutreg }, 0);
                break;
            case 76:
                //Amplitude Modulation Level
                genesis[g].opn2.chan[chan].lfoampd = GENMDM_DECODE(value,2);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0xB4 | vlo, 
                        .data = genesis[g].opn2.chan[chan].lfooutreg }, 0);
                break;
            case 75:
                //Frequency Modulation Level
                genesis[g].opn2.chan[chan].lfofreqd = GENMDM_DECODE(value,3);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0xB4 | vlo, 
                        .data = genesis[g].opn2.chan[chan].lfooutreg }, 0);
                break;
            case 16:
                //Total Level
                genesis[g].opn2.chan[chan].op[op].totallevel = value;
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x40 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].tlreg }, 0);
                break;
            case 20:
                //Multiple
                genesis[g].opn2.chan[chan].op[op].fmult = GENMDM_DECODE(value,4);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x30 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].freqreg }, 0);
                break;
            case 24:
                //Detune
                genesis[g].opn2.chan[chan].op[op].detune = GENMDM_DECODE(value,3);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x30 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].freqreg }, 0);
                break;
            case 39:
                //Rate Scaling
                genesis[g].opn2.chan[chan].op[op].ratescale = GENMDM_DECODE(value,2);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x50 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].atkrsreg }, 0);
                break;
            case 43:
                //Attack Rate
                genesis[g].opn2.chan[chan].op[op].attackrate = GENMDM_DECODE(value,5);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x50 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].atkrsreg }, 0);
                break;
            case 47:
                //First Decay Rate
                genesis[g].opn2.chan[chan].op[op].decay1rate = GENMDM_DECODE(value,5);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x60 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].dec1amreg }, 0);
                break;
            case 51:
                //Secondary Decay Rate
                genesis[g].opn2.chan[chan].op[op].decay2rate = GENMDM_DECODE(value,5);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x70 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].dec2reg }, 0);
                break;
            case 55:
                //Secondary Amplitude Level
                genesis[g].opn2.chan[chan].op[op].decaylevel = GENMDM_DECODE(value,4);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x80 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].rellvlreg }, 0);
                break;
            case 59:
                //Release Rate
                genesis[g].opn2.chan[chan].op[op].releaserate = GENMDM_DECODE(value,4);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x80 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].rellvlreg }, 0);
                break;
            case 70:
                //Amplitude Modulation Enable
                genesis[g].opn2.chan[chan].op[op].amplfo = GENMDM_DECODE(value,1);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x60 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].dec1amreg }, 0);
                break;
            case 90:
                //SSG-EG
                genesis[g].opn2.chan[chan].op[op].ssgreg = GENMDM_DECODE(value,4);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2cmd, .addr = 0x90 | opaddr, 
                        .data = genesis[g].opn2.chan[chan].op[op].ssgreg }, 0);
                break;
            };
        }else if(v == 0){
            switch(cc){
            case 74:
                //LFO Enable
                genesis[g].opn2.lfo_enabled = GENMDM_DECODE(value,1);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x22, 
                        .data = genesis[g].opn2.lforeg }, 0);
                break;
            case 1:
                //LFO Speed
                genesis[g].opn2.lfo_freq = GENMDM_DECODE(value,3);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x22, 
                        .data = genesis[g].opn2.lforeg }, 0);
                break;
            case 84:
                //Octave Division
                //TODO and also wut?
                break;
            case 83:
                //PAL/NTSC Tuning
                //TODO
                break;
            case 98: //originally 92, conflicts with SSG-EG
                //Timer Controls
                genesis[g].opn2.timerctrlreg = (genesis[g].opn2.timerctrlreg & 0xC0) | GENMDM_DECODE(value,6);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x27, 
                        .data = genesis[g].opn2.timerctrlreg }, 0);
                break;
            case 94:
                //Test Register 0x21 Lowest Four Bits
                genesis[g].opn2.testreg21 = (genesis[g].opn2.testreg21 & 0xF0) | (GENMDM_DECODE(value,4));
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x21, 
                        .data = genesis[g].opn2.testreg21 }, 0);
                break;
            case 95:
                //Test Register 0x21 Highest Four Bits
                genesis[g].opn2.testreg21 = (genesis[g].opn2.testreg21 & 0x0F) | (GENMDM_DECODE(value,4) << 4);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x21, 
                        .data = genesis[g].opn2.testreg21 }, 0);
                break;
            case 96:
                //Test Register 0x2C Lowest Four Bits
                genesis[g].opn2.testreg2C = (genesis[g].opn2.testreg2C & 0xF0) | (GENMDM_DECODE(value,4));
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2C, 
                        .data = genesis[g].opn2.testreg2C }, 0);
                break;
            case 97:
                //Test Register 0x2C Highest Four Bits
                genesis[g].opn2.testreg2C = (genesis[g].opn2.testreg2C & 0x0F) | (GENMDM_DECODE(value,4) << 4);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2C, 
                        .data = genesis[g].opn2.testreg2C }, 0);
                break;
            }
        }else if(v == 7){
            switch(cc){
            case 78:
                //DAC Enable
                genesis[g].opn2.dac_enable = GENMDM_DECODE(value,1);
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2B, 
                        .data = genesis[g].opn2.dacenablereg }, 0);
                break;
            case 79:
                //DAC Direct Data
                genesis[g].opn2.dac_high = value << 1;
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x2A, 
                        .data = genesis[g].opn2.dac_high }, 0);
                break;
            case 86:
                //DAC Sample Pitch Speed
                //TODO
                break;
            case 88:
                //DAC Sample Oversample
                //TODO
                break;
            case 89:
                //DAC Noise / Custom Wave Mode
                //TODO
                break;
            case 100:
                //Custom Wave Byte
                //TODO
                break;
            }
        }
    }else if(midi_package.event == 0x8 || (midi_package.event == 0x9 && midi_package.velocity == 0)){
        //Note Off
        if(isopn2){
            //Key off
            if(midi_package.note == trackervoicekeys[voicekeyidx]){
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x28, .data = GetKeyCommand(0, g, chan, 0) }, 0);
                trackervoicekeys[voicekeyidx] = -1;
            }
        }else if(ispsg){
            //PSG square or noise channel
            if(midi_package.note == trackervoicekeys[voicekeyidx]){
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = (g << 4), .data = (chan<<5)|0x9F }, 0);
                trackervoicekeys[voicekeyidx] = -1;
            }
        }else if(ch3_op){
            //OPN2 ch3 additional frequency
            //This sends a command identical to the current state if this ch3_op shouldn't be keying anything off
            VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x28, .data = GetKeyCommand(0, g, 2, ch3_op) }, 0);
        }
    }else if(midi_package.event == 0x9){
        //Note On
        if(isopn2){
            //TODO under what conditions is velocity mapped to TL?
            if(trackervoicekeys[voicekeyidx] >= 0){
                //Send note off first
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x28, .data = GetKeyCommand(0, g, chan, 0) }, 0);
            }
            trackervoicekeys[voicekeyidx] = midi_package.note;
            u8 subchan = (chan < 3) ? chan : chan+1;
            VgmChipWriteCmd cmd = VGM_getOPN2Frequency(midi_package.note, 0, genesis_clock_opn2); //TODO cents
            cmd.cmd = (g << 4) | 2 | (subchan>>2);
            cmd.addr = 0xA4 | (subchan & 0x03);
            VGM_HeadQueue_Enqueue(qhead, cmd, 0);
            //Key on
            cmd.cmd &= 0xFE; //Switch to low bank
            cmd.addr = 0x28;
            cmd.data = GetKeyCommand(1, g, chan, 0);
            VGM_HeadQueue_Enqueue(qhead, cmd, 0);
        }else if(ispsg && v != 11){
            //PSG square channel
            trackervoicekeys[voicekeyidx] = midi_package.note;
            VgmChipWriteCmd cmd = VGM_getPSGFrequency(midi_package.note, 0, genesis_clock_psg); //TODO cents
            cmd.data |= (chan<<5) | 0x80;
            cmd.cmd = (g << 4);
            VGM_HeadQueue_Enqueue(qhead, cmd, 0);
            VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = (g << 4), .data = (chan<<5)|0x90|(15-(midi_package.velocity >> 3)) }, 0);
        }else if(v == 11){
            //PSG noise channel
            u8 mod, noiseopts;
            mod = (midi_package.note % 12);
            switch(mod){
            //turn your head 90^ clockwise (:
            case 0:
                case 1:  noiseopts = 0b000; break;
            case 2:
                case 3:  noiseopts = 0b001; break;
            case 4:      noiseopts = 0b010; break;
            case 5:      noiseopts = 0b100; break;
                case 6:  noiseopts = 0b101; break;
            case 7:
                case 8:  noiseopts = 0b110; break;
            case 9:
                case 10: noiseopts = 0b011; break;
            case 11:     noiseopts = 0b111; break;
            //get it? (:
            };
            trackervoicekeys[voicekeyidx] = midi_package.note;
            VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = (g << 4), .data = 0xE0|noiseopts }, 0);
            VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = (g << 4), .data = 0xF0|(15-(midi_package.velocity >> 3)) }, 0);
        }else if(ch3_op){
            //OPN2 ch3 additional frequency
            if(((trackerkeyonmodes[(6*g)+2] >> (2*(ch3_op-1))) & 3) == 3 && genesis[g].opn2.chan[2].op[ch3_op-1].kon){
                //Key off first
                VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x28, .data = GetKeyCommand(0, g, 2, ch3_op) }, 0);
            }
            VgmChipWriteCmd cmd = VGM_getOPN2Frequency(midi_package.note, 0, genesis_clock_opn2); //TODO cents
            cmd.cmd = (g << 4) | 2;
            cmd.addr = 0xAC | (ch3_op % 3);
            VGM_HeadQueue_Enqueue(qhead, cmd, 0);
            VGM_HeadQueue_Enqueue(qhead, (VgmChipWriteCmd){ .cmd = opn2globcmd, .addr = 0x28, .data = GetKeyCommand(1, g, 2, ch3_op) }, 0);
        }
    }
}
