#ifndef LNCONSTANTS_H
#define LNCONSTANTS_H

#include <QString>
#include "libPr3_global.h"
#include <QtCore>
class LIBPR3SHARED_EXPORT LnConstants
{
 Q_GADGET
 Q_ENUMS(LoconetOpcodes)
public:
    LnConstants();
    /* Note that the values in this class have been taken from the llnmom C program of
    * Ron W. Auld, which included some work of John Kabat.  The symbol names
    * are copied from the loconet.h file, CVS revision 1.1.1.1, program release 0.3.0  Those
    * parts are (C) Copyright 2001 Ron W. Auld, and are used with direct
    * permission of the copyright holder.
    * <P>
    * Most major comment blocks here are quotes from the Digitrax Loconet(r)
    * OPCODE SUMMARY: found in the Loconet(r) Personal Edition 1.
    * <P>
    * Al Silverstein provided the reverse-engineering effort for the
    * OPC_MULTI_SENSE message.
    * <P>
    * Some of the message formats used in this class are Copyright Digitrax, Inc.
    * and used with permission as part of the JMRI project.  That permission
    * does not extend to uses in other software products.  If you wish to
    * use this code, algorithm or these message formats outside of JMRI, please
    * contact Digitrax Inc for separate permission.
    */

   /* various bit masks */
   static  int DIRF_DIR         ; //0x20;  /* direction bit    */
   static int DIRF_F0          ; //0x10;  /* Function 0 bit   */
   static int DIRF_F4          ; //0x08;  /* Function 1 bit   */
   static int DIRF_F3          ; //0x04;  /* Function 2 bit   */
   static int DIRF_F2          ; //0x02;  /* Function 3 bit   */
   static int DIRF_F1          ; //0x01;  /* Function 4 bit   */
   static int SND_F8           ; //0x08;  /* Sound 4/Function 8 bit */
   static int SND_F7           ; //0x04;  /* Sound 3/Function 7 bit */
   static int SND_F6           ; //0x02;  /* Sound 2/Function 6 bit */
   static int SND_F5           ; //0x01;  /* Sound 1/Function 5 bit */

   //static int OPC_SW_ACK_CLOSED; //0x20;  /* command switch closed/open bit   */
#define OPC_SW_ACK_CLOSED 0x20
   //static int OPC_SW_ACK_OUTPUT; //0x10;  /* command switch output on/off bit */
#define OPC_SW_ACK_OUTPUT 0x10

   static int OPC_INPUT_REP_CB ; //0x40;  /* control bit, reserved otherwise      */
   static int OPC_INPUT_REP_SW ; //0x20;  /* input is switch input, aux otherwise */
   static int OPC_INPUT_REP_HI ; //0x10;  /* input is HI, LO otherwise            */

   static int OPC_SW_REP_SW    ; //0x20;  /* switch input, aux input otherwise    */
   static int OPC_SW_REP_HI    ; //0x10;  /* input is HI, LO otherwise            */
   //static int OPC_SW_REP_CLOSED; //0x20;  /* 'Closed' line is ON, OFF otherwise   */
#define OPC_SW_REP_CLOSED 0x20
   //static int OPC_SW_REP_THROWN; //0x10;  /* 'Thrown' line is ON, OFF otherwise   */
#define OPC_SW_REP_THROWN 0x10
   static int OPC_SW_REP_INPUTS; //0x40;  /* sensor inputs, outputs otherwise     */

   static int OPC_SW_REQ_DIR   ; //0x20;  /* switch direction - closed/thrown     */
   static int OPC_SW_REQ_OUT   ; //0x10;  /* output On/Off                        */

   static int OPC_LOCO_SPD_ESTOP; //0x01; /* emergency stop command               */

#define OPC_MULTI_SENSE_MSG     0x60 // byte 1
#define OPC_MULTI_SENSE_PRESENT 0x20 // MSG field: transponder seen
#define OPC_MULTI_SENSE_ABSENT  0x00 // MSG field: transponder lost
#define OPC_MULTI_SENSE_POWER   0x60 // MSG field: Power message

   /* Slot Status byte definitions and macros */
   /***********************************************************************************
   *   D7-SL_SPURGE    ; 1=SLOT purge en,                                             *
   *                   ; ALSO adrSEL (INTERNAL use only) (not seen on NET!)           *
   *                                                                                  *
   *   D6-SL_CONUP     ; CONDN/CONUP: bit encoding-Control double linked Consist List *
   *                   ;    11=LOGICAL MID CONSIST , Linked up AND down               *
   *                   ;    10=LOGICAL CONSIST TOP, Only linked downwards             *
   *                   ;    01=LOGICAL CONSIST SUB-MEMBER, Only linked upwards        *
   *                   ;    00=FREE locomotive, no CONSIST indirection/linking        *
   *                   ; ALLOWS "CONSISTS of CONSISTS". Uplinked means that           *
   *                   ; Slot SPD number is now SLOT adr of SPD/DIR and STATUS        *
   *                   ; of consist. i.e. is ;an Indirect pointer. This Slot          *
   *                   ; has same BUSY/ACTIVE bits as TOP of Consist. TOP is          *
   *                   ; loco with SPD/DIR for whole consist. (top of list).          *
   *                   ; BUSY/ACTIVE: bit encoding for SLOT activity                  *
   *                                                                                  *
   *   D5-SL_BUSY      ; 11=IN_USE loco adr in SLOT -REFRESHED                        *
   *                                                                                  *
   *   D4-SL_ACTIVE    ; 10=IDLE loco adr in SLOT -NOT refreshed                      *
   *                   ; 01=COMMON loco adr IN SLOT -refreshed                        *
   *                   ; 00=FREE SLOT, no valid DATA -not refreshed                   *
   *                                                                                  *
   *   D3-SL_CONDN     ; shows other SLOT Consist linked INTO this slot, see SL_CONUP *
   *                                                                                  *
   *   D2-SL_SPDEX     ; 3 BITS for Decoder TYPE encoding for this SLOT               *
   *                                                                                  *
   *   D1-SL_SPD14     ; 011=send 128 speed mode packets                              *
   *                                                                                  *
   *   D0-SL_SPD28     ; 010=14 step MODE                                             *
   *                   ; 001=28 step. Generate Trinary packets for this               *
   *                   ;              Mobile ADR                                      *
   *                   ; 000=28 step. 3 BYTE PKT regular mode                         *
   *                   ; 111=128 Step decoder, Allow Advanced DCC consisting          *
   *                   ; 100=28 Step decoder ,Allow Advanced DCC consisting           *
   ***********************************************************************************/

//   static int STAT1_SL_SPURGE  ; //0x80;  /* internal use only, not seen on net */
//   static int STAT1_SL_CONUP   ; //0x40;  /* consist status                     */
//   static int STAT1_SL_BUSY    ; //0x20;  /* used with STAT1_SL_ACTIVE,         */
//   static int STAT1_SL_ACTIVE  ; //0x10;  /*                                    */
//   static int STAT1_SL_CONDN   ; //0x08;  /*                                    */
//   static int STAT1_SL_SPDEX   ; //0x04;  /*                                    */
//   static int STAT1_SL_SPD14   ; //0x02;  /*                                    */
//   static int STAT1_SL_SPD28   ; //0x01;  /*                                    */
//   static int STAT2_SL_SUPPRESS; //0x01;  /* 1 = Adv. Consisting supressed      */
//   static int STAT2_SL_NOT_ID  ; //0x04;  /* 1 = ID1/ID2 is not ID usage        */
//   static int STAT2_SL_NOTENCOD; //0x08;  /* 1 = ID1/ID2 is not encoded alias   */
//   static int STAT2_ALIAS_MASK ; //STAT2_SL_NOTENCOD | STAT2_SL_NOT_ID;
//   static int STAT2_ID_IS_ALIAS; //STAT2_SL_NOT_ID;int LnConstants::STAT1_SL_SPURGE   = 0x80;  /* internal use only, not seen on net */
   enum STAT
   {
   STAT1_SL_CONUP    = 0x40,  /* consist status                     */
   STAT1_SL_BUSY     = 0x20,  /* used with STAT1_SL_ACTIVE,         */
   STAT1_SL_ACTIVE   = 0x10,  /*                                    */
   STAT1_SL_CONDN    = 0x08,  /*                                    */
   STAT1_SL_SPDEX    = 0x04,  /*                                    */
   STAT1_SL_SPD14    = 0x02,  /*                                    */
   STAT1_SL_SPD28    = 0x01,  /*                                    */
   STAT2_SL_SUPPRESS = 0x01,  /* 1 = Adv. Consisting supressed      */
   STAT2_SL_NOT_ID   = 0x04,  /* 1 = ID1/ID2 is not ID usage        */
   STAT2_SL_NOTENCOD = 0x08,  /* 1 = ID1/ID2 is not encoded alias   */
   STAT2_ALIAS_MASK  = STAT2_SL_NOTENCOD | STAT2_SL_NOT_ID,
   STAT2_ID_IS_ALIAS = STAT2_SL_NOT_ID
   };

   /* mask and values for consist determination */
   static int CONSIST_MASK     ; //STAT1_SL_CONDN | STAT1_SL_CONUP;
//   static const int CONSIST_MID      ; //STAT1_SL_CONDN | STAT1_SL_CONUP;
//   static const int CONSIST_TOP      ; //STAT1_SL_CONDN;
//   static const int CONSIST_SUB      ; //STAT1_SL_CONUP;
//   static const int CONSIST_NO       ; //0;
   static QString CONSIST_STAT(int s);
   enum MASK
   {
    CONSIST_MID       = STAT1_SL_CONDN | STAT1_SL_CONUP,
    CONSIST_TOP       = STAT1_SL_CONDN,
    CONSIST_SUB       = STAT1_SL_CONUP,
    CONSIST_NO        = 0

   };

   /* mask and values for locomotive use determination */
   static int LOCOSTAT_MASK    ; //STAT1_SL_BUSY  | STAT1_SL_ACTIVE;
//   static const int LOCO_IN_USE      ; //STAT1_SL_BUSY  | STAT1_SL_ACTIVE;
//   static const int LOCO_IDLE        ; //STAT1_SL_BUSY;
//   static const int LOCO_COMMON      ; //STAT1_SL_ACTIVE;
//   static const int LOCO_FREE        ; //0;
   enum LOCOSTAT
   {
    LOCO_IN_USE       = STAT1_SL_BUSY  | STAT1_SL_ACTIVE,
    LOCO_IDLE         = STAT1_SL_BUSY,
    LOCO_COMMON       = STAT1_SL_ACTIVE,
    LOCO_FREE         = 0
   };

   static QString LOCO_STAT(int s);

   /* mask and values for decoder type encoding for this slot */
   static int DEC_MODE_MASK    ; //STAT1_SL_SPDEX | STAT1_SL_SPD14 | STAT1_SL_SPD28;
   /* Advanced consisting allowed for the next two */
//   static int DEC_MODE_128A    ; //STAT1_SL_SPDEX | STAT1_SL_SPD14 | STAT1_SL_SPD28;
//   static int DEC_MODE_28A     ; //STAT1_SL_SPDEX ;
//   /* normal modes */
//   static int DEC_MODE_128     ; //STAT1_SL_SPD14 | STAT1_SL_SPD28;
//   static int DEC_MODE_14      ; //STAT1_SL_SPD14;
//   static int DEC_MODE_28TRI   ; //STAT1_SL_SPD28;
//   static int DEC_MODE_28      ; //0;
   enum DECMODEMASK
   {
    DEC_MODE_128A     = STAT1_SL_SPDEX | STAT1_SL_SPD14 | STAT1_SL_SPD28,
    DEC_MODE_28A      = STAT1_SL_SPDEX,
       /* normal modes */
    DEC_MODE_128      = STAT1_SL_SPD14 | STAT1_SL_SPD28,
    DEC_MODE_14       = STAT1_SL_SPD14,
    DEC_MODE_28TRI    = STAT1_SL_SPD28,
    DEC_MODE_28       = 0
   };

   static QString DEC_MODE(int s);

   /* values for track status encoding for this slot */
   enum SlotTrackStatus
   {
   GTRK_PROG_BUSY   = 0x08,      /* 1; //programming track in this master is Busy         */
   GTRK_MLOK1       = 0x04,      /* 0; //Master is DT200, 1=Master implements LocoNet 1.1 */
   GTRK_IDLE        = 0x02,      /* 0=TRACK is PAUSED, B'cast EMERG STOP.                */
   GTRK_POWER       = 0x01,      /* 1=DCC packets are ON in MASTER, Global POWER up      */
   FC_SLOT          = 0x7b,      /* Fast clock is in this slot                           */
   PRG_SLOT         = 0x7c,      /* This slot communicates with the programming track    */
   CFG_SLOT         = 0x7f      /* This slot holds configuration bits                   */
   };

   /* values and macros to decode programming messages */
   static int PCMD_RW          ; //0x40;      /* 1; //write, 0; //read                                  */
   static int PCMD_BYTE_MODE   ; //0x20;      /* 1; //byte operation, 0; //bit operation (if possible)  */
   static int PCMD_TY1         ; //0x10;      /* TY1 Programming type select bit                      */
   static int PCMD_TY0         ; //0x08;      /* TY0 Programming type select bit                      */
   static int PCMD_OPS_MODE    ; //0x04;      /* 1; //Ops mode, 0; //Service Mode                       */
   static int PCMD_RSVRD1      ; //0x02;      /* reserved                                             */
   static int PCMD_RSVRD0      ; //0x01;      /* reserved                                             */

   /* programming mode mask */
   static int PCMD_MODE_MASK   ; //PCMD_BYTE_MODE | PCMD_OPS_MODE | PCMD_TY1 | PCMD_TY0;

   /*
    *  programming modes
    */
   /* Paged mode  byte R/W on Service Track */
   static int PAGED_ON_SRVC_TRK      ; //PCMD_BYTE_MODE;

   /* Direct mode byte R/W on Service Track */
   static int DIR_BYTE_ON_SRVC_TRK   ; //PCMD_BYTE_MODE | PCMD_TY0;

   /* Direct mode bit  R/W on Service Track */
   static int DIR_BIT_ON_SRVC_TRK    ; //PCMD_TY0;

   /* Physical Register byte R/W on Service Track */
   static int REG_BYTE_RW_ON_SRVC_TRK; //PCMD_TY1;

   /* Service Track Reserved function */
   static int SRVC_TRK_RESERVED      ; //PCMD_TY1 | PCMD_TY0;

   /* Ops mode byte program - no feedback */
   static int OPS_BYTE_NO_FEEDBACK   ; //PCMD_BYTE_MODE | PCMD_OPS_MODE;

   /* Ops mode byte program - feedback */
   static int OPS_BYTE_FEEDBACK      ; //OPS_BYTE_NO_FEEDBACK | PCMD_TY0;

   /* Ops mode bit program - no feedback */
   static int OPS_BIT_NO_FEEDBACK    ; //PCMD_OPS_MODE;

   /* Ops mode bit program - feedback */
   static int OPS_BIT_FEEDBACK       ; //OPS_BIT_NO_FEEDBACK | PCMD_TY0;

   /* Programmer Status error flags */
   static int PSTAT_USER_ABORTED ; //0x08;    /* User aborted this command */
   static int PSTAT_READ_FAIL    ; //0x04;    /* Failed to detect Read Compare Acknowledge from decoder */
   static int PSTAT_WRITE_FAIL   ; //0x02;    /* No Write acknowledge from decoder                      */
   static int PSTAT_NO_DECODER   ; //0x01;    /* Service mode programming track empty                   */

   /* bit masks for CVH */
   static int CVH_CV8_CV9        ; //0x30;    /* mask for CV# bits 8 and 9    */
   static int CVH_CV7            ; //0x01;    /* mask for CV# bit 7           */
   static int CVH_D7             ; //0x02;    /* MSbit for data value         */

   // The following two are commented out pending some decisions as to (a) whether
   // they belong here or in the parser and (b) understanding what they say about
   // a data format; note use of a pointer dereference

   /* build data byte from programmer message */
   //static int PROG_DATA(ptr)      (((ptr->cvh & CVH_D7) << 6) | (ptr->data7 & 0x7f))

   /* build CV # from programmer message */
   //static int PROG_CV_NUM(ptr)    (((((ptr->cvh & CVH_CV8_CV9) >> 3) | (ptr->cvh & CVH_CV7)) * 128) + (ptr->cvl & 0x7f))


   // The struct typedefs here have been moved to individual classes as follows:
   // Locomotive Address Message:   	LocoAdrMsg
   // Switch with/without Acknowledge:	SwitchAckMsg, SwitchReqMsg
   // Slot data request:				SlotReqMsg
   // Move/Link Slot Message:			SlotMoveMsg, slotLinkMsg
   // Consist Function Message:		ConsistFuncMsg
   // Write slot status message:		SlotStatusMsg
   // Long ACK message:				LongAckMsg
   // Sensor input report:				InputRepMsg
   // Turnout sensor state report:		SwRepMsg
   // Request Switch function:			SwReqMsg
   // Set slot sound functions:		LocoSndMsg
   // Set slot direction and F0-F4 functions:	locoDirfMsg
   // Set slot speed functions:		LocoSpdMsg
   // Read/Write Slot data messages:	RwSlotDataMsg
   // Fast Clock Message:				fastClockMsg
   // Programmer Task Message (used in Start and Final Reply, both):	progTaskMsg;
   // Peer-peer transfer message:		PeerXferMsg;
   // send packet immediate message:	SendPktMsg;


   /* loconet opcodes */
   enum LoconetOpcodes{
   OPC_GPBUSY        = 0x81,
   OPC_GPOFF         = 0x82,
   OPC_GPON          = 0x83,
   OPC_IDLE          = 0x85,
   OPC_LOCO_SPD      = 0xa0,
   OPC_LOCO_DIRF     = 0xa1,
   OPC_LOCO_SND      = 0xa2,
   OPC_SW_REQ        = 0xb0,
   OPC_SW_REP        = 0xb1,
   OPC_INPUT_REP     = 0xb2,
   OPC_UNKNOWN       = 0xb3,
   OPC_LONG_ACK      = 0xb4,
   OPC_SLOT_STAT1    = 0xb5,
   OPC_CONSIST_FUNC  = 0xb6,
   OPC_UNLINK_SLOTS  = 0xb8,
   OPC_LINK_SLOTS    = 0xb9,
   OPC_MOVE_SLOTS    = 0xba,
   OPC_RQ_SL_DATA    = 0xbb,
   OPC_SW_STATE      = 0xbc,
   OPC_SW_ACK        = 0xbd,
   OPC_LOCO_ADR      = 0xbf,
   OPC_MULTI_SENSE   = 0xd0, // =  Undocumented name
   OPC_PANEL_RESPONSE= 0xd7, // =  Undocumented name
   OPC_PANEL_QUERY   = 0xdf, // =  Undocumented name
   OPC_LISSY_UPDATE  = 0xe4, // =  Undocumented name
   OPC_PEER_XFER     = 0xe5,
   OPC_ALM_READ      = 0xe6, // =  Undocumented name
   OPC_SL_RD_DATA    = 0xe7,
   OPC_IMM_PACKET    = 0xed,
   OPC_IMM_PACKET_2  = 0xee,
   OPC_WR_SL_DATA    = 0xef,
   OPC_WR_SL_DATA_EXP= 0xee,
   OPC_ALM_WRITE     = 0xee, // =  Undocumented name
   OPC_MASK          = 0x7f  /* mask for acknowledge opcodes */
   };

   static QString OPC_NAME(int opcode);

   // start of values not from llnmon.c

   // Expanded slot index values
   static int EXP_MAST           ; //0;
   static int EXP_SLOT           ; //0x01;
   static int EXPD_LENGTH        ; //16;
   //offsets into message
   static int EXPD_STAT          ; //0;
   static int EXPD_ADRL          ; //1;
   static int EXPD_ADRH          ; //2;
   static int EXPD_FLAGS         ; //3;
   static int EXPD_SPD           ; //4;
   static int EXPD_F28F20F12     ; //5;
   static int EXPD_DIR_F0F4_F1   ; //6;
   static int EXPD_F11_F5        ; //7;
   static int EXPD_F19_F13       ; //8;
   static int EXPD_F27_F21       ; //9;

   //  opcode keys used to express interest in various messages
   //            note these are _not_ the loconet opcode values!

   static int KEY_GPBUSY 			;//= 1<<  0;
   static int KEY_GPOFF 			;//= 1<<  1;
   static int KEY_GPON 			;//= 1<<  2;
   static int KEY_IDLE  			;//= 1<<  3;

   static int KEY_LOCO_SPD		;//= 1<<  4;
   static int KEY_LOCO_DIRF		;//= 1<<  5;
   static int KEY_LOCO_SND		;//= 1<<  6;
   static int KEY_SW_REQ			;//= 1<<  7;

   static int KEY_SW_REP			;//= 1<<  8;
   static int KEY_INPUT_REP		;//= 1<<  9;
   static int KEY_LONG_ACK		;//= 1<< 10;
   static int KEY_SLOT_STAT1		;//= 1<< 11;

   static int KEY_CONSIST_FUNC	;//= 1<< 12;
   static int KEY_UNLINK_SLOTS	;//= 1<< 13;
   static int KEY_LINK_SLOTS		;//= 1<< 14;
   static int KEY_MOVE_SLOTS		;//= 1<< 15;

   static int KEY_RQ_SL_DATA		;//= 1<< 16;
   static int KEY_SW_STATE		;//= 1<< 17;
   static int KEY_SW_ACK			;//= 1<< 18;
   static int KEY_LOCO_ADR		;//= 1<< 19;

   static int KEY_PEER_XFR		;//= 1<< 20;
   static int KEY_IMM_PACKET		;//= 1<< 21;
   static int KEY_WR_SL_DATA		;//= 1<< 22;

   // reverse-engineered constants

   static int RE_IPL_MFR_DIGITRAX           ; //0x00;
   static int RE_IPL_MFR_ALL                ; //0x00;
   static int RE_IPL_DIGITRAX_HOST_UT4      ; //0x04;
   static int RE_IPL_DIGITRAX_HOST_UR92     ; //0x5C;
   static int RE_IPL_DIGITRAX_HOST_DCS51    ; //0x33;
   static int RE_IPL_DIGITRAX_HOST_DT402    ; //0x2A;
   static int RE_IPL_DIGITRAX_HOST_PR3      ; //0x23;
   static int RE_IPL_DIGITRAX_HOST_ALL      ; //0x00;
   static int RE_IPL_DIGITRAX_SLAVE_RF24    ; //0x18;
   static int RE_IPL_DIGITRAX_SLAVE_ALL     ; //0x00;
#define RE_IPL_PING_OPERATION         0x08
#define RE_IPL_IDENTITY_OPERATION     0x0f
#define RE_LACK_SPEC_CASE1            0x50 // special case LACK response for OpSw accesses
#define RE_LACK_SPEC_CASE2            0x00 // special case LACK response for OpSw accesses
#define RE_OPC_PR3_MODE               0xD3
   static int RE_MULTI_SENSE_DEV_TYPE_PM4X  ; //0x00;
   static int RE_MULTI_SENSE_DEV_TYPE_BDL16X; //0x01;
   static int RE_MULTI_SENSE_DEV_TYPE_SE8   ; //0x02;
   static int RE_MULTI_SENSE_DEV_TYPE_DS64  ; //0x03;

   // Below data is assumed, based on firmware files available from RR-Cirkits web site
   static int RE_IPL_MFR_RR_CIRKITS         ; //87;
   static int RE_IPL_RRCIRKITS_HOST_TC64    ; //11;
   static int RE_IPL_RRCIRKITS_SLAVE_ALL    ; //00;

   // Constants associated with OPC_PEER_XFR for Duplex operations
   static int RE_DPLX_OP_TYPE_WRITE             ; //0x00;
   static int RE_DPLX_OP_TYPE_QUERY             ; //0x08;
   static int RE_DPLX_OP_TYPE_REPORT            ; //0x10;
   static int RE_DPLX_OP_LEN                    ; //0x14;
   static int RE_IPL_OP_LEN                     ; //0x14;
   static int RE_IPL_OP_QUERY                   ; //0x08;
   static int RE_IPL_OP_REPORT                  ; //0x10;
   static int RE_IPL_OP_SLV_QUERY               ; //0x00;
   static int RE_IPL_OP_HFW_QUERY               ; //0x00;
   static int RE_IPL_OP_HSNM_QUERY              ; //0x00;
   static int RE_IPL_OP_SFW_QUERY               ; //0x00;
   static int RE_IPL_OP_HSN0_QUERY              ; //0x01;
   static int RE_IPL_OP_HSN1_QUERY              ; //0x00;
   static int RE_IPL_OP_HSN2_QUERY              ; //0x00;
   static int RE_IPL_OP_SSNM_QUERY              ; //0x00;
   static int RE_IPL__OP_SSN0_QUERY             ; //0x00;
   static int RE_IPL_OP_SSN1_QUERY              ; //0x00;
   static int RE_IPL_OP_SSN2_QUERY              ; //0x00;
   static int RE_IPL_OP_SSN3_QUERY              ; //0x00;
   static int RE_DPLX_GP_CHAN_TYPE              ; //2;
   static int RE_DPLX_GP_NAME_TYPE              ; //3;
   static int RE_DPLX_GP_ID_TYPE                ; //4;
   static int RE_DPLX_GP_PW_TYPE                ; //7;
   static int RE_DPLX_OPC_BAD                   ; //0x80;
   static int RE_DPLX_MSB1_BIT                  ; //1;
   static int RE_DPLX_MSB2_BIT                  ; //2;
   static int RE_DPLX_MSB3_BIT                  ; //4;
   static int RE_DPLX_MSB4_BIT                  ; //8;
   static int RE_DPLX_BUMP_MSB1_BIT             ; //7;
   static int RE_DPLX_BUMP_MSB2_BIT             ; //6;
   static int RE_DPLX_BUMP_MSB3_BIT             ; //5;
   static int RE_DPLX_BUMP_MSB4_BIT             ; //4;
   static int RE_DPLX_7BITS_MAX                 ; //127;
   static int RE_DPLX_MAX_NOT_OPC               ; //0x7F;
   static int RE_DPLX_ALT_CH_MSB_BIT            ; //0x4;
   static int RE_DPLX_ALT_CH_MSB_SHIFT          ; //0x5;
   static int RE_DPLX_ALT_ID_MSB_BIT            ; //0x8;
   static int RE_DPLX_ALT_ID_MSB_SHIFT          ; //0x4;
   static int RE_DPLX_ALT_PW1_MSB_BIT           ; //0x1;
   static int RE_DPLX_ALT_PW1_MSB_SHIFT         ; //0x3;
   static int RE_DPLX_ALT_PW3_MSB_BIT           ; //0x2;
   static int RE_DPLX_ALT_PW3_MSB_SHIFT         ; //0x2;

   static int RE_DPLX_DATA_LS_NIBBLE            ; //0x0F;
   static int RE_DPLX_DATA_MS_NIBBLE            ; //0x70;
   static int RE_DPLX_DATA_MS_NIBBLE_SHIFT      ; //4;

   // Duplex Group Scan Operation Constants
   static int RE_DPLX_SCAN_OP_LEN               ; //0x14 ;
   static int RE_DPLX_SCAN_QUERY_B2             ; //0x10 ;
   static int RE_DPLX_SCAN_QUERY_B3             ; //0x08 ;
   static int RE_DPLX_SCAN_QUERY_B4             ; //0x00 ;
   static int RE_DPLX_SCAN_REPORT_B2            ; //0x10 ;
   static int RE_DPLX_SCAN_REPORT_B3            ; //0x10 ;


};

#endif // LNCONSTANTS_H
