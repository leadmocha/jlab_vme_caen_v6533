#ifndef CAENV6533_H
#define CAENV6533_H

//
// Reference Documentation
// CAEN Revision 7 (22/07/2014)
//
// See Source File for details

// Define some global default settings
extern const int caenv6533_default_board_addr;
extern const int caenv6533_default_addr_mod;

///////////////////////////////////////////////////////////////////////////////
// Getters for board registers
// All board parameters are **READ ONLY**
///////////////////////////////////////////////////////////////////////////////

// This register can be used to read channel maximum allowed voltage. VMAX is a
// hardware limit, set by the corresponding board front panel trimmer
// [0 : 4100] V (internal resolution: 1 V)
unsigned short caenv6533GetVMAX(int board_addr);

// This register can be used to read channel maximum allowed current IMAX is a
// hardware limit, set by the corresponding board front panel trimmer.
// [0 : 3100] uA (internal resolution: 1 uA)
unsigned short caenv6533GetIMAX(int board_addr);

// Board Status in 16 bit register
// STATUS bit   Meaning
// ----------   ------------------
// 0            Channel 0 ALARM
// 1            Channel 1 ALARM
// 2            Channel 2 ALARM
// 3            Channel 3 ALARM
// 4            Channel 4 ALARM
// 5            Channel 5 ALARM
// 6            Reserved
// 7            Reserved
// 8            Board POWER FAIL
// 9            Board OVER POWER
// 10           Board MAXV UNCALIBRATED
// 11           Board MAXI UNCALIBRATED
// 12..15       Reserved
unsigned short caenv6533GetSTATUS(int board_addr);

// Board redout of microcontroller firmware release
// Bit          Meaning
// -----        -------
// [7:0]        Minor release number
// [15:8]       Major release number
unsigned short caenv6533GetFWREL(int board_addr);


///////////////////////////////////////////////////////////////////////////////
// Getters/setters for channel registers/parameters
///////////////////////////////////////////////////////////////////////////////

// Channel Voltage Set Value [0 : 4000] V (internal resolution: 0.1 V)
// Read/Write
float caenv6533GetVSET(int board_addr, short chan);
void caenv6533SetVSET(int board_addr, short chan, float val);

// Channel Current Set Value [0 : 3100] uA (internal resolution: 0.05 uA)
// Read/Write
float caenv6533GetISET(int board_addr, short chan);
void caenv6533SetISET(int board_addr, short chan, float val);

// Channel Voltage  Readback [0 : 4000] V (internal resolution: 0.1 V)
// Read only
float caenv6533GetVMON(int board_addr, short chan);

// This register can be read to get channel current value when IMON RANGE is
// set to HIGH. [0 : 3100] uA (res 0.05 uA)
// Read only
float caenv6533GetImonH(int board_addr, short chan);

// This is channel ON/OFF control register.
// Possible register values and meaning are:
// 0: OFF
// 1: ON
// Read/Write
unsigned short caenv6533GetPW(int board_addr, short chan);
void caenv6533SetPW(int board_addr, short chan, unsigned short val);

// Channel Status in 16 bit register
// STATUS bit   Meaning
// ----------   -----------------------
// 0            Channel ON
// 1            Channel RAMP UP
// 2            Channel RAMP DOWN
// 3            Channel OVER CURRENT
// 4            Channel OVER VOLTAGE
// 5            Channel UNDER VOLTAGE
// 6            Channel MAXV
// 7            Channel MAXI
// 8            Channel TRIP
// 9            Channel OVER POWER
// 10           Channel OVER TEMPERATURE
// 11           Channel DISABLED
// 12           Channel INTERLOCK
// 13           Channel UNCALIBRATED
// 14..15       Reserved
//
// Read Only
unsigned short caenv6533GetCHSTATUS(int board_addr, short chan);

// Channel trip time [0 : 1000] s (internal resolution: 0.1 s)
// Note: 1000 s = Infinite.
// Read/Write
float caenv6533GetTRIP_TIME(int board_addr, short chan);
void caenv6533SetTRIP_TIME(int board_addr, short chan, float val);

// Channel Software Max Volage [0 : 4000] V (internal resolution: 0.1 V)
// Read/Write
float caenv6533GetSVMAX(int board_addr, short chan);
void caenv6533SetSVMAX(int board_addr, short chan, float val);

// Channel Ramp Down rate [0 : 500] V/s (internal resolution: 1 V/s)
// Read/Write
unsigned short caenv6533GetRAMP_DOWN(int board_addr, short chan);
void caenv6533SetRAMP_DOWN(int board_addr, short chan, unsigned short val);

// Channel Ramp Up rate [0 : 500] V/s (internal resolution: 1 V/s)
// Read/Write
unsigned short caenv6533GetRAMP_UP(int board_addr, short chan);
void caenv6533SetRAMP_UP(int board_addr, short chan, unsigned short val);

// This is channel Power Down Mode control register.
// Possible register values and meaning are:
// 0: KILL
// 1:RAMP
// Read/Write
unsigned short caenv6533GetPWDOWN(int board_addr, short chan);
void caenv6533SetPWDOWN(int board_addr, short chan, unsigned short val);

// This register reads channel POLARITY.
// Possible values are:
// 0: NEGATIVE
// 1: POSITIVE
// Read only
unsigned short caenv6533GetPOLARITY(int board_addr, short chan);

// Channel current temperature [-40 : +125] C (internal resolution: 1 C)
// Read only
short caenv6533GetTEMPERATURE(int board_addr, short chan);  // C [-40 : +125]

// This is channel Imon Range control register.
// Possible register values and meaning are:
// 0: Range High
// 1: Range Low
// Read/Write
unsigned short caenv6533GetIMON_RANGE(int board_addr, short chan);
void caenv6533SetIMON_RANGE(int board_addr, short chan, unsigned short val);

// This register can be read to get channel current value
// when IMON_RANGE is set “LOW” [0 : 300] uA (internal resolution: 0.005 uA)
float caenv6533GetImonL(int board_addr, short chan);

#endif // CAENV6533_H
