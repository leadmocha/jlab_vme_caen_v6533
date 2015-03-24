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
const int caenv6533_default_board_addr    = 0x32100000;
const int caenv6533_default_addr_mod      = 0x39; // 0x39 is for A24. 0x9 is for A32
const int ch_base_addr  = 0x80; // Channel base addr

/*
 * Determine the proper function to use depending on the system we are in
 */
int caenBusToLocalAdrs(int adrsSpace, char *busAdrs, char **pLocalAdrs)
{
  int stat;
#ifdef VXWORKS
  stat = sysBusToLocalAdrs(adrsSpace, busAdrs, pLocalAdrs);
#else
  stat = vmeBusToLocalAdrs(adrsSpace, busAdrs, pLocalAdrs);
#endif
  /*if( stat != 0 ) {
    printf("ERROR: caenBusToLocalAdrs res=%d. Details: ",stat);
  } else {
    printf("caenBusToLocalAdrs ");
  }
  printf("mod=0x%.8x, addr=0x%.8x, laddr=0x%.8x\n",adrsSpace,busAdrs,pLocalAdrs);
  */
  return stat;
}

/*
 * Set pointer to specified channel register
 */
int caenChannelAddr(int board_addr, unsigned short chan, short offset,
    unsigned short **reg)
{
  return caenBusToLocalAdrs(caenv6533_default_addr_mod,
      (char *)(board_addr+(ch_base_addr*chan)+offset),
      (char **)reg);
}

/*
 * Set pointer to specified board register
 */
int caenBoardAddr(int board_addr, unsigned short offset, unsigned short **reg)
{
  return caenBusToLocalAdrs(caenv6533_default_addr_mod,
      (char *)(board_addr+offset),
      (char **)reg);
}

/*
 * Read a specified channel with specified offset
 */
void caenWriteChannel(int board_addr, unsigned short chan,
    unsigned short offset, unsigned short val)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,offset,&reg);
#ifdef VXWORKS
  *reg = val&0xFF;
#else
  vmeWrite16(reg,val);
#endif
}

/*
 * Read a specified channel with specified offset
 */
unsigned short caenReadChannel(int board_addr, unsigned short chan,
    unsigned short offset)
{
  unsigned short *reg;
  caenChannelAddr(board_addr,chan,offset,&reg);
#ifdef VXWORKS
  return *reg&0xFFFF;
#else
  return vmeRead16(reg);
#endif
}

/*
 * Read a value from the board.
 * Ensure that value is read in proper order regardless of OS
 */
unsigned short caenReadBoard(int board_addr, unsigned short offset)
{
  unsigned short *reg;
  caenBoardAddr(board_addr,offset,&reg);
#ifdef VXWORKS
  return *reg&0xFFFF;
#else
  return vmeRead16(reg);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Getters for board registers
// All board parameters are **READ ONLY**
///////////////////////////////////////////////////////////////////////////////
short caenv6533GetVMAX(int board_addr)
{
  return caenReadBoard(board_addr,0x50); // in V
}

short caenv6533GetIMAX(int board_addr)
{
  return caenReadBoard(board_addr,0x54); // in uA
}

short caenv6533GetSTATUS(int board_addr)
{
  return caenReadBoard(board_addr,0x58);
}


short caenv6533GetFWREL(int board_addr)
{
  return caenReadBoard(board_addr,0x5C);
}


///////////////////////////////////////////////////////////////////////////////
// Getters/setters for channel registers/parameters
// Read Header file for documentation
///////////////////////////////////////////////////////////////////////////////

float caenv6533GetVSET(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0x80)*0.1; // V
}

void caenv6533SetVSET(int board_addr, short chan, float val)
{
  caenWriteChannel(board_addr,chan,0x80,val/0.1); // V
}

float caenv6533GetISET(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0x84)*0.05;  // uA
}

void caenv6533SetISET(int board_addr, short chan, float val)
{
  caenWriteChannel(board_addr,chan,0x84,val/0.05); // uA
}

float caenv6533GetVMON(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0x88)*0.1; // V
}

float caenv6533GetImonH(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan, 0x90)*0.05; // uA
}

short caenv6533GetPW(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan, 0x90);
}

void caenv6533SetPW(int board_addr, short chan, short val)
{
  caenWriteChannel(board_addr,chan, 0x90, val);
}

short caenv6533GetCHSTATUS(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0x94);
}

float caenv6533GetTRIP_TIME(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0x98)*0.1; // s
}

void caenv6533SetTRIP_TIME(int board_addr, short chan, float val)
{
  caenWriteChannel(board_addr,chan,0x98,val/0.1); // s
}

float caenv6533GetSVMAX(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0x9C)*0.1; // V
}

void caenv6533SetSVMAX(int board_addr, short chan, float val)
{
  caenWriteChannel(board_addr,chan,0x9C,val/0.1); // V
}


short caenv6533GetRAMP_DOWN(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0xA0); // V/s
}

void caenv6533SetRAMP_DOWN(int board_addr, short chan, short val)
{
  caenWriteChannel(board_addr,chan,0xA0,val); // V/s
}

short caenv6533GetRAMP_UP(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0xA4); // V/s
}

void caenv6533SetRAMP_UP(int board_addr, short chan, short val)
{
  caenWriteChannel(board_addr,chan,0xA4,val); // V/s
}

short caenv6533GetPWDOWN(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0xA8);
}

void caenv6533SetPWDOWN(int board_addr, short chan, short val)
{
  caenWriteChannel(board_addr,chan,0xA8,val);
}

short caenv6533GetPOLARITY(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0xAC);
}

short caenv6533GetTEMPERATURE(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0xB0); // C
}

short caenv6533GetIMON_RANGE(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0xB4);
}

void caenv6533SetIMON_RANGE(int board_addr, short chan, short val)
{
  caenWriteChannel(board_addr,chan,0xB4,val);
}


float caenv6533GetImonL(int board_addr, short chan)
{
  return caenReadChannel(board_addr,chan,0xB8)*0.005; // uA
}

///////////////////////////////////////////////////////////////////////////////
// Getters for board configuration
// All board configurations are **READ ONLY**
///////////////////////////////////////////////////////////////////////////////

short caenv6533GetCHNUM(int board_addr)
{
  return caenReadBoard(board_addr,0x8100);
}


void caenv6533GetDESCR(int board_addr, char *desc)
{
  unsigned int temp;
  int i;
  for(i = 0; i < 10; i++ ) {
    temp = caenReadBoard(board_addr,0x8102 + (0x2*i));
    desc[2*i] = temp & 0xFF;
    desc[2*i+1] = temp >> 8;
  }
}

