;
;  CPUID.ASM
;  Contains code taken from cpuid3a.asm.
;
;  Original copyright notice follows:
;
; Filename: cpuid3a.asm
; Copyright 1993, 1994, 1995, 1996, 1997 by Intel Corp.
;
; This program has been developed by Intel Corporation. Intel
; has various intellectual property rights which it may assert
; under certain circumstances, such as if another
; manufacturer’s processor mis-identifies itself as being
; "GenuineIntel" when the CPUID instruction is executed.
;
; Intel specifically disclaims all warranties, express or
; implied, and all liability, including consequential and other
; indirect damages, for the use of this program, including
; liability for infringement of any proprietary rights,
; and including the warranties of merchantability and fitness
; for a particular purpose. Intel does not assume any
; responsibility for any errors which may appear in this program
; nor any responsibility to update it.

.586
_CPUIDDATA SEGMENT USE32 PARA PUBLIC 'BSS'
DGROUP GROUP _CPUIDDATA

PUBLIC _CPUTYPE
PUBLIC _CPUFEATURE
PUBLIC _VENDOR
PUBLIC _CPUVENDOR

_CPUTYPE db ?
_CPUFEATURE dd ?
_CPUVENDOR dd ?
_VENDOR db 13 dup(?)
_CPUIDDATA ENDS


_TEXT32 SEGMENT USE32 PUBLIC 'CODE'
PUBLIC CPUID_PROC
CPUID_PROC:

check_80386:
pushfd                  ; push original EFLAGS
pop eax                 ; get original EFLAGS
mov ecx, eax            ; save original EFLAGS
xor eax, 40000h         ; flip AC bit in EFLAGS
push eax                ; save new EFLAGS value on stack
popfd                   ; replace current EFLAGS value
pushfd                  ; get new EFLAGS
pop eax                 ; store new EFLAGS in EAX
xor eax, ecx            ; can’t toggle AC bit, processor=80386
mov _CPUTYPE, 3          ; turn on 80386 processor flag
jz end_cpu_type         ; jump if 80386 processor
push ecx
popfd                   ; restore AC bit in EFLAGS first

check_80486:
mov _CPUTYPE, 4          ; turn on 80486 processor flag
mov eax, ecx            ; get original EFLAGS
xor eax, 200000h        ; flip ID bit in EFLAGS
push eax                ; save new EFLAGS value on stack
popfd                   ; replace current EFLAGS value
pushfd                  ; get new EFLAGS
pop eax                 ; store new EFLAGS in EAX
xor eax, ecx            ; can’t toggle ID bit,
je end_cpu_type         ; processor=80486

push ebx                ; save registers
push esi
push edi
mov eax, 0              ; set up for CPUID instruction
db 0fh                  ; Hardcoded CPUID instruction
db 0a2h
mov dword ptr _VENDOR, ebx
mov dword ptr _VENDOR[+4], edx
mov dword ptr _VENDOR[+8], ecx
mov byte ptr _VENDOR+12,0

mov eax, 1
db 0fh                  ; Hardcoded CPUID instruction
db 0a2h

shr eax, 8              ; isolate family
and eax, 0fh
mov _CPUTYPE, al        ; set _cpu_type with family
mov _CPUFEATURE,edx     ; set feature flags

pop edi
pop esi
pop ebx

end_cpu_type:
ret

_TEXT32 ENDS
END

