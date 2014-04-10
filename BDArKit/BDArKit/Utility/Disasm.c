#pragma once


#include "StdAfx.h"
#include <NTDDK.h>
#include <WinDef.h>
#include "Disasm.h"


#define CONDITION_ASSERT(x) { if (!(x)){goto Exit0;};}

#define OPCODE_Bytes1            1, 1, 0, 0, 0, 0
#define OPCODE_Bytes1Dynamic     1, 1, 0, 0, 0, OPCODE_DYNAMIC
#define OPCODE_Bytes2            2, 2, 0, 0, 0, 0
#define OPCODE_Bytes2Jump        2, 2, 0, 1, 0, 0
#define OPCODE_Bytes2CantJump    2, 2, 0, 1, 0, OPCODE_NOENLARGE
#define OPCODE_Bytes2Dynamic     2, 2, 0, 0, 0, OPCODE_DYNAMIC
#define OPCODE_Bytes3            3, 3, 0, 0, 0, 0
#define OPCODE_Bytes3Dynamic     3, 3, 0, 0, 0, OPCODE_DYNAMIC
#define OPCODE_Bytes3Or5         5, 3, 0, 0, 0, 0
#define OPCODE_Bytes3Or5Rax      5, 3, 0, 0, 0, OPCODE_RAX
#define OPCODE_Bytes3Or5Target   5, 3, 0, 1, 0, 0
#define OPCODE_Bytes5Or7Dynamic  7, 5, 0, 0, 0, OPCODE_DYNAMIC
#define OPCODE_Bytes3Or5Address  5, 3, 0, 0, 0, OPCODE_ADDRESS
#define OPCODE_Bytes4            4, 4, 0, 0, 0, 0
#define OPCODE_Bytes5            5, 5, 0, 0, 0, 0
#define OPCODE_Bytes7            7, 7, 0, 0, 0, 0
#define OPCODE_Bytes2Mod         2, 2, 1, 0, 0, 0
#define OPCODE_Bytes2Mod1        3, 3, 1, 0, 1, 0
#define OPCODE_Bytes2ModOperand  6, 4, 1, 0, 4, 0
#define OPCODE_Bytes3Mod         3, 3, 2, 0, 0, 0
#define OPCODE_BytesPrefix       1, 1, 0, 0, 0, 0
#define OPCODE_BytesRax          1, 1, 0, 0, 0, 0
#define OPCODE_0F                1, 1, 0, 0, 0, 0
#define OPCODE_66                1, 1, 0, 0, 0, 0
#define OPCODE_67                1, 1, 0, 0, 0, 0
#define OPCODE_F6                0, 0, 0, 0, 0, 0
#define OPCODE_F7                0, 0, 0, 0, 0, 0
#define OPCODE_FF                2, 2, 1, 0, 0, 0
#define OPCODE_Invalid           1, 1, 0, 0, 0, 0
#define OPCODE_End               0, 0, 0, 0, 0, 0

OPCODEITEM g_OpcodeTable[257] =
{
	{ 0x00, OPCODE_Bytes2Mod },                      // ADD /r
	{ 0x01, OPCODE_Bytes2Mod },                      // ADD /r
	{ 0x02, OPCODE_Bytes2Mod },                      // ADD /r
	{ 0x03, OPCODE_Bytes2Mod },                      // ADD /r
	{ 0x04, OPCODE_Bytes2 },                         // ADD ib
	{ 0x05, OPCODE_Bytes3Or5 },                      // ADD iw
	{ 0x06, OPCODE_Bytes1 },                         // PUSH
	{ 0x07, OPCODE_Bytes1 },                         // POP
	{ 0x08, OPCODE_Bytes2Mod },                      // OR /r
	{ 0x09, OPCODE_Bytes2Mod },                      // OR /r
	{ 0x0A, OPCODE_Bytes2Mod },                      // OR /r
	{ 0x0B, OPCODE_Bytes2Mod },                      // OR /r
	{ 0x0C, OPCODE_Bytes2 },                         // OR ib
	{ 0x0D, OPCODE_Bytes3Or5 },                      // OR iw
	{ 0x0E, OPCODE_Bytes1 },                         // PUSH
	{ 0x0F, OPCODE_0F },                             // Extension Ops
	{ 0x10, OPCODE_Bytes2Mod },                      // ADC /r
	{ 0x11, OPCODE_Bytes2Mod },                      // ADC /r
	{ 0x12, OPCODE_Bytes2Mod },                      // ADC /r
	{ 0x13, OPCODE_Bytes2Mod },                      // ADC /r
	{ 0x14, OPCODE_Bytes2 },                         // ADC ib
	{ 0x15, OPCODE_Bytes3Or5 },                      // ADC id
	{ 0x16, OPCODE_Bytes1 },                         // PUSH
	{ 0x17, OPCODE_Bytes1 },                         // POP
	{ 0x18, OPCODE_Bytes2Mod },                      // SBB /r
	{ 0x19, OPCODE_Bytes2Mod },                      // SBB /r
	{ 0x1A, OPCODE_Bytes2Mod },                      // SBB /r
	{ 0x1B, OPCODE_Bytes2Mod },                      // SBB /r
	{ 0x1C, OPCODE_Bytes2 },                         // SBB ib
	{ 0x1D, OPCODE_Bytes3Or5 },                      // SBB id
	{ 0x1E, OPCODE_Bytes1 },                         // PUSH
	{ 0x1F, OPCODE_Bytes1 },                         // POP
	{ 0x20, OPCODE_Bytes2Mod },                      // AND /r
	{ 0x21, OPCODE_Bytes2Mod },                      // AND /r
	{ 0x22, OPCODE_Bytes2Mod },                      // AND /r
	{ 0x23, OPCODE_Bytes2Mod },                      // AND /r
	{ 0x24, OPCODE_Bytes2 },                         // AND ib
	{ 0x25, OPCODE_Bytes3Or5 },                      // AND id
	{ 0x26, OPCODE_BytesPrefix },                    // ES prefix
	{ 0x27, OPCODE_Bytes1 },                         // DAA
	{ 0x28, OPCODE_Bytes2Mod },                      // SUB /r
	{ 0x29, OPCODE_Bytes2Mod },                      // SUB /r
	{ 0x2A, OPCODE_Bytes2Mod },                      // SUB /r
	{ 0x2B, OPCODE_Bytes2Mod },                      // SUB /r
	{ 0x2C, OPCODE_Bytes2 },                         // SUB ib
	{ 0x2D, OPCODE_Bytes3Or5 },                      // SUB id
	{ 0x2E, OPCODE_BytesPrefix },                    // CS prefix
	{ 0x2F, OPCODE_Bytes1 },                         // DAS
	{ 0x30, OPCODE_Bytes2Mod },                      // XOR /r
	{ 0x31, OPCODE_Bytes2Mod },                      // XOR /r
	{ 0x32, OPCODE_Bytes2Mod },                      // XOR /r
	{ 0x33, OPCODE_Bytes2Mod },                      // XOR /r
	{ 0x34, OPCODE_Bytes2 },                         // XOR ib
	{ 0x35, OPCODE_Bytes3Or5 },                      // XOR id
	{ 0x36, OPCODE_BytesPrefix },                    // SS prefix
	{ 0x37, OPCODE_Bytes1 },                         // AAA
	{ 0x38, OPCODE_Bytes2Mod },                      // CMP /r
	{ 0x39, OPCODE_Bytes2Mod },                      // CMP /r
	{ 0x3A, OPCODE_Bytes2Mod },                      // CMP /r
	{ 0x3B, OPCODE_Bytes2Mod },                      // CMP /r
	{ 0x3C, OPCODE_Bytes2 },                         // CMP ib
	{ 0x3D, OPCODE_Bytes3Or5 },                      // CMP id
	{ 0x3E, OPCODE_BytesPrefix },                    // DS prefix
	{ 0x3F, OPCODE_Bytes1 },                         // AAS
#ifdef DETOURS_X64 // For Rax Prefix

#else
	{ 0x40, OPCODE_Bytes1 },                         // INC
	{ 0x41, OPCODE_Bytes1 },                         // INC
	{ 0x42, OPCODE_Bytes1 },                         // INC
	{ 0x43, OPCODE_Bytes1 },                         // INC
	{ 0x44, OPCODE_Bytes1 },                         // INC
	{ 0x45, OPCODE_Bytes1 },                         // INC
	{ 0x46, OPCODE_Bytes1 },                         // INC
	{ 0x47, OPCODE_Bytes1 },                         // INC
	{ 0x48, OPCODE_Bytes1 },                         // DEC
	{ 0x49, OPCODE_Bytes1 },                         // DEC
	{ 0x4A, OPCODE_Bytes1 },                         // DEC
	{ 0x4B, OPCODE_Bytes1 },                         // DEC
	{ 0x4C, OPCODE_Bytes1 },                         // DEC
	{ 0x4D, OPCODE_Bytes1 },                         // DEC
	{ 0x4E, OPCODE_Bytes1 },                         // DEC
	{ 0x4F, OPCODE_Bytes1 },                         // DEC
#endif
	{ 0x50, OPCODE_Bytes1 },                         // PUSH
	{ 0x51, OPCODE_Bytes1 },                         // PUSH
	{ 0x52, OPCODE_Bytes1 },                         // PUSH
	{ 0x53, OPCODE_Bytes1 },                         // PUSH
	{ 0x54, OPCODE_Bytes1 },                         // PUSH
	{ 0x55, OPCODE_Bytes1 },                         // PUSH
	{ 0x56, OPCODE_Bytes1 },                         // PUSH
	{ 0x57, OPCODE_Bytes1 },                         // PUSH
	{ 0x58, OPCODE_Bytes1 },                         // POP
	{ 0x59, OPCODE_Bytes1 },                         // POP
	{ 0x5A, OPCODE_Bytes1 },                         // POP
	{ 0x5B, OPCODE_Bytes1 },                         // POP
	{ 0x5C, OPCODE_Bytes1 },                         // POP
	{ 0x5D, OPCODE_Bytes1 },                         // POP
	{ 0x5E, OPCODE_Bytes1 },                         // POP
	{ 0x5F, OPCODE_Bytes1 },                         // POP
	{ 0x60, OPCODE_Bytes1 },                         // PUSHAD
	{ 0x61, OPCODE_Bytes1 },                         // POPAD
	{ 0x62, OPCODE_Bytes2Mod },                      // BOUND /r
	{ 0x63, OPCODE_Bytes2Mod },                      // ARPL /r
	{ 0x64, OPCODE_BytesPrefix },                    // FS prefix
	{ 0x65, OPCODE_BytesPrefix },                    // GS prefix
	{ 0x66, OPCODE_66 },                             // Operand Prefix
	{ 0x67, OPCODE_67 },                             // Address Prefix
	{ 0x68, OPCODE_Bytes3Or5 },                      // PUSH
	{ 0x69, OPCODE_Bytes2ModOperand },               //
	{ 0x6A, OPCODE_Bytes2 },                         // PUSH
	{ 0x6B, OPCODE_Bytes2Mod1 },                     // IMUL /r ib
	{ 0x6C, OPCODE_Bytes1 },                         // INS
	{ 0x6D, OPCODE_Bytes1 },                         // INS
	{ 0x6E, OPCODE_Bytes1 },                         // OUTS/OUTSB
	{ 0x6F, OPCODE_Bytes1 },                         // OUTS/OUTSW
	{ 0x70, OPCODE_Bytes2Jump },                     // JO
	{ 0x71, OPCODE_Bytes2Jump },                     // JNO
	{ 0x72, OPCODE_Bytes2Jump },                     // JB/JC/JNAE
	{ 0x73, OPCODE_Bytes2Jump },                     // JAE/JNB/JNC
	{ 0x74, OPCODE_Bytes2Jump },                     // JE/JZ
	{ 0x75, OPCODE_Bytes2Jump },                     // JNE/JNZ
	{ 0x76, OPCODE_Bytes2Jump },                     // JBE/JNA
	{ 0x77, OPCODE_Bytes2Jump },                     // JA/JNBE
	{ 0x78, OPCODE_Bytes2Jump },                     // JS
	{ 0x79, OPCODE_Bytes2Jump },                     // JNS
	{ 0x7A, OPCODE_Bytes2Jump },                     // JP/JPE
	{ 0x7B, OPCODE_Bytes2Jump },                     // JNP/JPO
	{ 0x7C, OPCODE_Bytes2Jump },                     // JL/JNGE
	{ 0x7D, OPCODE_Bytes2Jump },                     // JGE/JNL
	{ 0x7E, OPCODE_Bytes2Jump },                     // JLE/JNG
	{ 0x7F, OPCODE_Bytes2Jump },                     // JG/JNLE
	{ 0x80, OPCODE_Bytes2Mod1 },                     // ADC/2 ib, etc.s
	{ 0x81, OPCODE_Bytes2ModOperand },               //
	{ 0x82, OPCODE_Bytes2 },                         // MOV al,x
	{ 0x83, OPCODE_Bytes2Mod1 },                     // ADC/2 ib, etc.
	{ 0x84, OPCODE_Bytes2Mod },                      // TEST /r
	{ 0x85, OPCODE_Bytes2Mod },                      // TEST /r
	{ 0x86, OPCODE_Bytes2Mod },                      // XCHG /r @todo
	{ 0x87, OPCODE_Bytes2Mod },                      // XCHG /r @todo
	{ 0x88, OPCODE_Bytes2Mod },                      // MOV /r
	{ 0x89, OPCODE_Bytes2Mod },                      // MOV /r
	{ 0x8A, OPCODE_Bytes2Mod },                      // MOV /r
	{ 0x8B, OPCODE_Bytes2Mod },                      // MOV /r
	{ 0x8C, OPCODE_Bytes2Mod },                      // MOV /r
	{ 0x8D, OPCODE_Bytes2Mod },                      // LEA /r
	{ 0x8E, OPCODE_Bytes2Mod },                      // MOV /r
	{ 0x8F, OPCODE_Bytes2Mod },                      // POP /0
	{ 0x90, OPCODE_Bytes1 },                         // NOP
	{ 0x91, OPCODE_Bytes1 },                         // XCHG
	{ 0x92, OPCODE_Bytes1 },                         // XCHG
	{ 0x93, OPCODE_Bytes1 },                         // XCHG
	{ 0x94, OPCODE_Bytes1 },                         // XCHG
	{ 0x95, OPCODE_Bytes1 },                         // XCHG
	{ 0x96, OPCODE_Bytes1 },                         // XCHG
	{ 0x97, OPCODE_Bytes1 },                         // XCHG
	{ 0x98, OPCODE_Bytes1 },                         // CWDE
	{ 0x99, OPCODE_Bytes1 },                         // CDQ
	{ 0x9A, OPCODE_Bytes5Or7Dynamic },               // CALL cp
	{ 0x9B, OPCODE_Bytes1 },                         // WAIT/FWAIT
	{ 0x9C, OPCODE_Bytes1 },                         // PUSHFD
	{ 0x9D, OPCODE_Bytes1 },                         // POPFD
	{ 0x9E, OPCODE_Bytes1 },                         // SAHF
	{ 0x9F, OPCODE_Bytes1 },                         // LAHF
	{ 0xA0, OPCODE_Bytes3Or5Address },               // MOV
	{ 0xA1, OPCODE_Bytes3Or5Address },               // MOV
	{ 0xA2, OPCODE_Bytes3Or5Address },               // MOV
	{ 0xA3, OPCODE_Bytes3Or5Address },               // MOV
	{ 0xA4, OPCODE_Bytes1 },                         // MOVS
	{ 0xA5, OPCODE_Bytes1 },                         // MOVS/MOVSD
	{ 0xA6, OPCODE_Bytes1 },                         // CMPS/CMPSB
	{ 0xA7, OPCODE_Bytes1 },                         // CMPS/CMPSW
	{ 0xA8, OPCODE_Bytes2 },                         // TEST
	{ 0xA9, OPCODE_Bytes3Or5 },                      // TEST
	{ 0xAA, OPCODE_Bytes1 },                         // STOS/STOSB
	{ 0xAB, OPCODE_Bytes1 },                         // STOS/STOSW
	{ 0xAC, OPCODE_Bytes1 },                         // LODS/LODSB
	{ 0xAD, OPCODE_Bytes1 },                         // LODS/LODSW
	{ 0xAE, OPCODE_Bytes1 },                         // SCAS/SCASB
	{ 0xAF, OPCODE_Bytes1 },                         // SCAS/SCASD
	{ 0xB0, OPCODE_Bytes2 },                         // MOV B0+rb
	{ 0xB1, OPCODE_Bytes2 },                         // MOV B0+rb
	{ 0xB2, OPCODE_Bytes2 },                         // MOV B0+rb
	{ 0xB3, OPCODE_Bytes2 },                         // MOV B0+rb
	{ 0xB4, OPCODE_Bytes2 },                         // MOV B0+rb
	{ 0xB5, OPCODE_Bytes2 },                         // MOV B0+rb
	{ 0xB6, OPCODE_Bytes2 },                         // MOV B0+rb
	{ 0xB7, OPCODE_Bytes2 },                         // MOV B0+rb
	{ 0xB8, OPCODE_Bytes3Or5Rax },                   // MOV B8+rb
	{ 0xB9, OPCODE_Bytes3Or5 },                      // MOV B8+rb
	{ 0xBA, OPCODE_Bytes3Or5 },                      // MOV B8+rb
	{ 0xBB, OPCODE_Bytes3Or5 },                      // MOV B8+rb
	{ 0xBC, OPCODE_Bytes3Or5 },                      // MOV B8+rb
	{ 0xBD, OPCODE_Bytes3Or5 },                      // MOV B8+rb
	{ 0xBE, OPCODE_Bytes3Or5 },                      // MOV B8+rb
	{ 0xBF, OPCODE_Bytes3Or5 },                      // MOV B8+rb
	{ 0xC0, OPCODE_Bytes2Mod1 },                     // RCL/2 ib, etc.
	{ 0xC1, OPCODE_Bytes2Mod1 },                     // RCL/2 ib, etc.
	{ 0xC2, OPCODE_Bytes3 },                         // RET
	{ 0xC3, OPCODE_Bytes1 },                         // RET
	{ 0xC4, OPCODE_Bytes2Mod },                      // LES
	{ 0xC5, OPCODE_Bytes2Mod },                      // LDS
	{ 0xC6, OPCODE_Bytes2Mod1 },                     // MOV
	{ 0xC7, OPCODE_Bytes2ModOperand },               // MOV
	{ 0xC8, OPCODE_Bytes4 },                         // ENTER
	{ 0xC9, OPCODE_Bytes1 },                         // LEAVE
	{ 0xCA, OPCODE_Bytes3Dynamic },                  // RET
	{ 0xCB, OPCODE_Bytes1Dynamic },                  // RET
	{ 0xCC, OPCODE_Bytes1Dynamic },                  // INT 3
	{ 0xCD, OPCODE_Bytes2Dynamic },                  // INT ib
	{ 0xCE, OPCODE_Bytes1Dynamic },                  // INTO
	{ 0xCF, OPCODE_Bytes1Dynamic },                  // IRET
	{ 0xD0, OPCODE_Bytes2Mod },                      // RCL/2, etc.
	{ 0xD1, OPCODE_Bytes2Mod },                      // RCL/2, etc.
	{ 0xD2, OPCODE_Bytes2Mod },                      // RCL/2, etc.
	{ 0xD3, OPCODE_Bytes2Mod },                      // RCL/2, etc.
	{ 0xD4, OPCODE_Bytes2 },                         // AAM
	{ 0xD5, OPCODE_Bytes2 },                         // AAD
	{ 0xD6, OPCODE_Invalid },                            //
	{ 0xD7, OPCODE_Bytes1 },                         // XLAT/XLATB
	{ 0xD8, OPCODE_Bytes2Mod },                      // FADD, etc.
	{ 0xD9, OPCODE_Bytes2Mod },                      // F2XM1, etc.
	{ 0xDA, OPCODE_Bytes2Mod },                      // FLADD, etc.
	{ 0xDB, OPCODE_Bytes2Mod },                      // FCLEX, etc.
	{ 0xDC, OPCODE_Bytes2Mod },                      // FADD/0, etc.
	{ 0xDD, OPCODE_Bytes2Mod },                      // FFREE, etc.
	{ 0xDE, OPCODE_Bytes2Mod },                      // FADDP, etc.
	{ 0xDF, OPCODE_Bytes2Mod },                      // FBLD/4, etc.
	{ 0xE0, OPCODE_Bytes2CantJump },                 // LOOPNE cb
	{ 0xE1, OPCODE_Bytes2CantJump },                 // LOOPE cb
	{ 0xE2, OPCODE_Bytes2CantJump },                 // LOOP cb
	{ 0xE3, OPCODE_Bytes2Jump },                     // JCXZ/JECXZ
	{ 0xE4, OPCODE_Bytes2 },                         // __in ib
	{ 0xE5, OPCODE_Bytes2 },                         // __in id
	{ 0xE6, OPCODE_Bytes2 },                         // __out ib
	{ 0xE7, OPCODE_Bytes2 },                         // __out ib
	{ 0xE8, OPCODE_Bytes3Or5Target },                // CALL cd
	{ 0xE9, OPCODE_Bytes3Or5Target },                // JMP cd
	{ 0xEA, OPCODE_Bytes5Or7Dynamic },               // JMP cp
	{ 0xEB, OPCODE_Bytes2Jump },                     // JMP cb
	{ 0xEC, OPCODE_Bytes1 },                         // __in ib
	{ 0xED, OPCODE_Bytes1 },                         // __in id
	{ 0xEE, OPCODE_Bytes1 },                         // __out
	{ 0xEF, OPCODE_Bytes1 },                         // __out
	{ 0xF0, OPCODE_BytesPrefix },                    // LOCK prefix
	{ 0xF1, OPCODE_Invalid },                            //
	{ 0xF2, OPCODE_BytesPrefix },                    // REPNE prefix
	{ 0xF3, OPCODE_BytesPrefix },                    // REPE prefix
	{ 0xF4, OPCODE_Bytes1 },                         // HLT
	{ 0xF5, OPCODE_Bytes1 },                         // CMC
	{ 0xF6, OPCODE_F6 },                             // TEST/0, DIV/6
	{ 0xF7, OPCODE_F7 },                             // TEST/0, DIV/6
	{ 0xF8, OPCODE_Bytes1 },                         // CLC
	{ 0xF9, OPCODE_Bytes1 },                         // STC
	{ 0xFA, OPCODE_Bytes1 },                         // CLI
	{ 0xFB, OPCODE_Bytes1 },                         // STI
	{ 0xFC, OPCODE_Bytes1 },                         // CLD
	{ 0xFD, OPCODE_Bytes1 },                         // STD
	{ 0xFE, OPCODE_Bytes2Mod },                      // DEC/1,INC/0
	{ 0xFF, OPCODE_FF },                             // CALL/2
	{ 0, OPCODE_End },
};

OPCODEITEM g_opcode0FTable[257] =
{
	{ 0x00, OPCODE_Bytes2Mod },                      // LLDT/2, etc.
	{ 0x01, OPCODE_Bytes2Mod },                      // INVLPG/7, etc.
	{ 0x02, OPCODE_Bytes2Mod },                      // LAR/r
	{ 0x03, OPCODE_Bytes2Mod },                      // LSL/r
	{ 0x04, OPCODE_Invalid },                            // _04
	{ 0x05, OPCODE_Invalid },                            // _05
	{ 0x06, OPCODE_Bytes2 },                         // CLTS
	{ 0x07, OPCODE_Invalid },                            // _07
	{ 0x08, OPCODE_Bytes2 },                         // INVD
	{ 0x09, OPCODE_Bytes2 },                         // WBINVD
	{ 0x0A, OPCODE_Invalid },                            // _0A
	{ 0x0B, OPCODE_Bytes2 },                         // UD2
	{ 0x0C, OPCODE_Invalid },                            // _0C
	{ 0x0D, OPCODE_Bytes2Mod },                      // PREFETCH
	{ 0x0E, OPCODE_Bytes2 },                         // FEMMS
	{ 0x0F, OPCODE_Bytes3Mod },                      // 3DNow Opcodes
	{ 0x10, OPCODE_Bytes2Mod },                      // MOVSS MOVUPD MOVSD
	{ 0x11, OPCODE_Bytes2Mod },                      // MOVSS MOVUPD MOVSD
	{ 0x12, OPCODE_Bytes2Mod },                      // MOVLPD
	{ 0x13, OPCODE_Bytes2Mod },                      // MOVLPD
	{ 0x14, OPCODE_Bytes2Mod },                      // UNPCKLPD
	{ 0x15, OPCODE_Bytes2Mod },                      // UNPCKHPD
	{ 0x16, OPCODE_Bytes2Mod },                      // MOVHPD
	{ 0x17, OPCODE_Bytes2Mod },                      // MOVHPD
	{ 0x18, OPCODE_Bytes2Mod },                      // PREFETCHINTA...
	{ 0x19, OPCODE_Invalid },                            // _19
	{ 0x1A, OPCODE_Invalid },                            // _1A
	{ 0x1B, OPCODE_Invalid },                            // _1B
	{ 0x1C, OPCODE_Invalid },                            // _1C
	{ 0x1D, OPCODE_Invalid },                            // _1D
	{ 0x1E, OPCODE_Invalid },                            // _1E
	{ 0x1F, OPCODE_Invalid },                            // _1F
	{ 0x20, OPCODE_Bytes2Mod },                      // MOV/r
	{ 0x21, OPCODE_Bytes2Mod },                      // MOV/r
	{ 0x22, OPCODE_Bytes2Mod },                      // MOV/r
	{ 0x23, OPCODE_Bytes2Mod },                      // MOV/r
	{ 0x24, OPCODE_Invalid },                            // _24
	{ 0x25, OPCODE_Invalid },                            // _25
	{ 0x26, OPCODE_Invalid },                            // _26
	{ 0x27, OPCODE_Invalid },                            // _27
	{ 0x28, OPCODE_Bytes2Mod },                      // MOVAPS MOVAPD
	{ 0x29, OPCODE_Bytes2Mod },                      // MOVAPS MOVAPD
	{ 0x2A, OPCODE_Bytes2Mod },                      // CVPI2PS &
	{ 0x2B, OPCODE_Bytes2Mod },                      // MOVNTPS MOVNTPD
	{ 0x2C, OPCODE_Bytes2Mod },                      // CVTTPS2PI &
	{ 0x2D, OPCODE_Bytes2Mod },                      // CVTPS2PI &
	{ 0x2E, OPCODE_Bytes2Mod },                      // UCOMISS UCOMISD
	{ 0x2F, OPCODE_Bytes2Mod },                      // COMISS COMISD
	{ 0x30, OPCODE_Bytes2 },                         // WRMSR
	{ 0x31, OPCODE_Bytes2 },                         // RDTSC
	{ 0x32, OPCODE_Bytes2 },                         // RDMSR
	{ 0x33, OPCODE_Bytes2 },                         // RDPMC
	{ 0x34, OPCODE_Bytes2 },                         // SYSENTER
	{ 0x35, OPCODE_Bytes2 },                         // SYSEXIT
	{ 0x36, OPCODE_Invalid },                            // _36
	{ 0x37, OPCODE_Invalid },                            // _37
	{ 0x38, OPCODE_Invalid },                            // _38
	{ 0x39, OPCODE_Invalid },                            // _39
	{ 0x3A, OPCODE_Invalid },                            // _3A
	{ 0x3B, OPCODE_Invalid },                            // _3B
	{ 0x3C, OPCODE_Invalid },                            // _3C
	{ 0x3D, OPCODE_Invalid },                            // _3D
	{ 0x3E, OPCODE_Invalid },                            // _3E
	{ 0x3F, OPCODE_Invalid },                            // _3F
	{ 0x40, OPCODE_Bytes2Mod },                      // CMOVO (0F 40)
	{ 0x41, OPCODE_Bytes2Mod },                      // CMOVNO (0F 41)
	{ 0x42, OPCODE_Bytes2Mod },                      // CMOVB & CMOVNE (0F 42)
	{ 0x43, OPCODE_Bytes2Mod },                      // CMOVAE & CMOVNB (0F 43)
	{ 0x44, OPCODE_Bytes2Mod },                      // CMOVE & CMOVZ (0F 44)
	{ 0x45, OPCODE_Bytes2Mod },                      // CMOVNE & CMOVNZ (0F 45)
	{ 0x46, OPCODE_Bytes2Mod },                      // CMOVBE & CMOVNA (0F 46)
	{ 0x47, OPCODE_Bytes2Mod },                      // CMOVA & CMOVNBE (0F 47)
	{ 0x48, OPCODE_Bytes2Mod },                      // CMOVS (0F 48)
	{ 0x49, OPCODE_Bytes2Mod },                      // CMOVNS (0F 49)
	{ 0x4A, OPCODE_Bytes2Mod },                      // CMOVP & CMOVPE (0F 4A)
	{ 0x4B, OPCODE_Bytes2Mod },                      // CMOVNP & CMOVPO (0F 4B)
	{ 0x4C, OPCODE_Bytes2Mod },                      // CMOVL & CMOVNGE (0F 4C)
	{ 0x4D, OPCODE_Bytes2Mod },                      // CMOVGE & CMOVNL (0F 4D)
	{ 0x4E, OPCODE_Bytes2Mod },                      // CMOVLE & CMOVNG (0F 4E)
	{ 0x4F, OPCODE_Bytes2Mod },                      // CMOVG & CMOVNLE (0F 4F)
	{ 0x50, OPCODE_Bytes2Mod },                      // MOVMSKPD MOVMSKPD
	{ 0x51, OPCODE_Bytes2Mod },                      // SQRTPS &
	{ 0x52, OPCODE_Bytes2Mod },                      // RSQRTTS RSQRTPS
	{ 0x53, OPCODE_Bytes2Mod },                      // RCPPS RCPSS
	{ 0x54, OPCODE_Bytes2Mod },                      // ANDPS ANDPD
	{ 0x55, OPCODE_Bytes2Mod },                      // ANDNPS ANDNPD
	{ 0x56, OPCODE_Bytes2Mod },                      // ORPS ORPD
	{ 0x57, OPCODE_Bytes2Mod },                      // XORPS XORPD
	{ 0x58, OPCODE_Bytes2Mod },                      // ADDPS &
	{ 0x59, OPCODE_Bytes2Mod },                      // MULPS &
	{ 0x5A, OPCODE_Bytes2Mod },                      // CVTPS2PD &
	{ 0x5B, OPCODE_Bytes2Mod },                      // CVTDQ2PS &
	{ 0x5C, OPCODE_Bytes2Mod },                      // SUBPS &
	{ 0x5D, OPCODE_Bytes2Mod },                      // MINPS &
	{ 0x5E, OPCODE_Bytes2Mod },                      // DIVPS &
	{ 0x5F, OPCODE_Bytes2Mod },                      // MASPS &
	{ 0x60, OPCODE_Bytes2Mod },                      // PUNPCKLBW/r
	{ 0x61, OPCODE_Bytes2Mod },                      // PUNPCKLWD/r
	{ 0x62, OPCODE_Bytes2Mod },                      // PUNPCKLWD/r
	{ 0x63, OPCODE_Bytes2Mod },                      // PACKSSWB/r
	{ 0x64, OPCODE_Bytes2Mod },                      // PCMPGTB/r
	{ 0x65, OPCODE_Bytes2Mod },                      // PCMPGTW/r
	{ 0x66, OPCODE_Bytes2Mod },                      // PCMPGTD/r
	{ 0x67, OPCODE_Bytes2Mod },                      // PACKUSWB/r
	{ 0x68, OPCODE_Bytes2Mod },                      // PUNPCKHBW/r
	{ 0x69, OPCODE_Bytes2Mod },                      // PUNPCKHWD/r
	{ 0x6A, OPCODE_Bytes2Mod },                      // PUNPCKHDQ/r
	{ 0x6B, OPCODE_Bytes2Mod },                      // PACKSSDW/r
	{ 0x6C, OPCODE_Bytes2Mod },                      // PUNPCKLQDQ
	{ 0x6D, OPCODE_Bytes2Mod },                      // PUNPCKHQDQ
	{ 0x6E, OPCODE_Bytes2Mod },                      // MOVD/r
	{ 0x6F, OPCODE_Bytes2Mod },                      // MOV/r
	{ 0x70, OPCODE_Bytes2Mod1 },                     // PSHUFW/r ib
	{ 0x71, OPCODE_Bytes2Mod1 },                     // PSLLW/6 ib,PSRAW/4 ib,PSRLW/2 ib
	{ 0x72, OPCODE_Bytes2Mod1 },                     // PSLLD/6 ib,PSRAD/4 ib,PSRLD/2 ib
	{ 0x73, OPCODE_Bytes2Mod1 },                     // PSLLQ/6 ib,PSRLQ/2 ib
	{ 0x74, OPCODE_Bytes2Mod },                      // PCMPEQB/r
	{ 0x75, OPCODE_Bytes2Mod },                      // PCMPEQW/r
	{ 0x76, OPCODE_Bytes2Mod },                      // PCMPEQD/r
	{ 0x77, OPCODE_Bytes2 },                         // EMMS
	{ 0x78, OPCODE_Invalid },                            // _78
	{ 0x79, OPCODE_Invalid },                            // _79
	{ 0x7A, OPCODE_Invalid },                            // _7A
	{ 0x7B, OPCODE_Invalid },                            // _7B
	{ 0x7C, OPCODE_Invalid },                            // _7C
	{ 0x7D, OPCODE_Invalid },                            // _7D
	{ 0x7E, OPCODE_Bytes2Mod },                      // MOVD/r
	{ 0x7F, OPCODE_Bytes2Mod },                      // MOV/r
	{ 0x80, OPCODE_Bytes3Or5Target },                // JO
	{ 0x81, OPCODE_Bytes3Or5Target },                // JNO
	{ 0x82, OPCODE_Bytes3Or5Target },                // JB,JC,JNAE
	{ 0x83, OPCODE_Bytes3Or5Target },                // JAE,JNB,JNC
	{ 0x84, OPCODE_Bytes3Or5Target },                // JE,JZ,JZ
	{ 0x85, OPCODE_Bytes3Or5Target },                // JNE,JNZ
	{ 0x86, OPCODE_Bytes3Or5Target },                // JBE,JNA
	{ 0x87, OPCODE_Bytes3Or5Target },                // JA,JNBE
	{ 0x88, OPCODE_Bytes3Or5Target },                // JS
	{ 0x89, OPCODE_Bytes3Or5Target },                // JNS
	{ 0x8A, OPCODE_Bytes3Or5Target },                // JP,JPE
	{ 0x8B, OPCODE_Bytes3Or5Target },                // JNP,JPO
	{ 0x8C, OPCODE_Bytes3Or5Target },                // JL,NGE
	{ 0x8D, OPCODE_Bytes3Or5Target },                // JGE,JNL
	{ 0x8E, OPCODE_Bytes3Or5Target },                // JLE,JNG
	{ 0x8F, OPCODE_Bytes3Or5Target },                // JG,JNLE
	{ 0x90, OPCODE_Bytes2Mod },                      // CMOVO (0F 40)
	{ 0x91, OPCODE_Bytes2Mod },                      // CMOVNO (0F 41)
	{ 0x92, OPCODE_Bytes2Mod },                      // CMOVB & CMOVC & CMOVNAE (0F 42)
	{ 0x93, OPCODE_Bytes2Mod },                      // CMOVAE & CMOVNB & CMOVNC (0F 43)
	{ 0x94, OPCODE_Bytes2Mod },                      // CMOVE & CMOVZ (0F 44)
	{ 0x95, OPCODE_Bytes2Mod },                      // CMOVNE & CMOVNZ (0F 45)
	{ 0x96, OPCODE_Bytes2Mod },                      // CMOVBE & CMOVNA (0F 46)
	{ 0x97, OPCODE_Bytes2Mod },                      // CMOVA & CMOVNBE (0F 47)
	{ 0x98, OPCODE_Bytes2Mod },                      // CMOVS (0F 48)
	{ 0x99, OPCODE_Bytes2Mod },                      // CMOVNS (0F 49)
	{ 0x9A, OPCODE_Bytes2Mod },                      // CMOVP & CMOVPE (0F 4A)
	{ 0x9B, OPCODE_Bytes2Mod },                      // CMOVNP & CMOVPO (0F 4B)
	{ 0x9C, OPCODE_Bytes2Mod },                      // CMOVL & CMOVNGE (0F 4C)
	{ 0x9D, OPCODE_Bytes2Mod },                      // CMOVGE & CMOVNL (0F 4D)
	{ 0x9E, OPCODE_Bytes2Mod },                      // CMOVLE & CMOVNG (0F 4E)
	{ 0x9F, OPCODE_Bytes2Mod },                      // CMOVG & CMOVNLE (0F 4F)
	{ 0xA0, OPCODE_Bytes2 },                         // PUSH
	{ 0xA1, OPCODE_Bytes2 },                         // POP
	{ 0xA2, OPCODE_Bytes2 },                         // CPUID
	{ 0xA3, OPCODE_Bytes2Mod },                      // BT  (0F A3)
	{ 0xA4, OPCODE_Bytes2Mod1 },                     // SHLD
	{ 0xA5, OPCODE_Bytes2Mod },                      // SHLD
	{ 0xA6, OPCODE_Invalid },                            // _A6
	{ 0xA7, OPCODE_Invalid },                            // _A7
	{ 0xA8, OPCODE_Bytes2 },                         // PUSH
	{ 0xA9, OPCODE_Bytes2 },                         // POP
	{ 0xAA, OPCODE_Bytes2 },                         // RSM
	{ 0xAB, OPCODE_Bytes2Mod },                      // BTS (0F AB)
	{ 0xAC, OPCODE_Bytes2Mod1 },                     // SHRD
	{ 0xAD, OPCODE_Bytes2Mod },                      // SHRD
	{ 0xAE, OPCODE_Bytes2Mod },                      // FXRSTOR/1,FXSAVE/0
	{ 0xAF, OPCODE_Bytes2Mod },                      // IMUL (0F AF)
	{ 0xB0, OPCODE_Bytes2Mod },                      // CMPXCHG (0F B0)
	{ 0xB1, OPCODE_Bytes2Mod },                      // CMPXCHG (0F B1)
	{ 0xB2, OPCODE_Bytes2Mod },                      // LSS/r
	{ 0xB3, OPCODE_Bytes2Mod },                      // BTR (0F B3)
	{ 0xB4, OPCODE_Bytes2Mod },                      // LFS/r
	{ 0xB5, OPCODE_Bytes2Mod },                      // LGS/r
	{ 0xB6, OPCODE_Bytes2Mod },                      // MOVZX/r
	{ 0xB7, OPCODE_Bytes2Mod },                      // MOVZX/r
	{ 0xB8, OPCODE_Invalid },                            // _B8
	{ 0xB9, OPCODE_Invalid },                            // _B9
	{ 0xBA, OPCODE_Bytes2Mod1 },                     // BT & BTC & BTR & BTS (0F BA)
	{ 0xBB, OPCODE_Bytes2Mod },                      // BTC (0F BB)
	{ 0xBC, OPCODE_Bytes2Mod },                      // BSF (0F BC)
	{ 0xBD, OPCODE_Bytes2Mod },                      // BSR (0F BD)
	{ 0xBE, OPCODE_Bytes2Mod },                      // MOVSX/r
	{ 0xBF, OPCODE_Bytes2Mod },                      // MOVSX/r
	{ 0xC0, OPCODE_Bytes2Mod },                      // XADD/r
	{ 0xC1, OPCODE_Bytes2Mod },                      // XADD/r
	{ 0xC2, OPCODE_Bytes2Mod },                      // CMPPS &
	{ 0xC3, OPCODE_Bytes2Mod },                      // MOVNTI
	{ 0xC4, OPCODE_Bytes2Mod1 },                     // PINSRW /r ib
	{ 0xC5, OPCODE_Bytes2Mod1 },                     // PEXTRW /r ib
	{ 0xC6, OPCODE_Bytes2Mod1 },                     // SHUFPS & SHUFPD
	{ 0xC7, OPCODE_Bytes2Mod },                      // CMPXCHG8B (0F C7)
	{ 0xC8, OPCODE_Bytes2 },                         // BSWAP 0F C8 + rd
	{ 0xC9, OPCODE_Bytes2 },                         // BSWAP 0F C8 + rd
	{ 0xCA, OPCODE_Bytes2 },                         // BSWAP 0F C8 + rd
	{ 0xCB, OPCODE_Bytes2 },                         //CVTPD2PI BSWAP 0F C8 + rd
	{ 0xCC, OPCODE_Bytes2 },                         // BSWAP 0F C8 + rd
	{ 0xCD, OPCODE_Bytes2 },                         // BSWAP 0F C8 + rd
	{ 0xCE, OPCODE_Bytes2 },                         // BSWAP 0F C8 + rd
	{ 0xCF, OPCODE_Bytes2 },                         // BSWAP 0F C8 + rd
	{ 0xD0, OPCODE_Invalid },                            // _D0
	{ 0xD1, OPCODE_Bytes2Mod },                      // PSRLW/r
	{ 0xD2, OPCODE_Bytes2Mod },                      // PSRLD/r
	{ 0xD3, OPCODE_Bytes2Mod },                      // PSRLQ/r
	{ 0xD4, OPCODE_Bytes2Mod },                      // PADDQ
	{ 0xD5, OPCODE_Bytes2Mod },                      // PMULLW/r
	{ 0xD6, OPCODE_Bytes2Mod },                      // MOVDQ2Q / MOVQ2DQ
	{ 0xD7, OPCODE_Bytes2Mod },                      // PMOVMSKB/r
	{ 0xD8, OPCODE_Bytes2Mod },                      // PSUBUSB/r
	{ 0xD9, OPCODE_Bytes2Mod },                      // PSUBUSW/r
	{ 0xDA, OPCODE_Bytes2Mod },                      // PMINUB/r
	{ 0xDB, OPCODE_Bytes2Mod },                      // PAND/r
	{ 0xDC, OPCODE_Bytes2Mod },                      // PADDUSB/r
	{ 0xDD, OPCODE_Bytes2Mod },                      // PADDUSW/r
	{ 0xDE, OPCODE_Bytes2Mod },                      // PMAXUB/r
	{ 0xDF, OPCODE_Bytes2Mod },                      // PANDN/r
	{ 0xE0, OPCODE_Bytes2Mod  },                     // PAVGB
	{ 0xE1, OPCODE_Bytes2Mod },                      // PSRAW/r
	{ 0xE2, OPCODE_Bytes2Mod },                      // PSRAD/r
	{ 0xE3, OPCODE_Bytes2Mod },                      // PAVGW
	{ 0xE4, OPCODE_Bytes2Mod },                      // PMULHUW/r
	{ 0xE5, OPCODE_Bytes2Mod },                      // PMULHW/r
	{ 0xE6, OPCODE_Bytes2Mod },                      // CTDQ2PD &
	{ 0xE7, OPCODE_Bytes2Mod },                      // MOVNTQ
	{ 0xE8, OPCODE_Bytes2Mod },                      // PSUBB/r
	{ 0xE9, OPCODE_Bytes2Mod },                      // PSUBW/r
	{ 0xEA, OPCODE_Bytes2Mod },                      // PMINSW/r
	{ 0xEB, OPCODE_Bytes2Mod },                      // POR/r
	{ 0xEC, OPCODE_Bytes2Mod },                      // PADDSB/r
	{ 0xED, OPCODE_Bytes2Mod },                      // PADDSW/r
	{ 0xEE, OPCODE_Bytes2Mod },                      // PMAXSW /r
	{ 0xEF, OPCODE_Bytes2Mod },                      // PXOR/r
	{ 0xF0, OPCODE_Invalid },                            // _F0
	{ 0xF1, OPCODE_Bytes2Mod },                      // PSLLW/r
	{ 0xF2, OPCODE_Bytes2Mod },                      // PSLLD/r
	{ 0xF3, OPCODE_Bytes2Mod },                      // PSLLQ/r
	{ 0xF4, OPCODE_Bytes2Mod },                      // PMULUDQ/r
	{ 0xF5, OPCODE_Bytes2Mod },                      // PMADDWD/r
	{ 0xF6, OPCODE_Bytes2Mod },                      // PSADBW/r
	{ 0xF7, OPCODE_Bytes2Mod },                      // MASKMOVQ
	{ 0xF8, OPCODE_Bytes2Mod },                      // PSUBB/r
	{ 0xF9, OPCODE_Bytes2Mod },                      // PSUBW/r
	{ 0xFA, OPCODE_Bytes2Mod },                      // PSUBD/r
	{ 0xFB, OPCODE_Bytes2Mod },                      // FSUBQ/r
	{ 0xFC, OPCODE_Bytes2Mod },                      // PADDB/r
	{ 0xFD, OPCODE_Bytes2Mod },                      // PADDW/r
	{ 0xFE, OPCODE_Bytes2Mod },                      // PADDD/r
	{ 0xFF, OPCODE_Invalid },                            // _FF
	{ 0, OPCODE_End },
};


BYTE g_ModRM[256] = 
{
	0,0,0,0, OPCODE_SIB|1,OPCODE_RIP|4,0,0, 0,0,0,0, OPCODE_SIB|1,OPCODE_RIP|4,0,0, // 0x
	0,0,0,0, OPCODE_SIB|1,OPCODE_RIP|4,0,0, 0,0,0,0, OPCODE_SIB|1,OPCODE_RIP|4,0,0, // 1x
	0,0,0,0, OPCODE_SIB|1,OPCODE_RIP|4,0,0, 0,0,0,0, OPCODE_SIB|1,OPCODE_RIP|4,0,0, // 2x
	0,0,0,0, OPCODE_SIB|1,OPCODE_RIP|4,0,0, 0,0,0,0, OPCODE_SIB|1,OPCODE_RIP|4,0,0, // 3x
	1,1,1,1, 2,1,1,1, 1,1,1,1, 2,1,1,1,                 // 4x
	1,1,1,1, 2,1,1,1, 1,1,1,1, 2,1,1,1,                 // 5x
	1,1,1,1, 2,1,1,1, 1,1,1,1, 2,1,1,1,                 // 6x
	1,1,1,1, 2,1,1,1, 1,1,1,1, 2,1,1,1,                 // 7x
	4,4,4,4, 5,4,4,4, 4,4,4,4, 5,4,4,4,                 // 8x
	4,4,4,4, 5,4,4,4, 4,4,4,4, 5,4,4,4,                 // 9x
	4,4,4,4, 5,4,4,4, 4,4,4,4, 5,4,4,4,                 // Ax
	4,4,4,4, 5,4,4,4, 4,4,4,4, 5,4,4,4,                 // Bx
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,                 // Cx
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,                 // Dx
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,                 // Ex
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0                  // Fx
};

//取单条指令的opcode长度
LONG GetOpcodeSize(PBYTE pOpcode)
{
	LONG lRet = 0;
	POPCODEITEM pOpcodeItem = NULL;
	BOOL bAddressOverride = FALSE;
	BOOL bOperandOverride = FALSE;
	BYTE bModRm;
	BYTE bFlags;
	BYTE bSib;
	BOOLEAN bResult = FALSE;

	CONDITION_ASSERT(pOpcode);

	bResult = MmIsAddressValid(pOpcode);
	CONDITION_ASSERT(bResult);

	switch(*pOpcode)
	{
	case 0x66:
		bOperandOverride = TRUE;
		lRet++;
		pOpcode++;
		break;
	case 0x67:
		bAddressOverride = TRUE;
		lRet++;
		pOpcode++;
		break;
	case 0xf0:
	case 0xf2:
	case 0xf3:
	case 0x2e:
	case 0x36:
	case 0x3e:
	case 0x26:
	case 0x64:
	case 0x65:
		lRet++;
		pOpcode++;
		break;
	case 0xf6:
		if ((pOpcode[1] & 0x38) == 0x00)
		{
			OPCODEITEM Temp = {0xf6, OPCODE_Bytes2Mod1};
			pOpcodeItem = &Temp;// TEST BYTE /0
		}
		else
		{
			// DIV /6
			// IDIV /7
			// IMUL /5
			// MUL /4
			// NEG /3
			// NOT /2
			OPCODEITEM Temp = {0xf6, OPCODE_Bytes2Mod};
			pOpcodeItem = &Temp;
		}
		break;
	case 0xf7:
		if ((pOpcode[1] & 0x38) == 0x00)
		{
			OPCODEITEM Temp = {0xf6, OPCODE_Bytes2ModOperand};
			pOpcodeItem = &Temp;// TEST BYTE /0
		}
		else
		{
			// DIV /6
			// IDIV /7
			// IMUL /5
			// MUL /4
			// NEG /3
			// NOT /2
			OPCODEITEM Temp = {0xf6, OPCODE_Bytes2Mod};
			pOpcodeItem = &Temp;
		}
		break;
	case 0x0f:
		lRet++;
		pOpcode++;

		pOpcodeItem = &g_opcode0FTable[pOpcode[0]];
		break;
	default:
		break;
	}

	if (!pOpcodeItem)
		pOpcodeItem = &g_OpcodeTable[*pOpcode];

	lRet += (pOpcodeItem->nFlagBits & OPCODE_ADDRESS)
		? (bAddressOverride ? pOpcodeItem->nFixedSize16 : pOpcodeItem->nFixedSize)
		: (bOperandOverride ? pOpcodeItem->nFixedSize16 : pOpcodeItem->nFixedSize);

	if (pOpcodeItem->nModOffset > 0) 
	{
		bModRm = pOpcode[pOpcodeItem->nModOffset];
		bFlags = g_ModRM[bModRm];

		lRet += bFlags & OPCODE_NOTSIB;

		if (bFlags & OPCODE_SIB) 
		{
			bSib = pOpcode[pOpcodeItem->nModOffset + 1];

			if ((bSib & 0x07) == 0x05) 
			{
				if ((bModRm & 0xc0) == 0x00) 
				{
					lRet += 4;
				}
				else if ((bModRm & 0xc0) == 0x40) 
				{
					lRet += 1;
				}
				else if ((bModRm & 0xc0) == 0x80) 
				{
					lRet += 4;
				}
			}
		}
		else if (bFlags & OPCODE_RIP) 
		{
		}
	}

Exit0:
	return lRet;
}