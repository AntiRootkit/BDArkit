#pragma once


#define    OPCODE_DYNAMIC      0x1u
#define    OPCODE_ADDRESS      0x2u
#define    OPCODE_NOENLARGE    0x4u
#define    OPCODE_RAX          0x8u
#define    OPCODE_SIB          0x10u
#define    OPCODE_RIP          0x20u
#define    OPCODE_NOTSIB       0x0fu


typedef struct _tagOPCODEITEM
{
	ULONG       nOpcode;    // Opcode
	ULONG       nFixedSize;    // Fixed size of opcode
	ULONG       nFixedSize16;    // Fixed size when 16 bit operand
	ULONG       nModOffset;    // Offset to mod/rm byte (0=none)
	LONG        nRelOffset;    // Offset to relative target.
	LONG        nTargetBack;    // Offset back to absolute or rip target
	ULONG       nFlagBits;    // Flags for DYNAMIC, etc.
}OPCODEITEM, *POPCODEITEM;

LONG GetOpcodeSize(PBYTE pOpcode);