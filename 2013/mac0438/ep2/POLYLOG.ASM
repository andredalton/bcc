;
;  This code is Copyright 1997-1998 by Colin Percival.
;
;  This code may be copied, modified, and used for non-commercial purposes
;  only.  If source code to a program using this code is made publicly available, 
;  it must include a valid copyright notice.
;

;
; Shell for polylog functions -- calls right versions based upon CPU.
;
; Silly jne/jmp code is because the stupid assembler compiles the conditional jump (je)
; with an 8 bit offset, which is incorrect.

.387
.586

_POWMODDATA SEGMENT USE32 PARA PUBLIC 'BSS'     ;internal variables used by
DGROUP GROUP _POWMODDATA                        ;powmod.
align 16
fc100   dd      100.
fc5     dd      5.
f32     dd      0,041F00000H
f64     dd      0,043F00000H
f96     dd      0,045F00000H
f128    dd      0,047F00000H
calcfractemp dd ?
_POWMODDATA ENDS


_TEXT32 SEGMENT USE32 PUBLIC 'CODE'
EXTRN _overheat_err:DWORD

EXTRN _where:QWORD
EXTRN _m_start_pos:QWORD
EXTRN _progress:QWORD
EXTRN _m_end_pos:QWORD
EXTRN _fracdone:DWORD
EXTRN _numclocks:DWORD

EXTRN _CPUVENDOR:DWORD
EXTRN _CPUTYPE:BYTE
CPU_INTEL EQU 0
CPU_AMD EQU 1
CPU_CYRIX EQU 2
CPU_IDT EQU 3
CPU_UNKNOWN EQU -1

EXTRN POWERINIT_P5:PROC
EXTRN CALC_THREAD_DOWORK_P5:PROC
EXTRN POLYLOGCONVOUT_P5:PROC

EXTRN POWERINIT_P6:PROC
EXTRN CALC_THREAD_DOWORK_P6:PROC
EXTRN POLYLOGCONVOUT_P6:PROC

PUBLIC POWERINIT
POWERINIT:
cmp _CPUVENDOR,CPU_INTEL
jne PI_NI
cmp byte _CPUTYPE,5
jne PI_NI
jmp POWERINIT_P5                 ; Intel Pentium use Pentium code
PI_NI:                          ; Everything else use P6 code
jmp POWERINIT_P6

PUBLIC CALC_THREAD_DOWORK
CALC_THREAD_DOWORK:
cmp _CPUVENDOR,CPU_INTEL
jne CT_NI
cmp byte _CPUTYPE,5
jne CT_NI
jmp CALC_THREAD_DOWORK_P5        ; Intel Pentium use Pentium code
CT_NI:                          ; Everything else use P6 code
jmp CALC_THREAD_DOWORK_P6

PUBLIC POLYLOGCONVOUT
POLYLOGCONVOUT:
cmp _CPUVENDOR,CPU_INTEL
jne PL_NI
cmp byte _CPUTYPE,5
jne PL_NI
jmp POLYLOGCONVOUT_P5            ; Intel Pentium use Pentium code
PL_NI:                          ; Everything else use P6 code
jmp POLYLOGCONVOUT_P6

PUBLIC calcfracdone_
calcfracdone_:
        fild qword _progress
        fild qword _m_start_pos
        fld st(0)
        fxch st(2)
        fsubr
        fxch st(1)
        fild qword _m_end_pos
        fsubr
        fdiv
        fmul dword fc100
        fstp dword _fracdone
        fild qword _where
        fild qword _progress
        fmul dword fc5
        fsub
        fstp dword calcfractemp
        mov eax,calcfractemp
        shr eax,23
        sub eax,127
        imul eax,109011
        add eax,108409
        shr eax,8
        mov _numclocks,eax
        retn

        _TEXT32 ENDS
        END

