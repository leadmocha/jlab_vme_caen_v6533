///////////////////////////////////////////////////////////////////////////////
//  CAEN V6533N VME Module Library
//
//  History:
//
//  * Wed Mar 04, 2015 Juan Carlos Cornejo <cornejo@jlab.org>
//  - Made to be very generic to using VxWorks or Linux
//  - Made board base address and address space modifiable
//  - Changed function names to match CAEN docs
//  - Set all registers to return/set true values corrected for resolution
//  - Ensure register data type is signed/unsigned as needed
//  - Standardized to CAEN Revision 7 documentation (22/07/2014)
//
//  * Fri Feb 27, 2015 Juan Carlos Cornejo <cornejo@jlab.org>
//  - Updated to work with the intel VME boards
//
//  * XXX May XX, 2014 Yuxiang Zhao <yxzhao@jlab.org>
//  - Initial development for VxWorks

#include <stdio.h>
#include <jvme.h>
#include "caenv6533.h"

// Default jumper address is 3210 == 0x3210 0000 for A32.
// However, in A24 we only need the leading two digits i.e.
// A32: 0x3210 0000
// A24: 0x10 0000
const int caenv6533_default_board_addr    = 0x100000;
const int caenv6533_default_addr_mod      = 0x39; // 0x39 is for A24. 0x09 is for A32
const int ch_base_addr  = 0x80; // Channel base addr

/*
 * Determine the proper function to use depending on the system we are in
 */
int caenBusToLocalAdrs(int adrsSpace, char *busAdrs, char **pLocalAdrs)
{
#ifdef VXWORKS
  return sysBusToLocalAdrs(adrsSpace, busAdrs, pLocalAdrs);
#else
  return vmeBusToLocalAdrs(adrsSpace, busAdrs, pLocalAdrs);
#endif
}

/*
 * Set pointer to specified channel register
 */
int caenChannelAddr(int board_addr, short chan, unsigned short offset,
    unsigned short **reg)
{
  caenBusToLocalAdrs(caenv6533_default_addr_mod,
      (char *)(board_addr+(ch_base_addr*chan)+offset),
      (char **)reg);
}

/*
 * Set pointer to specified board register
 */
int caenBoardAddr(int board_addr, unsigned short offset, unsigned short **reg)
{
  caenBusToLocalAdrs(caenv6533_default_addr_mod,
      (char *)(board_addr+offset),
      (char **)reg);
}

///////////////////////////////////////////////////////////////////////////////
// Getters for board registers
// All board parameters are **READ ONLY**
///////////////////////////////////////////////////////////////////////////////
unsigned short caenv6533GetVMAX(int board_addr)
{
  unsigned short *reg;
  caenBoardAddr(board_addr,0x0050,&reg);
  return *reg; // V
}

unsigned short caenv6533GetIMAX(int board_addr)
{
  unsigned short *reg;
  caenBoardAddr(board_addr,0x0054,&reg);
  return *reg; // uA
}

unsigned short caenv6533GetSTATUS(int board_addr)
{
  unsigned short *reg;
  caenBoardAddr(board_addr,0x0058,&reg);
  return *reg;
}


unsigned short caenv6533GetFWREL(int board_addr)
{
  unsigned short *reg;
  caenBoardAddr(board_addr,0x005C,&reg);
  return *reg;
}


///////////////////////////////////////////////////////////////////////////////
// Getters/setters for channel registers/parameters
// Read Header file for documentation
///////////////////////////////////////////////////////////////////////////////

float caenv6533GetVSET(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0x80,&reg);
  return (*reg)*0.1;  // V
}

void caenv6533SetVSET(int board_addr, short chan, float val)
{
  unsigned short *reg;
  unsigned short rval = val/0.1; // V
  caenChannelAddr(board_addr,chan,0x80,&reg);
  *reg=(rval)&0xffff;
}

float caenv6533GetISET(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0x84,&reg);
  return (*reg)*0.05; // uA
}

void caenv6533SetISET(int board_addr, short chan, float val)
{
  unsigned short *reg;
  unsigned short rval = val/0.05;  // uA
  caenChannelAddr(board_addr,chan,0x84,&reg);
  *reg=(rval)&0xffff;
}

float caenv6533GetVMON(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0x88,&reg);
  return (*reg)*0.1;  // V
}

float caenv6533GetImonH(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan, 0x90, &reg);
  return (*reg)*0.05; // uA
}

unsigned short caenv6533GetPW(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan, 0x90, &reg);
  return *reg;
}

void caenv6533SetPW(int board_addr, short chan, unsigned short val)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan, 0x90, &reg);
  *reg = val;
}

unsigned short caenv6533GetCHSTATUS(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0x94,&reg);
  return *reg;
}

float caenv6533GetTRIP_TIME(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0x98,&reg);
  return (*reg)*0.1;  // s
}

void caenv6533SetTRIP_TIME(int board_addr, short chan, float val)
{
  unsigned short *reg;
  unsigned short rval = val/0.1;  // s
  caenChannelAddr(board_addr,chan,0x98,&reg);
  *reg=(rval)&0xffff; // s
}

float caenv6533GetSVMAX(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0x9C,&reg);
  return (*reg)*0.1;  // V
}

void caenv6533SetSVMAX(int board_addr, short chan, float val)
{
  unsigned short *reg;
  unsigned short rval = val/0.1;  // V
  caenChannelAddr(board_addr,chan,0x9C,&reg);
  *reg=(rval)&0xffff; // V
}


unsigned short caenv6533GetRAMP_DOWN(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xA0,&reg);
  return *reg;        // V/s
}

void caenv6533SetRAMP_DOWN(int board_addr, short chan, unsigned short val)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xA0,&reg);
  *reg=(val)&0xffff; // V/s
}

unsigned short caenv6533GetRAMP_UP(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xA4,&reg);
  return *reg;        // V/s
}

void caenv6533SetRAMP_UP(int board_addr, short chan, unsigned short val)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xA4,&reg);
  *reg=(val)&0xffff; // V/s
}

unsigned short caenv6533GetPWDOWN(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xA8,&reg);
  return *reg;
}

void caenv6533SetPWDOWN(int board_addr, short chan, unsigned short val)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xA8,&reg);
  *reg=(val)&0xffff;
}

unsigned short caenv6533GetPOLARITY(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xAC,&reg);
  return *reg;
}

short caenv6533GetTEMPERATURE(int board_addr, short chan)
{
  short *reg;
  caenChannelAddr(board_addr,chan,0xB0,(unsigned short **)&reg);
  return *reg; // C
}

unsigned short caenv6533GetIMON_RANGE(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xB4,&reg);
  return *reg;
}

void caenv6533SetIMON_RANGE(int board_addr, short chan, unsigned short val)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xB4,&reg);
  *reg=(val)&0xffff;
}


float caenv6533GetImonL(int board_addr, short chan)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,0xB8,&reg);
  return (*reg*0.005); // uA
}

