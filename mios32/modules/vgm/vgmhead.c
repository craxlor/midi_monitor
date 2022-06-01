/*
 * VGM Data and Playback Driver: VGM Playback Head
 *
 * ==========================================================================
 *
 *  Copyright (C) 2016 Sauraen (sauraen@gmail.com)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */


#include "vgmhead.h"
#include "vgmqueue.h"
#include "vgmram.h"
#include "vgmstream.h"
#include <genesis.h>
#include "vgm_heap2.h"
#include "vgmtuning.h"

VgmHead* vgm_heads[VGM_HEAD_MAXNUM];
u32 vgm_numheads;

void VGM_Head_Init(){
    vgm_numheads = 0;
}

VgmHead* VGM_Head_Create(VgmSource* source, u32 freqmult, u32 tempomult, u32 tloffs){
    if(source == NULL) return NULL;
    VgmHead* head = vgmh2_malloc(sizeof(VgmHead));
    head->playing = 0;
    head->source = source;
    head->ticks = 0; //will get changed at restart
    head->srcaddr = 0;
    u8 i;
    u16 mutes = source->mutes;
    for(i=0; i<12; ++i){
        head->channel[i].ALL = 0;
        head->channel[i].mute = mutes & 1;
        mutes >>= 1;
        if(i>=1 && i<=6){
            head->channel[i].map_voice = i-1;
        }else if(i>=8 && i<=0xA){
            head->channel[i].map_voice = i-8;
        }
    }
    head->opn2mult = (((source->opn2clock << 9) / (genesis_clock_opn2 >> 3)) * freqmult) >> 12; 
    // ((7670454 << 8) / 500000); //0x1000;
    head->psgmult = (((source->psgclock << 10) / (genesis_clock_psg >> 2)) * freqmult) >> 12;
    // 0x1000;
    head->psgfreq0to1 = source->psgfreq0to1;
    head->tempomult = tempomult;
    head->tloffs = tloffs;
    head->iswait = 1;
    head->iswrite = 0;
    head->isdone = 0;
    if(source->type == VGM_SOURCE_TYPE_RAM){
        head->data = VGM_HeadRAM_Create(source);
    }else if(source->type == VGM_SOURCE_TYPE_STREAM){
        head->data = VGM_HeadStream_Create(source);
    }else if(source->type == VGM_SOURCE_TYPE_QUEUE){
        head->data = VGM_HeadQueue_Create(source);
    }
    MIOS32_IRQ_Disable();
    if(vgm_numheads == VGM_HEAD_MAXNUM){
        MIOS32_IRQ_Enable();
        vgmh2_free(head);
        return NULL;
    }
    vgm_heads[vgm_numheads] = head;
    ++vgm_numheads;
    MIOS32_IRQ_Enable();
    return head;
}
s32 VGM_Head_Delete(VgmHead* head){
    if(head == NULL) return -1;
    s32 ret = -1;
    MIOS32_IRQ_Disable();
    u8 i;
    for(i=0; i<vgm_numheads; ++i){
        if(vgm_heads[i] == head){
            for(; i<vgm_numheads-1; ++i){
                vgm_heads[i] = vgm_heads[i+1];
            }
            vgm_heads[i] = NULL;
            --vgm_numheads;
            ret = 0;
            break;
        }
    }
    MIOS32_IRQ_Enable();
    if(head->source->type == VGM_SOURCE_TYPE_RAM){
        VGM_HeadRAM_Delete(head->data);
    }else if(head->source->type == VGM_SOURCE_TYPE_STREAM){
        VGM_HeadStream_Delete(head->data);
    }else if(head->source->type == VGM_SOURCE_TYPE_QUEUE){
        VGM_HeadQueue_Delete(head->data);
    }
    vgmh2_free(head);
    return ret;
}

void VGM_Head_Restart(VgmHead* head, u32 vgm_time){
    if(head == NULL) return;
    head->ticks = vgm_time;
    head->isdone = 0;
    if(head->source->type == VGM_SOURCE_TYPE_RAM){
        VGM_HeadRAM_Restart(head);
    }else if(head->source->type == VGM_SOURCE_TYPE_STREAM){
        VGM_HeadStream_Restart(head);
    }else if(head->source->type == VGM_SOURCE_TYPE_QUEUE){
        VGM_HeadQueue_Restart(head);
    }
}
void VGM_Head_cmdNext(VgmHead* head, u32 vgm_time){
    if(head == NULL) return;
    if(head->source->type == VGM_SOURCE_TYPE_RAM){
        VGM_HeadRAM_cmdNext(head, vgm_time);
    }else if(head->source->type == VGM_SOURCE_TYPE_STREAM){
        VGM_HeadStream_cmdNext(head, vgm_time);
    }else if(head->source->type == VGM_SOURCE_TYPE_QUEUE){
        VGM_HeadQueue_cmdNext(head, vgm_time);
    }
}

#define CHECK_NODATA(c) do{ if(head->channel[(c)].nodata) { cmd->cmd = 0xFF; return; }} while(0)
#define CHECK_MUTE(c) do{ if(head->channel[(c)].mute) { cmd->cmd = 0xFF; return; }} while(0)

void VGM_Head_doMapping(VgmHead* head, VgmChipWriteCmd* cmd){
    u8 chan = 0xFF, board = 0xFF;
    u8 addrhi = (cmd->cmd & 0x01);
    u8 type = (cmd->cmd & 0x0E);
    u8 uppertype = cmd->cmd >> 4;
    if(uppertype != 0 && uppertype != 5){
        cmd->cmd = 0xFF; return;
    }
    if(type == 0){
        //PSG write command
        if(cmd->data & 0x80){
            chan = ((cmd->data & 0x60) >> 5);
            head->psglastchannel = chan;
            chan += 8;
            CHECK_NODATA(chan);
            if(cmd->data & 0x10){
                //It's a volume write, we should mute
                CHECK_MUTE(chan);
            }
            board = head->channel[chan].map_chip;
            if(chan != 0xB){
                chan = head->channel[chan].map_voice;
                cmd->data = (cmd->data & 0x9F) | (chan << 5);
            }
        }else{
            chan = head->psglastchannel + 8;
            CHECK_NODATA(chan);
            board = head->channel[chan].map_chip;
        }
    }else if(type == 2){
        //OPN2 write command
        if(cmd->addr == 0x28){
            //Key on
            chan = cmd->data & 0x07;
            if((chan & 3) == 3){ cmd->cmd = 0xFF; return; }
            if(chan < 0x03) chan += 1; //Move channels 0,1,2 up to 1,2,3
            CHECK_NODATA(chan);
            CHECK_MUTE(chan);
            board = head->channel[chan].map_chip;
            chan = head->channel[chan].map_voice;
            if(chan >= 0x03) chan += 1; //Move 3,4,5 up to 4,5,6
            cmd->data = (cmd->data & 0xF8) | chan;
        }else if(cmd->addr == 0x2A || cmd->addr == 0x2B){
            //DAC write
            CHECK_NODATA(7);
            if(cmd->addr == 0x2A) CHECK_MUTE(7); //Only mute DAC data, not DAC enable!
            board = head->channel[7].map_chip;
        }else if((cmd->addr <= 0xAE && cmd->addr >= 0xA8) || (cmd->addr <= 0x27 && cmd->addr >= 0x24)){
            //Ch3 frequency write or options/timers write
            CHECK_NODATA(3);
            board = head->channel[3].map_chip;
        }else if(cmd->addr <= 0x2F || cmd->addr >= 0xB8){
            //Other chip write
            CHECK_NODATA(0);
            board = head->channel[0].map_chip;
        }else{
            //Operator or channel/voice write
            chan = cmd->addr & 0x03;
            if(chan == 3){ cmd->cmd = 0xFF; return; }
            chan += addrhi + (addrhi << 1) + 1; //Now 1,2,3,4,5,6
            CHECK_NODATA(chan);
            board = head->channel[chan].map_chip;
            chan = head->channel[chan].map_voice;
            chan += (chan >= 3); //Now 0,1,2,4,5,6
            addrhi = (chan & 0x04) >> 2;
            chan &= 0x03;
            cmd->addr = (cmd->addr & 0xFC) | chan;
        }
    }else{
        cmd->cmd = 0xFF; return;
    }
    cmd->cmd = (board << 4) | type | addrhi;
}

void VGM_Head_doTransformations(VgmHead* head, VgmChipWriteCmd* cmd){
    if(cmd->cmd == 0x50){
        //PSG write
        if(!(cmd->data & 0x80)) return; //Freq second byte
        if(cmd->data & 0x10){
            //Attenuation
            u8 tl = cmd->data & 0x0F;
            if(tl == 0x0F) return; //Don't change off commands
            tl = tl + (head->tloff[3] >> 3);
            if(tl > 0x0E) tl = 0x0E;
            cmd->data = (cmd->data & 0xF0) | tl;
        }else{
            //Not attenuation
            if(cmd->data < 0xE0){
                //Not noise--i.e. frequency
                VGM_fixPSGFrequency(cmd, head->psgmult, head->psgfreq0to1);
            }
        }
    }else if((cmd->cmd & 0xFE) == 0x52){
        //OPN2 write
        if((cmd->addr & 0xF4) == 0xA4){
            //Frequency MSB write
            VGM_fixOPN2Frequency(cmd, head->opn2mult);
        }else if((cmd->addr & 0xF0) == 0x40){
            u8 tl = cmd->data & 0x7F;
            u8 op = ((cmd->addr & 0x08) >> 3) | ((cmd->addr & 0x04) >> 1);
            tl = tl + head->tloff[op];
            if(tl > 0x7F) tl = 0x7F;
            cmd->data = (cmd->data & 0x80) | tl;
        }
    }
}

void VGM_Head_setWritecmd(VgmHead* head, VgmChipWriteCmd cmd){
    head->writecmd.all = cmd.all;
    head->iswrite = VGM_Cmd_IsWrite(cmd);
    u32 wait = VGM_Cmd_GetWaitValue(cmd);
    head->iswait = (wait > 0);
    head->ticks += wait;
}



