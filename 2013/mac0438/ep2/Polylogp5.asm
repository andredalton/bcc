;
;  This code is Copyright 1997-1998 by Colin Percival.
;
;  This code may be copied, modified, and used for non-commercial purposes
;  only.  If source code to a program using this code is made publicly available, 
;  it must include a valid copyright notice.
;

;
; Based upon powermod.asm, which computes 2^power mod d and 2^power mod w.
;

.387
.586

_result  EQU     [edi-64]
_d       EQU     [edi]
_dinc    EQU     [edi+8]
amul    EQU     [edi+12]
_w       EQU     [edi+16]
_winc    EQU     [edi+24]
xmul    EQU     [edi+28]
_power   EQU     [edi+32]
_powerinc EQU    [edi+40]
_count  EQU     [edi+44]
_dhi     EQU     [edi+48]
_whi     EQU     [edi+56]
result_a EQU     [edi+64]
d0      EQU     qword [edi+80]
d1      EQU     qword [edi+88]
w0      EQU     qword [edi+96]
w1      EQU     qword [edi+104]
fstart  EQU     dword [edi+112]
fmstart EQU     dword [edi+116]
pow     EQU     tbyte [edi+128]
lut1    EQU     dword [edi+144]
lut2    EQU     dword [edi+192]
_POWMODDATA SEGMENT USE32 PARA PUBLIC 'BSS'     ;internal variables used by
DGROUP GROUP _POWMODDATA                        ;powmod.

align 16
fpowerr2 dd     164 dup(?)        ;fpowerr2[4*x]=2^(95-x)
f0 EQU fpowerr2+95*4
f2 EQU fpowerr2+93*4
f63 EQU fpowerr2+32*4
f95 EQU fpowerr2
f31 EQU fpowerr2+64*4
fm1 EQU fpowerr2+96*4
fm33 EQU fpowerr2+128*4
f3power2 dd     128 dup(?)       ;f3power2[4*x]=1.5*(127-x)
fpower2 dd      128 dup(?)       ;fpower2[4*x]=2^x

f32     dq      ?
f64     dq      ?
f63a    dq      ?
f96     dq      ?
f128    dq      ?

_POWMODDATA ENDS


_TEXT32 SEGMENT USE32 PUBLIC 'CODE'
EXTRN _overheat_err:DWORD

EXTRN _where:QWORD
EXTRN _m_start_pos:QWORD
EXTRN _progress:QWORD
EXTRN _m_end_pos:QWORD
EXTRN _fracdone:DWORD
EXTRN _numclocks:DWORD

PUBLIC POLYLOGCONVOUT_P5
POLYLOGCONVOUT_P5:
push edi
mov edi,[esp+8]
fld qword _result+24            ;1u r3
fmul qword f128                 ;2u R3
fld qword _result+16            ;3u r2 R3
fmul qword f96                  ;4u R2 R3
fld qword _result+8             ;5u r1 R2 R3
fmul qword f64                  ;6u R1 R2 R3
fxch st(1)                      ;6v R2 R1 R3
fld qword _result               ;7u r0 R2 R1 R3
fmul qword f32                  ;8u R0 R2 R1 R3
fxch st(3)                      ;8v R3 R2 R1 R0
fstp qword _result+24           ;9-10 R2 R1 R0
fstp qword _result+16           ;11-12 R1 R0
fstp qword _result+8            ;13-14 R0
fstp qword _result              ;15-16
pop edi
ret 4


PUBLIC POWERINIT_P5
POWERINIT_P5:
        mov eax,0x6F000000
        mov ecx,164
        mov ebx,offset fpowerr2
l11:
        mov [ebx],eax
        sub eax,0x00800000
        add ebx,4
        dec ecx
        jnz l11

        mov eax,0x3FC00000
        mov ecx,128
        mov ebx,offset f3power2+4*127
l12:
        mov [ebx],eax
        add eax,0x00800000
        sub ebx,4
        dec ecx
        jnz l12

        mov eax,0x3F800000
        mov ecx,128
        mov ebx,offset fpower2
l13:
        mov [ebx],eax
        add eax,0x00800000
        add ebx,4
        dec ecx
        jnz l13

        fld fpower2+32*4
        fst qword f32
        fld st(0)
        fmul st(0),st(1)
        
        fld dword fcp75
        fmul st(0),st(1)
        fstp qword f63a
        
        fst qword f64
        fmul st(0),st(1)
        fst qword f96
        fmulp st(1),st(0)
        fstp qword f128

        ret

PUBLIC POWERFUNC_P5

POWERFUNC_P5:

push esi
push edi
push ebp
mov edi,[esp+16]
mov dword _dhi,0
mov dword _whi,0
mov eax,_d+4
mov ebx,_w+4
mov dword _dhi+4,eax
mov dword _whi+4,ebx
mov dword pow+8,0

mainloop:

fild qword _w           ;1u     w
fild qword _d           ;2u     d w
fild qword _power       ;3u     power d w
fild qword _dhi         ;4u     d1 power d w
fild qword _whi         ;5u     w1 d1 power d w
fxch st(2)              ;6u     power d1 w1 d w         (fild does not pair)
fstp tbyte pow          ;7-9u   d1 w1 d w
fst qword d1            ;10-11u d1 w1 d w
fsubr st(0),st(2)       ;12u    d0 w1 d w
fxch st(1)              ;12v    w1 d0 d w
fst qword w1            ;13-14u w1 d0 d w
fsubr st(0),st(3)       ;15u    w0 d0 d w
fxch st(1)              ;15v    d0 w0 d w
fstp qword d0           ;16-17u w0 d w
                        ;18     STALL                   (fstp)
fstp qword w0           ;19-20u d w
fld f0                  ;21u    1 d w
fxch st(2)              ;21v    w d 1
fdiv st(2),st(0)        ;22[-60] w d 1/w
;qword pow=abcdefgh ijklmnop qrstuvwx yzABCDEF GHIJKLMN OPQRST00 000...
;word pow+8=3FFE+002E
mov eax,dword pow       ;23u    eax=GHIJKLMN ...
mov edx,dword pow+4     ;23v    edx=abcdefgh ...
shr eax,2               ;24u    eax=00GHIJKL ...
mov esi,edx             ;24v    esi=abcdefgh ...
shl esi,30              ;25u    esi=EF000000 ...
mov ecx,dword pow+8     ;25v    ecx=3FFE+002E
or eax,esi              ;26u    eax=EFGHIJKL ...
sub ecx,0x00003FFE      ;26v    ecx=002E
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Check for FPU errors/overheating here. if ecx is not within reasonable bounds, raise
; error.

cmp ecx,64
jbe not_overheat

mov _overheat_err, 1
jmp exit_powmod

not_overheat:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mov ebx,0x3F800000      ;27u    ebx=(float) s=0,e=127,f=0; =1.0
mov esi,edx             ;27v    esi=abcdefgh ...
cmp ecx,7               ;28u
jnge l4                 ;28v
shr esi,3               ;29u    esi=000abcde fghi...
sub ecx,6               ;29v    ecx=0028
shl edx,6               ;30u    edx=ghijkl mnopqr stuvwx yzABCD EF0000 00
and esi,0x1F800000      ;30v    esi=000abcde f000...
add ebx,esi             ;31u    ebx=0 (7F+00abcdef) 00000... =(2.0)^(abcdef)
mov esi,edx             ;31v    esi=ghijkl ...
l4:
mov fstart,ebx          ;32u
xor ebx,0x7F800000      ;32v    ebx=0 (80-00abcdef) 00000...
sub ebx,0x00800000      ;33u    ebx=0 (7F-00abcdef) 00000... =(2.0)^(-abcdef)
mov ebp,edx             ;33v    ebp=ghijkl mnopqr stuvwx yzABCD EF0000 00       
mov fmstart,ebx         ;34u    fmstart=1/fstart
mov ebx,0               ;34v    ebx=0
cmp ecx,0x018           ;35u    flags='>'
jbe l5                  ;35v

;ebp=edx=esi=ghijkl mnopqr stuvwx yzABCD EF0000 00
;ebx=0

and esi,0xF8000000      ;36u    esi=ghijk000...
and ebp,0x04000000      ;36v    ebp=00000l00...
shl edx,6               ;37u    edx=mnopqr stuvwx yzABCD EF0000 000000 00
or esi,0x04000000       ;37v    esi=ghijk100...
shr ebp,26              ;38u    ebp=l
mov lut1[ebx*4],esi     ;38v
mov lut2[ebx*4],ebp     ;39u
mov esi,edx             ;39v    esi=mnopqr...
mov ebp,edx             ;40u    ebp=mnopqr...
inc ebx                 ;40v
and esi,0xF8000000      ;41u    esi=mnopq000...
and ebp,0x04000000      ;41v    ebp=00000r00...
shl edx,6               ;42u    edx=stuvwx yzABCD EF0000 000000 00...
or esi,0x04000000       ;42v    esi=mnopq100...
shr ebp,26              ;43u    ebp=r
mov lut1[ebx*4],esi     ;43v
mov lut2[ebx*4],ebp     ;44u
mov esi,edx             ;44v    esi=stuvwx...
mov ebp,edx             ;45u    ebp=stuvwx...
inc ebx                 ;45v
and esi,0xF8000000      ;46u    esi=stuvw000...
and ebp,0x04000000      ;46v    ebp=00000x00...
shl edx,6               ;47u    edx=yzABCD EF0000 000000 00...
or esi,0x04000000       ;47v    esi=stuvw100...
shr ebp,26              ;48u    ebp=x
mov lut1[ebx*4],esi     ;48v
mov lut2[ebx*4],ebp     ;49u
mov esi,edx             ;49v    esi=yzABCD...
mov ebp,edx             ;50u    ebp=yzABCD...
inc ebx                 ;50v
and esi,0xF8000000      ;51u    esi=yzABC000...
and ebp,0x04000000      ;51v    ebp=00000D00...
shl edx,6               ;52u    edx=EF0000 000000 00...
or esi,0x04000000       ;52v    esi=yzABC100...
shr ebp,26              ;53u    ebp=D
mov lut1[ebx*4],esi     ;53v
mov lut2[ebx*4],ebp     ;54u
mov esi,edx             ;54v    esi=EF0000...
mov ebp,edx             ;55u    ebp=EF0000...
inc ebx                 ;55v

sub ecx,0x018           ;56u    ecx=0010
mov edx,eax             ;56v    edx=EFGHIJ KLMNOP QRST00 000...
mov esi,edx             ;57u    esi=EFGHIJ KLMNOP QRST00 000...
mov ebp,edx             ;57v    ebp=EFGHIJ KLMNOP QRST00 000...

l5:
                        ;58     Stall, while we wait for the fpu to be ready.

fld dword fstart        ;59     st w d 1/w
fxch st(2)              ;       d w st 1/w
fdiv st(2),st(0)        ;60[-98] d w st/d 1/w

sub ecx,6               ;61u
jbe l6                  ;61v    If there at most 6 bits left, skip this.
l7:
and esi,0xF8000000      ;1u     esi=EFGHI000...
and ebp,0x04000000      ;1v     ebp=00000J00...
shl edx,6               ;2u     edx=KLMNOP QRST00...
or esi,0x04000000       ;2v     esi=EFGHI1...
shr ebp,26              ;3u     ebp=J
mov lut1[ebx*4],esi     ;3v
mov lut2[ebx*4],ebp     ;4u
mov esi,edx             ;4v     esi=KLMNOP...
mov ebp,edx             ;5u     ebp=KLMNOP...
inc ebx                 ;5v
sub ecx,6               ;6u
jnbe l7                 ;6v     If there are more than 6 bits left, repeat.

                        ;61+6*4=85
l6:

add ecx,6               ;86u
mov eax,1               ;86v    eax=1
ror eax,cl              ;87-90  eax=0...0100...
and esi,0xF8000000      ;91u    esi=U...Y[Z]000...
and ebp,eax             ;91v    ebp=0...0Z000...
or esi,eax              ;92u    esi=U...Y100...
add ebp,-1              ;92v    cf=Z
adc ebp,ebp             ;93u    ebp=?????...???Z
mov lut1[ebx*4],esi     ;93v
inc ebx                 ;94u
and ebp,1               ;94v    ebp=Z
sub ecx,ecx             ;95u    ecx=0
sub esi,esi             ;95v    esi=0
mov lut2-4[ebx*4],ebp   ;96u
sub edx,edx             ;96u    edx=0

;lut1=ghijk100...,mnopq100...,stuvw100...,yzABC100...,
;     EFGHI100...,KLMNO100...,QRS10000...
;lut2=l,r,x,D,J,P,T
;ebx=7

fld f63                 ;97u    f63 d w st/d 1/w
fld fstart              ;98u    st f63 d w st/d 1/w
fmul st(0),st(5)        ;99u    st/w f63 d w st/d 1/w
fxch st(4)              ;99v    st/d f63 d w st/w 1/w
fadd st(1),st(0)        ;100u   st/d st/d+ d w st/w 1/w
fmul fmstart            ;101u   1/d st/d+ d w st/w 1/w
fxch st(4)              ;101v   st/w st/d+ d w 1/d 1/w
fadd f63                ;102u   st/w+ st/d+ d w 1/d 1/w
fxch st(1)              ;102v   st/d+ st/w+ d w 1/d 1/w
fsub f63                ;103u   [st/d] st/w+ d w 1/d 1/w
fxch st(1)              ;103v   st/w+ [st/d] d w 1/d 1/w
                        ;104    STALL           (waiting for data, nothing)
                        ;                       (needs loading).
fsub f63                ;105u   [st/w] [st/d] d w 1/d 1/w
fxch st(1)              ;105v   [st/d] [st/w] d w 1/d 1/w
fmulp st(2),st(0)       ;106u   [st/w] strd w 1/d 1/w
fld st(3)               ;107u   1/d [st/w] strd w 1/d 1/w
fxch st(3)              ;107v   w [st/w] strd 1/d 1/d 1/w
fmulp st(1),st(0)       ;108u   strw strd 1/d 1/d 1/w
fxch st(1)              ;108v   strd strw 1/d 1/d 1/w
fsubr fstart            ;109u   a strw 1/d 1/d 1/w
fld f95                 ;110u   f95 a strw 1/d 1/d 1/w
fxch st(2)              ;110v   strw a f95 1/d 1/d 1/w
fsubr fstart            ;111u   x a f95 1/d 1/d 1/w
fxch st(3)              ;111v   1/d a f95 x 1/d 1/w
fmul st(0),st(1)        ;112u   a/d a f95 x 1/d 1/w
fxch st(2)              ;112v   f95 a a/d x 1/d 1/w
fadd st(0),st(1)        ;113u   a1+ a a/d x 1/d 1/w
fxch st(2)              ;113v   a/d a a1+ x 1/d 1/w
fxch st(1)              ;114u   a a/d a1+ x 1/d 1/w     (would stall anyway)
fmul st(1),st(0)        ;115u   a a^2/d a1+ x 1/d 1/w
fxch st(2)              ;115v   a1+ b a x 1/d 1/w
fsub f95                ;116u   a1 b a x 1/d 1/w
fld f63                 ;117u   f63 a1 b a x 1/d 1/w
fadd st(0),st(2)        ;118u   b+ a1 b a x 1/d 1/w
fxch st(1)              ;118v   a1 b+ b a x 1/d 1/w
fsub st(3),st(0)        ;119u   a1 b+ b a0 x 1/d 1/w
fxch st(2)              ;119v   b b+ a1 a0 x 1/d 1/w
fadd f95                ;120u   b1+ b+ a1 a0 x 1/d 1/w
fxch st(1)              ;120v   b+ b1+ a1 a0 x 1/d 1/w
fsub f63                ;121u   b0+b1 b1+ a1 a0 x 1/d 1/w
fxch st(2)              ;121v   a1 b1+ b0+b1 a0 x 1/d 1/w
fmul st(0),st(3)        ;122u   A10 b1+ b0+b1 a0 x 1/d 1/w
fxch st(1)              ;122v   b1+ A10 b0+b1 a0 x 1/d 1/w
fsub f95                ;123u   b1 A10 b0+b1 a0 x 1/d 1/w
fxch st(3)              ;123v   a0 A10 b0+b1 b1 x 1/d 1/w
fmul st(0),st(0)        ;124u   A00 A10 b0+b1 b1 x 1/d 1/w
fxch st(1)              ;124v   A10 A00 b0+b1 b1 x 1/d 1/w
fadd st(0),st(0)        ;125u   2A10 A00 b0+b1 b1 x 1/d 1/w
fxch st(3)              ;125v   b1 A00 b0+b1 2A10 x 1/d 1/w
fsub st(2),st(0)        ;126u   b1 A00 b0 2A10 x 1/d 1/w

;qword pow=abcdefgh ijklmnop qrstuvwx yzABCDEF GHIJKLMN OPQRST00 000...
;fstartpow=2^(abcdef)=0 (7F+abcdef)8 000...=03F800000+000abcdef000...
;MAKE lut1, lut2 be 12 dup ?    (NOT 8 dup ? or 16 dup ?)
;lut1=ghijk100...,mnopq100...,stuvw100...,yzABC100...,
;     EFGHI100...,KLMNO100...,QRS10000...
;lut2=l,r,x,D,J,P,T
;ebx=7
;edi=@polylogdat
;FPU=   b1 A00 b0 2A10 x 1/d 1/w
;x=2&^abcdef;a=2&^abcdef

l2:
;FPU=   b1/s A00/s b0/s 2A10/s x/s1 1/d 1/w
;x=2&^(abcdef ghijkl);a=2&^(abcdef ghijkl)
mov eax,lut1[ecx*4]     ;1u     eax=ghijk100...
mov ebp,lut2[ecx*4]     ;1v     ebp=l,r,x,D,J,P,T
add eax,eax             ;2u     eax=hijk1000...         cf=g
je l3                   ;2v     if eax was 1000... then loop executes 0 times
l1:
;edx=0,g,g,ghi,ghij
adc esi,esi             ;1u     esi=g,gh,ghi,ghij,ghijk
add eax,eax             ;1v     eax=ijk1,jk1,k1,1,0
;s EQU 2^(edx*2)
;s1 EQU 2^edx
;t EQU 2^(esi*2)
;t1 EQU 2^esi
fmul d0                 ;2u     B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fld d0                  ;3u     d0 B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fmul st(0),st(3)        ;4u     B00/s B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fxch st(1)              ;4v     B10/s B00/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fsubp st(4),st(0)       ;5u     B00/s A00/s b0/s C10+A01/s x/s1 1/d 1/w
fxch st(2)              ;5v     b0/s A00/s B00/s C10+A01/s x/s1 1/d 1/w
fmul d1                 ;6u     B01/s A00/s B00/s C10+A01/s x/s1 1/d 1/w
fxch st(2)              ;6v     B00/s A00/s B01/s C10+A01/s x/s1 1/d 1/w
fsubp st(1),st(0)       ;7u     C00/s B01/s C10+A01/s x/s1 1/d 1/w
fld st(3)               ;8u     x/s1 C00/s B01/s C10+A01/s x/s1 1/d 1/w
fxch st(2)              ;8v     B01/s C00/s x/s1 C10+A01/s x/s1 1/d 1/w
fsubp st(3),st(0)       ;9u     C00/s x/s1 C10+C01/s x/s1 1/d 1/w
fld st(0)               ;10u    C00/s C00/s x/s1 C10+C01/s x/s1 1/d 1/w
fxch st(2)              ;10v    x/s1 C00/s C00/s C10+C01/s x/s1 1/d 1/w
fmul st(0),st(0)        ;11u    x^2/s C00/s C00/s C10+C01/s x/s1 1/d 1/w
fxch st(1)              ;11v    C00/s x^2/s C00/s C10+C01/s x/s1 1/d 1/w
fadd st(0),st(3)        ;12u    C-C11/s x^2/s C00/s C10+C01/s x/s1 1/d 1/w
fxch st(4)              ;12v    x/s1 x^2/s C00/s C10+C01/s C-C11/s 1/d 1/w
fld fpowerr2[edx*4]     ;13u    f95/s1 x/s1 x^2/s C00/s C10+C01/s C-C11/s 1/d 1/w
fxch st(2)              ;13v    x^2/s x/s1 f95x/s1 C00/s C10+C01/s C-C11/s 1/d 1/w
fmul st(0),st(7)        ;14u    x^2/w/s x/s1 f95x/s1 C00/s C10+C01/s C-C11/s 1/d 1/w
fxch st(5)              ;14v    C-C11/s x/s1 f95x/s1 C00/s C10+C01/s x^2/w/s 1/d 1/w
fadd f3power2[8*edx]    ;15u    C2+/s x/s1 f95x/s1 C00/s C10+C01/s x^2/w/s 1/d 1/w
fxch st(2)              ;15v    f95x/s1 x/s1 C2+/s C00/s C10+C01/s x^2/w/s 1/d 1/w
fadd st(0),st(1)        ;16u    x1+/s1 x/s1 C2+/s C00/s C10+C01/s x^2/w/s 1/d 1/w
fxch st(5)              ;16v    x^2/w/s x/s1 C2+/s C00/s C10+C01/s x1+/s1 1/d 1/w
fadd f63[8*edx]         ;17u    y+/s x/s1 C2+/s C00/s C10+C01/s x1+/s1 1/d 1/w
fxch st(2)              ;17v    C2+/s x/s1 y+/s C00/s C10+C01/s x1+/s1 1/d 1/w
fsub f3power2[8*edx]    ;18u    C2hi/s x/s1 y+/s C00/s C10+C01/s x1+/s1 1/d 1/w
fxch st(5)              ;18v    x1+/s1 x/s1 y+/s C00/s C10+C01/s C2hi/s 1/d 1/w
fsub fpowerr2[4*edx]    ;19u    x1/s1 x/s1 y+/s C00/s C10+C01/s C2hi/s 1/d 1/w
fxch st(2)              ;19v    y+/s x/s1 x1/s1 C00/s C10+C01/s C2hi/s 1/d 1/w
fsub f63[8*edx]         ;20u    y0+y1/s x/s1 x1/s1 C00/s C10+C01/s C2hi/s 1/d 1/w
fxch st(5)              ;20v    C2hi/s x/s1 x1/s1 C00/s C10+C01/s y0+y1/s 1/d 1/w
fsubp st(4),st(0)       ;21u    x/s1 x1/s1 C00/s C1/s y0+y1/s 1/d 1/w
fsub st(0),st(1)        ;22u    x0/s1 x1/s1 C00/s C1/s y0+y1/s 1/d 1/w
fxch st(2)              ;22v    C00/s x1/s1 x0/s1 C1/s y0+y1/s 1/d 1/w
fld fpowerr2[8*edx]     ;23u    f95/s C00/s x1/s1 x0/s1 C1/s y0+y1/s 1/d 1/w
fxch st(5)              ;23v    y0+y1/s C00/s x1/s1 x0/s1 C1/s f95/s 1/d 1/w
fadd st(5),st(0)        ;24u    y0+y1/s C00/s x1/s1 x0/s1 C1/s y1+/s 1/d 1/w
fxch st(1)              ;24v    C00/s y0+y1/s x1/s1 x0/s1 C1/s y1+/s 1/d 1/w
faddp st(4),st(0)       ;25u    y0+y1/s x1/s1 x0/s1 C/s y1+/s 1/d 1/w
                        ;       y0+y1/s x1/s1 x0/s1 a/t1 y1+/s 1/d 1/w
fxch st(1)              ;25v    x1/s1 y0+y1/s x0/s1 a/t1 y1+/s 1/d 1/w
fmul st(0),st(2)        ;26u    X10/s y0+y1/s x0/s1 a/t1 y1+/s 1/d 1/w
fxch st(4)              ;26v    y1+/s y0+y1/s x0/s1 a/t1 X10/s 1/d 1/w
fsub fpowerr2[8*edx]    ;27u    y1/s y0+y1/s x0/s1 a/t1 X10/s 1/d 1/w
fxch st(2)              ;27v    x0/s1 y0+y1/s y1/s a/t1 X10/s 1/d 1/w
fmul st(0),st(0)        ;28u    X00/s y0+y1/s y1/s a/t1 X10/s 1/d 1/w
fxch st(4)              ;28v    X10/s y0+y1/s y1/s a/t1 X00/s 1/d 1/w
fadd st(0),st(0)        ;29u    2X10/s y0+y1/s y1/s a/t1 X00/s 1/d 1/w
fxch st(1)              ;29v    y0+y1/s 2X10/s y1/s a/t1 X00/s 1/d 1/w
fsub st(0),st(2)        ;30u    y0/s 2X10/s y1/s a/t1 X00/s 1/d 1/w
fxch st(2)              ;30v    y1/s 2X10/s y0/s a/t1 X00/s 1/d 1/w
fmul w0                 ;31u    Y10/s 2X10/s y0/s a/t1 X00/s 1/d 1/w
fld w0                  ;32u    w0 Y10/s 2X10/s y0/s a/t1 X00/s 1/d 1/w
fmul st(0),st(3)        ;33u    Y00/s Y10/s 2X10/s y0/s a/t1 X00/s 1/d 1/w
fxch st(1)              ;33v    Y10/s Y00/s 2X10/s y0/s a/t1 X00/s 1/d 1/w
fsubp st(2),st(0)       ;34u    Y00/s Z10+X01/s y0/s a/t1 X00/s 1/d 1/w
fxch st(2)              ;34v    y0/s Z10+X01/s Y00/s a/t1 X00/s 1/d 1/w
fmul w1                 ;35u    Y01/s Z10+X01/s Y00/s a/t1 X00/s 1/d 1/w
fxch st(2)              ;35v    Y00/s Z10+X01/s Y01/s a/t1 X00/s 1/d 1/w
fsubp st(4),st(0)       ;36u    Z10+X01/s Y01/s a/t1 Z00/s 1/d 1/w
fld st(2)               ;37u    a/t1 Z10+X01/s Y01/s a/t1 Z00/s 1/d 1/w
fxch st(2)              ;37v    Y01/s Z10+X01/s a/t1 a/t1 Z00/s 1/d 1/w
fsubp st(1),st(0)       ;38u    Z10+Z01/s a/t1 a/t1 Z00/s 1/d 1/w
fld st(3)               ;39u    Z00/s Z10+Z01/s a/t1 a/t1 Z00/s 1/d 1/w
fxch st(2)              ;39v    a/t1 Z10+Z01/s Z00/s a/t1 Z00/s 1/d 1/w
fmul st(0),st(0)        ;40u    a^2/t Z10+Z01/s Z00/s a/t1 Z00/s 1/d 1/w
fxch st(2)              ;40v    Z00/s Z10+Z01/s a^2/t a/t1 Z00/s 1/d 1/w
fadd st(0),st(1)        ;41u    Z-Z11/s Z10+Z01/s a^2/t a/t1 Z00/s 1/d 1/w
fld fpowerr2[4*esi]     ;42u    f95/t1 Z-Z11/s Z10+Z01/s a^2/t a/t1 Z00/s 1/d 1/w
fxch st(3)              ;42v    a^2/t Z-Z11/s Z10+Z01/s f95a/t1 a/t1 Z00/s 1/d 1/w
fmul st(0),st(6)        ;43u    a^2/m/t Z-Z11/s Z10+Z01/s f95a/t1 a/t1 Z00/s 1/d 1/w
fxch st(1)              ;43v    Z-Z11/s a^2/m/t Z10+Z01/s f95a/t1 a/t1 Z00/s 1/d 1/w
fadd f3power2[8*edx]    ;44u    Z2+/s a^2/m/t Z10+Z01/s f95a/t1 a/t1 Z00/s 1/d 1/w
fxch st(3)              ;44v    f95a/t1 a^2/m/t Z10+Z01/s Z2+/s a/t1 Z00/s 1/d 1/w
fadd st(0),st(4)        ;45u    a1+/t1 a^2/m/t Z10+Z01/s Z2+/s a/t1 Z00/s 1/d 1/w
fxch st(1)              ;45v    a^2/m/t a1+/t1 Z10+Z01/s Z2+/s a/t1 Z00/s 1/d 1/w
fadd f63[8*esi]         ;46u    b+/t a1+/t1 Z10+Z01/s Z2+/s a/t1 Z00/s 1/d 1/w
fxch st(3)              ;46v    Z2+/s a1+/t1 Z10+Z01/s b+/t a/t1 Z00/s 1/d 1/w
fsub f3power2[8*edx]    ;47u    Z2hi/s a1+/t1 Z10+Z01/s b+/t a/t1 Z00/s 1/d 1/w
fxch st(1)              ;47v    a1+/t1 Z2hi/s Z10+Z01/s b+/t a/t1 Z00/s 1/d 1/w
fsub fpowerr2[4*esi]    ;48u    a1/t1 Z2hi/s Z10+Z01/s b+/t a/t1 Z00/s 1/d 1/w
fxch st(3)              ;48v    b+/t Z2hi/s Z10+Z01/s a1/t1 a/t1 Z00/s 1/d 1/w
fsub f63[8*esi]         ;49u    b0+b1/t Z2hi/s Z10+Z01/s a1/t1 a/t1 Z00/s 1/d 1/w
fxch st(1)              ;49v    Z2hi/s b0+b1/t Z10+Z01/s a1/t1 a/t1 Z00/s 1/d 1/w
fsubp st(2),st(0)       ;50u    b0+b1/t Z1/s a1/t1 a/t1 Z00/s 1/d 1/w
fxch st(2)              ;50v    a1/t1 Z1/s b0+b1/t a/t1 Z00/s 1/d 1/w
fsub st(3),st(0)        ;51u    a1/t1 Z1/s b0+b1/t a0/t1 Z00/s 1/d 1/w
fld fpowerr2[8*esi]     ;52u    f95/t a1/t1 Z1/s b0+b1/t a0/t1 Z00/s 1/d 1/w
fadd st(0),st(3)        ;53u    b1+/t a1/t1 Z1/s b0+b1/t a0/t1 Z00/s 1/d 1/w
fxch st(2)              ;53v    Z1/s a1/t1 b1+/t b0+b1/t a0/t1 Z00/s 1/d 1/w
faddp st(5),st(0)       ;54u    a1/t1 b1+/t b0+b1/t a0/t1 Z/s 1/d 1/w
                        ;       a1/t1 b1+/t b0+b1/t a0/t1 x/t1 1/d 1/w
fmul st(0),st(3)        ;55u    A10/t b1+/t b0+b1/t a0/t1 x/t1 1/d 1/w
fxch st(1)              ;55v    b1+/t A10/t b0+b1/t a0/t1 x/t1 1/d 1/w
fsub fpowerr2[8*esi]    ;56u    b1/t A10/t b0+b1/t a0/t1 x/t1 1/d 1/w
fxch st(3)              ;56v    a0/t1 A10/t b0+b1/t b1/t x/t1 1/d 1/w
fmul st(0),st(0)        ;57u    A00/t A10/t b0+b1/t b1/t x/t1 1/d 1/w
fxch st(1)              ;57v    A10/t A00/t b0+b1/t b1/t x/t1 1/d 1/w
fadd st(0),st(0)        ;58u    2A10/t A00/t b0+b1/t b1/t x/t1 1/d 1/w
fxch st(3)              ;58v    b1/t A00/t b0+b1/t 2A10/t x/t1 1/d 1/w
fsub st(2),st(0)        ;59u    b1/t A00/t b0/t 2A10/t x/t1 1/d 1/w
mov edx,esi             ;60u    edx=g,gh,ghi,ghij,ghijk
jne l1                  ;60v
l3:
mov esi,ebp             ;3u     esi=l,r,x,D,J,P,T
inc ecx                 ;3v     ecx=1,2,3,4,5,6,7
cmp ecx,ebx             ;4u     <,<,<,<,<,<,=
je last                 ;4v

;t=2^(esi*2)
;t1=2^(esi)=2^[l,r,...]
;s=2^[ghijk0,mnopq0,stuvw0,yzABC0,EFGHI0,KLMNO0 (,QRS0)]=2^(edx*2)
;s1=2^[ghijk,mnopq,stuvw,yzABC,EFGHI,KLMNO (,QRS)]=2^edx
;b1/s A00/s b0/s 2A10/s x/s1 1/d 1/w
;FPU=   b1/s A00/s b0/s 2A10/s x/s1 1/d 1/w
;x=2&^abcdefghijk;a=2&^abcdefghijk

fmul d0                 ;5u     B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fld d0                  ;6u     d0 B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fmul st(0),st(3)        ;7u     B00/s B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fxch st(1)              ;7v     B10/s B00/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fsubp st(4),st(0)       ;8u     B00/s A00/s b0/s C10+A01/s x/s1 1/d 1/w
fxch st(2)              ;8v     b0/s A00/s B00/s C10+A01/s x/s1 1/d 1/w
fmul d1                 ;9u     B01/s A00/s B00/s C10+A01/s x/s1 1/d 1/w
fxch st(2)              ;9v     B00/s A00/s B01/s C10+A01/s x/s1 1/d 1/w
fsubp st(1),st(0)       ;10u    C00/s B01/s C10+A01/s x/s1 1/d 1/w
fxch st(3)              ;10v    x/s1 B01/s C10+A01/s C00/s 1/d 1/w
fmul fpower2[edx*4]     ;11u    x B01/s C10+A01/s C00/s 1/d 1/w
fxch st(1)              ;11v    B01/s x C10+A01/s C00/s 1/d 1/w
fsubp st(2),st(0)       ;12u    x C10+C01/s C00/s 1/d 1/w
fxch st(2)              ;12v    C00/s C10+C01/s x 1/d 1/w
fld st(0)               ;13u    C00/s C00/s C10+C01/s x 1/d 1/w
fld st(3)               ;14u    x C00/s C00/s C10+C01/s x 1/d 1/w
fmul st(0),st(0)        ;15u    x^2 C00/s C00/s C10+C01/s x 1/d 1/w
fxch st(1)              ;15v    C00/s x^2 C00/s C10+C01/s x 1/d 1/w
fadd st(0),st(3)        ;16u    C-C11/s x^2 C00/s C10+C01/s x 1/d 1/w
fxch st(4)              ;16v    x x^2 C00/s C10+C01/s C-C11/s 1/d 1/w
fld f95                 ;17u    f95 x x^2 C00/s C10+C01/s C-C11/s 1/d 1/w
fxch st(2)              ;17v    x^2 x f95 C00/s C10+C01/s C-C11/s 1/d 1/w
fmul st(0),st(7)        ;18u    x^2/w x f95 C00/s C10+C01/s C-C11/s 1/d 1/w
fxch st(5)              ;18v    C-C11/s x1 f95 C00/s C10+C01/s x^2/w 1/d 1/w
fadd f3power2[edx*8]    ;19u    C2+/s x f95 C00/s C10+C01/s x^2/w 1/d 1/w
fxch st(2)              ;19v    f95 x C2+/s C00/s C10+C01/s x^2/w 1/d 1/w
fadd st(0),st(1)        ;20u    x1+ x C2+/s C00/s C10+C01/s x^2/w 1/d 1/w
fxch st(5)              ;20v    x^2/w x C2+/s C00/s C10+C01/s x1+ 1/d 1/w
fadd f63                ;21u    y+ x C2+/s C00/s C10+C01/s x1+ 1/d 1/w
fxch st(2)              ;21v    C2+/s x y+ C00/s C10+C01/s x1+ 1/d 1/w
fsub f3power2[edx*8]    ;22u    C2hi/s x y+ C00/s C10+C01/s x1+ 1/d 1/w
fxch st(5)              ;22v    x1+ x y+ C00/s C10+C01/s C2hi/s 1/d 1/w
fsub f95                ;23u    x1 x y+ C00/s C10+C01/s C2hi/s 1/d 1/w
fxch st(2)              ;23v    y+ x x1 C00/s C10+C01/s C2hi/s 1/d 1/w
fsub f63                ;24u    y0+y1 x x1 C00/s C10+C01/s C2hi/s 1/d 1/w
fxch st(5)              ;24v    C2hi/s x x1 C00/s C10+C01/s y0+y1 1/d 1/w
fsubp st(4),st(0)       ;25u    x x1 C00/s C1/s y0+y1 1/d 1/w
fsub st(0),st(1)        ;26u    x0 x1 C00/s C1/s y0+y1 1/d 1/w
fxch st(2)              ;26v    C00/s x1 x0 C1/s y0+y1 1/d 1/w
fld f95                 ;27u    f95 C00/s x1 x0 C1/s y0+y1 1/d 1/w
fxch st(5)              ;27v    y0+y1 C00/s x1 x0 C1/s f95 1/d 1/w
fadd st(5),st(0)        ;28u    y0+y1 C00/s x1 x0 C1/s y1+ 1/d 1/w
fxch st(1)              ;28v    C00/s y0+y1 x1 x0 C1/s y1+ 1/d 1/w
faddp st(4),st(0)       ;39u    y0+y1 x1 x0 C/s y1+ 1/d 1/w
                        ;       y0+y1 x1 x0 a/s/t1 y1+ 1/d 1/w
fxch st(1)              ;39v    x1 y0+y1 x0 a/s/t1 y1+ 1/d 1/w
fmul st(0),st(2)        ;30u    X10 y0+y1 x0 a/s/t1 y1+ 1/d 1/w
fxch st(4)              ;30v    y1+ y0+y1 x0 a/s/t1 X10 1/d 1/w
fsub f95                ;31u    y1 y0+y1 x0 a/s/t1 X10 1/d 1/w
fxch st(2)              ;31v    x0 y0+y1 y1 a/s/t1 X10 1/d 1/w
fmul st(0),st(0)        ;32u    X00 y0+y1 y1 a/s/t1 X10 1/d 1/w
fxch st(4)              ;32v    X10 y0+y1 y1 a/s/t1 X00 1/d 1/w
fadd st(0),st(0)        ;33u    2X10 y0+y1 y1 a/s/t1 X00 1/d 1/w
fxch st(1)              ;33v    y0+y1 2X10 y1 a/s/t1 X00 1/d 1/w
fsub st(0),st(2)        ;34u    y0 2X10 y1 a/s/t1 X00 1/d 1/w
fxch st(2)              ;34v    y1 2X10 y0 a/s/t1 X00 1/d 1/w
fmul w0                 ;35u    Y10 2X10 y0 a/s/t1 X00 1/d 1/w
fld w0                  ;36u    w0 Y10 2X10 y0 a/s/t1 X00 1/d 1/w
fmul st(0),st(3)        ;37u    Y00 Y10 2X10 y0 a/s/t1 X00 1/d 1/w
fxch st(1)              ;37v    Y10 Y00 2X10 y0 a/s/t1 X00 1/d 1/w
fsubp st(2),st(0)       ;38u    Y00 Z10+X01 y0 a/s/t1 X00 1/d 1/w
fxch st(2)              ;38v    y0 Z10+X01 Y00 a/s/t1 X00 1/d 1/w
fmul w1                 ;39u    Y01 Z10+X01 Y00 a/s/t1 X00 1/d 1/w
fxch st(2)              ;39v    Y00 Z10+X01 Y01 a/s/t1 X00 1/d 1/w
fsubp st(4),st(0)       ;40u    Z10+X01 Y01 a/s/t1 Z00 1/d 1/w
fxch st(2)              ;40v    a/s/t1 Y01 Z10+X01 Z00 1/d 1/w
fmul fpower2[edx*8]     ;41u    a/t1 Y01 Z10+X01 Z00 1/d 1/w
fxch st(2)              ;41v    Z10+X01 Y01 a/t1 Z00 1/d 1/w
fsubrp st(1),st(0)      ;42u    Z10+Z01 a/t1 Z00 1/d 1/w
fld st(2)               ;43u    Z00 Z10+Z01 a/t1 Z00 1/d 1/w
fxch st(1)              ;43v    Z10+Z01 Z00 a/t1 Z00 1/d 1/w
fld st(2)               ;44u    a/t1 Z10+Z01 Z00 a/t1 Z00 1/d 1/w
fmul st(0),st(0)        ;45u    a^2/t Z10+Z01 Z00 a/t1 Z00 1/d 1/w
fxch st(2)              ;45v    Z00 Z10+Z01 a^2/t a/t1 Z00 1/d 1/w
fadd st(0),st(1)        ;46u    Z-Z11 Z10+Z01 a^2/t a/t1 Z00 1/d 1/w
fld fpowerr2[esi*4]     ;47u    f95/t1 Z-Z11 Z10+Z01 a^2/t a/t1 Z00 1/d 1/w
fxch st(3)              ;47v    a^2/t Z-Z11 Z10+Z01 f95/t1 a/t1 Z00 1/d 1/w
fmul st(0),st(6)        ;48u    a^2/d/t Z-Z11 Z10+Z01 f95/t1 a/t1 Z00 1/d 1/w
fxch st(1)              ;48v    Z-Z11 a^2/d/t Z10+Z01 f95/t1 a/t1 Z00 1/d 1/w
fadd f3power2           ;49u    Z2+ a^2/d/t Z10+Z01 f95/t1 a/t1 Z00 1/d 1/w
fxch st(3)              ;49v    f95/t1 a^2/d/t Z10+Z01 Z2+ a/t1 Z00 1/d 1/w
fadd st(0),st(4)        ;50u    a1+/t1 a^2/d/t Z10+Z01 Z2+ a/t1 Z00 1/d 1/w
fxch st(1)              ;50v    a^2/d/t a1+/t1 Z10+Z01 Z2+ a/t1 Z00 1/d 1/w
fadd f63[esi*8]         ;51u    b+/t a1+/t1 Z10+Z01 Z2+ a/t1 Z00 1/d 1/w
fxch st(3)              ;51v    Z2+ a1+/t1 Z10+Z01 b+/t a/t1 Z00 1/d 1/w
fsub f3power2           ;52u    Z2hi a1+/t1 Z10+Z01 b+/t a/t1 Z00 1/d 1/w
fxch st(1)              ;52v    a1+/t1 Z2hi Z10+Z01 b+/t a/t1 Z00 1/d 1/w
fsub fpowerr2[esi*4]    ;53u    a1/t1 Z2hi Z10+Z01 b+/t a/t1 Z00 1/d 1/w
fxch st(3)              ;53v    b+/t Z2hi Z10+Z01 a1/t1 a/t1 Z00 1/d 1/w
fsub f63[esi*8]         ;54u    b0+b1/t Z2hi Z10+Z01 a1/t1 a/t1 Z00 1/d 1/w
fxch st(1)              ;54v    Z2hi b0+b1/t Z10+Z01 a1/t1 a/t1 Z00 1/d 1/w
fsubp st(2),st(0)       ;55u    b0+b1/t Z1 a1/t1 a/t1 Z00 1/d 1/w
fxch st(2)              ;55v    a1/t1 Z1 b0+b1/t a/t1 Z00 1/d 1/w
fsub st(3),st(0)        ;56u    a1/t1 Z1 b0+b1/t a0/t1 Z00 1/d 1/w
fld fpowerr2[esi*8]     ;57u    f95/t a1/t1 Z1 b0+b1/t a0/t1 Z00 1/d 1/w
fadd st(0),st(3)        ;58u    b1+/t a1/t1 Z1 b0+b1/t a0/t1 Z00 1/d 1/w
fxch st(2)              ;58v    Z1 a1/t1 b1+/t b0+b1/t a0/t1 Z00 1/d 1/w
faddp st(5),st(0)       ;59u    a1/t1 b1+/t b0+b1/t a0/t1 Z 1/d 1/w
                        ;       a1/t1 b1+/t b0+b1/t a0/t1 x/t1 1/d 1/w
fmul st(0),st(3)        ;60u    A10/t b1+/t b0+b1/t a0/t1 x/t1 1/d 1/w
fxch st(1)              ;60v    b1+/t A10/t b0+b1/t a0/t1 x/t1 1/d 1/w
fsub fpowerr2[esi*8]    ;61u    b1/t A10/t b0+b1/t a0/t1 x/t1 1/d 1/w
fxch st(3)              ;61v    a0/t1 A10/t b0+b1/t b1/t x/t1 1/d 1/w
fmul st(0),st(0)        ;62u    A00/t A10/t b0+b1/t b1/t x/t1 1/d 1/w
fxch st(1)              ;62v    A10/t A00/t b0+b1/t b1/t x/t1 1/d 1/w
fadd st(0),st(0)        ;63u    2A10/t A00/t b0+b1/t b1/t x/t1 1/d 1/w
fxch st(3)              ;63v    b1/t A00/t b0+b1/t 2A10/t x/t1 1/d 1/w
fsub st(2),st(0)        ;64u    b1/t A00/t b0/t 2A10/t x/t1 1/d 1/w
mov edx,esi             ;65u    edx=l,r,x,D,J,P,T
jmp l2                  ;65v

last:

                        ;126+60*inner+5*outer+3 [+10 for 2x mispredicted jcc]
lea esi,[edx*2+esi]     ;139u   esi=QRST
;t=2^QRST0=2^(esi*2)
;t1=2^QRST=2^(esi)
;s=2^QRS0=2^(edx*2)
;s1=2^QRS=2^edx
;b1/s A00/s b0/s 2A10/s x/s1 1/d 1/w
;x=2&^abcdefghijk...OPQRS;a=2&^abcdefghijk...OPQRS

fmul d0                 ;140u   B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fld d0                  ;141u   d0 B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fmul st(0),st(3)        ;142u   B00/s B10/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fxch st(1)              ;142v   B10/s B00/s A00/s b0/s 2A10/s x/s1 1/d 1/w
fsubp st(4),st(0)       ;143u   B00/s A00/s b0/s C10+A01/s x/s1 1/d 1/w
fxch st(2)              ;143v   b0/s A00/s B00/s C10+A01/s x/s1 1/d 1/w
fmul d1                 ;144u   B01/s A00/s B00/s C10+A01/s x/s1 1/d 1/w
fxch st(2)              ;144v   B00/s A00/s B01/s C10+A01/s x/s1 1/d 1/w
fsubp st(1),st(0)       ;145u   C00/s B01/s C10+A01/s x/s1 1/d 1/w
fld st(3)               ;146u   x/s1 C00/s B01/s C10+A01/s x/s1 1/d 1/w
fxch st(2)              ;146v   B01/s C00/s x/s1 C10+A01/s x/s1 1/d 1/w
fsubp st(3),st(0)       ;147u   C00/s x/s1 C10+C01/s x/s1 1/d 1/w
fld st(0)               ;148u   C00/s C00/s x/s1 C10+C01/s x/s1 1/d 1/w
fxch st(2)              ;148v   x/s1 C00/s C00/s C10+C01/s x/s1 1/d 1/w
fmul st(0),st(0)        ;149u   x^2/s C00/s C00/s C10+C01/s x/s1 1/d 1/w
fxch st(1)              ;149v   C00/s x^2/s C00/s C10+C01/s x/s1 1/d 1/w
fadd st(0),st(3)        ;150u   C-C11/s x^2/s C00/s C10+C01/s x/s1 1/d 1/w
fxch st(4)              ;150v   x/s1 x^2/s C00/s C10+C01/s C-C11/s 1/d 1/w
fld fpowerr2[edx*4]     ;151u   f95/s1 x/s1 x^2/s C00/s C10+C01/s C-C11/s 1/d 1/w
fxch st(2)              ;151v   x^2/s x/s1 f95x/s1 C00/s C10+C01/s C-C11/s 1/d 1/w
fmul st(0),st(7)        ;152u   x^2/w/s x/s1 f95x/s1 C00/s C10+C01/s C-C11/s 1/d 1/w
fxch st(5)              ;152v   C-C11/s x/s1 f95x/s1 C00/s C10+C01/s x^2/w/s 1/d 1/w
fadd f3power2[edx*8]    ;153u   C2+/s x/s1 f95x/s1 C00/s C10+C01/s x^2/w/s 1/d 1/w
fxch st(2)              ;153v   f95x/s1 x/s1 C2+/s C00/s C10+C01/s x^2/w/s 1/d 1/w
fadd st(0),st(1)        ;154u   x1+/s1 x/s1 C2+/s C00/s C10+C01/s x^2/w/s 1/d 1/w
fxch st(5)              ;154v   x^2/w/s x/s1 C2+/s C00/s C10+C01/s x1+/s1 1/d 1/w
fadd f63[edx*8]         ;155u   y+/s x/s1 C2+/s C00/s C10+C01/s x1+/s1 1/d 1/w
fxch st(2)              ;155v   C2+/s x/s1 y+/s C00/s C10+C01/s x1+/s1 1/d 1/w
fsub f3power2[edx*8]    ;156u   C2hi/s x/s1 y+/s C00/s C10+C01/s x1+/s1 1/d 1/w
fxch st(5)              ;156v   x1+/s1 x/s1 y+/s C00/s C10+C01/s C2hi/s 1/d 1/w
fsub fpowerr2[edx*4]    ;157u   x1/s1 x/s1 y+/s C00/s C10+C01/s C2hi/s 1/d 1/w
fxch st(2)              ;157v   y+/s x/s1 x1/s1 C00/s C10+C01/s C2hi/s 1/d 1/w
fsub f63[edx*8]         ;158u   y0+y1/s x/s1 x1/s1 C00/s C10+C01/s C2hi/s 1/d 1/w
fxch st(5)              ;158v   C2hi/s x/s1 x1/s1 C00/s C10+C01/s y0+y1/s 1/d 1/w
fsubp st(4),st(0)       ;159u   x/s1 x1/s1 C00/s C1/s y0+y1/s 1/d 1/w
fld fpowerr2[edx*8]     ;160u   f95/s x/s1 x1/s1 C00/s C1/s y0+y1/s 1/d 1/w
fxch st(2)              ;160v   x1/s1 x/s1 f95/s C00/s C1/s y0+y1/s 1/d 1/w
fsub st(1),st(0)        ;161u   x1/s1 x0/s1 f95/s C00/s C1/s y0+y1/s 1/d 1/w
fxch st(2)              ;161v   f95/s x0/s1 x1/s1 C00/s C1/s y0+y1/s 1/d 1/w
fadd st(0),st(5)        ;162u   y1+/s x0/s1 x1/s1 C00/s C1/s y0+y1/s 1/d 1/w
fxch st(3)              ;162v   C00/s x0/s1 x1/s1 y1+/s C1/s y0+y1/s 1/d 1/w
faddp st(4),st(0)       ;163u   x0/s1 x1/s1 y1+/s C/s y0+y1/s 1/d 1/w
fld st(1)               ;164u   x1/s1 x0/s1 x1/s1 y1+/s C/s y0+y1/s 1/d 1/w
fmul st(0),st(1)        ;165u   X10/s x0/s1 x1/s1 y1+/s C/s y0+y1/s 1/d 1/w
fxch st(3)              ;165v   y1+/s x0/s1 x1/s1 X10/s C/s y0+y1/s 1/d 1/w
fsub fpowerr2[edx*8]    ;166u   y1/s x0/s1 x1/s1 X10/s C/s y0+y1/s 1/d 1/w
fxch st(2)              ;166v   x1/s1 x0/s1 y1/s X10/s C/s y0+y1/s 1/d 1/w
fmul st(0),st(0)        ;167u   X11/s x0/s1 y1/s X10/s C/s y0+y1/s 1/d 1/w
fxch st(3)              ;167v   X10/s x0/s1 y1/s X11/s C/s y0+y1/s 1/d 1/w
fadd st(0),st(0)        ;168u   2X10/s x0/s1 y1/s X11/s C/s y0+y1/s 1/d 1/w
fxch st(4)              ;168v   a/t1 x0/s1 y1/s X11/s 2X10/s y0+y1/s 1/d 1/w
fmul fpower2[esi*4]     ;169u   a x0/s1 y1/s X11/s 2X10/s y0+y1/s 1/d 1/w
; a=2&^abcdef...OPQRST
fxch st(2)              ;169v   y1/s x0/s1 a X11/s 2X10/s y0+y1/s 1/d 1/w
fsub st(5),st(0)        ;170u   y1/s x0/s1 a X11/s 2X10/s y0/s 1/d 1/w
fxch st(1)              ;170v   x0/s1 y1/s a X11/s 2X10/s y0/s 1/d 1/w
fmul st(0),st(0)        ;171u   X00/s y1/s a X11/s 2X10/s y0/s 1/d 1/w
fxch st(2)              ;171v   a y1/s X00/s X11/s 2X10/s y0/s 1/d 1/w
                        ;172    STALL (fst requires data one clock ahead)
fstp tbyte result_a     ;173-5  y1/s X00/s X11/s 2X10/s y0/s 1/d 1/w
fld st(0)               ;176u   y1/s y1/s X00/s X11/s 2X10/s y0/s 1/d 1/w
fmul w0                 ;176u   Y10/s y1/s X00/s X11/s 2X10/s y0/s 1/d 1/w
fxch st(1)              ;177v   y1/s Y10/s X00/s X11/s 2X10/s y0/s 1/d 1/w
                        ;178    STALL
fmul w1                 ;179u   Y11/s Y10/s X00/s X11/s 2X10/s y0/s 1/d 1/w
fxch st(1)              ;179v   Y10/s Y11/s X00/s X11/s 2X10/s y0/s 1/d 1/w
fsubp st(4),st(0)       ;180u   Y11/s X00/s X11/s Z10+X01/s y0/s 1/d 1/w
fld st(4)               ;181u   y0/s Y11/s X00/s X11/s Z10+X01/s y0/s 1/d 1/w
fmul w1                 ;182u   Y01/s Y11/s X00/s X11/s Z10+X01/s y0/s 1/d 1/w
fxch st(1)              ;182v   Y11/s Y01/s X00/s X11/s Z10+X01/s y0/s 1/d 1/w
fsubp st(3),st(0)       ;183u   Y01/s X00/s Z11/s Z10+X01/s y0/s 1/d 1/w
fxch st(4)              ;183v   y0/s X00/s Z11/s Z10+X01/s Y01/s 1/d 1/w
fmul w0                 ;184u   Y00/s X00/s Z11/s Z10+X01/s Y01/s 1/d 1/w
fxch st(4)              ;184v   Y01/s X00/s Z11/s Z10+X01/s Y00/s 1/d 1/w
fsubp st(3),st(0)       ;185u   X00/s Z11/s Z10+Z01/s Y00/s 1/d 1/w
fld d1                  ;186u   d1 X00/s Z11/s Z10+Z01/s Y00/s 1/d 1/w
fxch st(1)              ;186v   X00/s d1 Z11/s Z10+Z01/s Y00/s 1/d 1/w
fsubrp st(4),st(0)      ;187u   d1 Z11/s Z10+Z01/s Z00/s 1/d 1/w
fxch st(1)              ;187v   Z11/s d1 Z10+Z01/s Z00/s 1/d 1/w
faddp st(2),st(0)       ;188u   d1 Z-Z00/s Z00/s 1/d 1/w
fld tbyte result_a      ;189-191 a d1 Z-Z00/s Z00/s 1/d 1/w
fmul dword amul         ;192u   a*amul d1 Z-Z00/s Z00/s 1/d 1/w
fxch st(2)              ;192v   Z-Z00/s d1 a*amul Z00/s 1/d 1/w
faddp st(3),st(0)       ;193u   d1 a*amul Z/s 1/d 1/w
fld st(3)               ;194u   1/d d1 a*amul Z/s 1/d 1/w
fmul st(0),st(2)        ;195u   a*amul/d d1 a*amul Z/s 1/d 1/w
fld w1                  ;196u   w1 a*amul/d d1 a*amul Z/s 1/d 1/w
fxch st(4)              ;196v   Z/s a*amul/d d1 a*amul w1 1/d 1/w
fmul fpower2[esi*4]     ;197u   x a*amul/d d1 a*amul w1 1/d 1/w
fxch st(1)              ;197v   a*amul/d x d1 a*amul w1 1/d 1/w
fadd f63a               ;198u   b+ x d1 a*amul w1 1/d 1/w
fld st(6)               ;299u   1/w b+ x d1 a*amul w1 1/d 1/w
fxch st(2)              ;299v   x b+ 1/w d1 a*amul w1 1/d 1/w
fmul dword xmul         ;200u   x*xmul b+ 1/w d1 a*amul w1 1/d 1/w
fxch st(1)              ;200v   b+ x*xmul 1/w d1 a*amul w1 1/d 1/w
fsub f63a               ;201u   b x*xmul 1/w d1 a*amul w1 1/d 1/w
fxch st(1)              ;201v   x*xmul b 1/w d1 a*amul w1 1/d 1/w
                        ;202    STALL
fmul st(2),st(0)        ;203u   x*xmul b x*xmul/w d1 a*amul w1 1/d 1/w
fxch st(1)              ;203v   b x*xmul x*xmul/w d1 a*amul w1 1/d 1/w
                        ;204    STALL
fmul st(3),st(0)        ;205u   b x*xmul x*xmul/w b*d1 a*amul w1 1/d 1/w
fxch st(2)              ;205v   x*xmul/w x*xmul b b*d1 a*amul w1 1/d 1/w
fadd f63a               ;206u   y+ x*xmul b b*d1 a*amul w1 1/d 1/w
fxch st(2)              ;206v   b x*xmul y+ b*d1 a*amul w1 1/d 1/w
fmul d0                 ;207u   b*d0 x*xmul y+ b*d1 a*amul w1 1/d 1/w
fxch st(3)              ;207v   b*d1 x*xmul y+ b*d0 a*amul w1 1/d 1/w
fsubp st(4),st(0)       ;208u   x*xmul y+ b*d0 a*amul-b*d1 w1 1/d 1/w
fxch st(1)              ;208v   y+ x*xmul b*d0 a*amul-b*d1 w1 1/d 1/w
fsub f63a               ;209u   y x*xmul b*d0 a*amul-b*d1 w1 1/d 1/w
fxch st(2)              ;209v   b*d0 x*xmul y a*amul-b*d1 w1 1/d 1/w
                        ;210    STALL
fsubp st(3),st(0)       ;211u   x*xmul y A w1 1/d 1/w
fxch st(1)              ;211v   y x*xmul A w1 1/d 1/w
fmul st(3),st(0)        ;212u   y x*xmul A y*w1 1/d 1/w
fld d1                  ;213u   d1 y x*xmul A y*w1 1/d 1/w
fxch st(1)              ;213v   y d1 x*xmul A y*w1 1/d 1/w
fmul w0                 ;214u   y*w0 d1 x*xmul A y*w1 1/d 1/w
fxch st(4)              ;214v   y*w1 d1 x*xmul A y*w0 1/d 1/w
fsubp st(2),st(0)       ;215u   d1 x*xmul-y*w1 A y*w0 1/d 1/w
fxch st(2)              ;215v   A x*xmul-y*w1 d1 y*w0 1/d 1/w
fld st(0)               ;216u   A A x*xmul-y*w1 d1 y*w0 1/d 1/w
fmul st(0),st(5)        ;217u   A/d A x*xmul-y*w1 d1 y*w0 1/d 1/w
fld qword _result       ;218u   r0 A/d A x*xmul-y*w1 d1 y*w0 1/d 1/w
fxch st(5)              ;218v   y*w0 A/d A x*xmul-y*w1 d1 r0 1/d 1/w
fsubp st(3),st(0)       ;219u   A/d A X d1 r0 1/d 1/w
fadd f31                ;220u   A/d+ A X d1 r0 1/d 1/w
fld st(6)               ;221u   1/w A/d+ A X d1 r0 1/d 1/w
fmul st(0),st(3)        ;222u   X/w A/d+ A X d1 r0 1/d 1/w
fxch st(1)              ;222v   A/d+ X/w A X d1 r0 1/d 1/w
fsub f31                ;223u   A0 X/w A X d1 r0 1/d 1/w
fxch st(1)              ;223v   X/w A0 A X d1 r0 1/d 1/w
                        ;224    STALL
fadd f31                ;225u   X/w+ A0 A X d1 r0 1/d 1/w
fxch st(1)              ;225v   A0 X/w+ A X d1 r0 1/d 1/w
fmul st(4),st(0)        ;226u   A0 X/w+ A X A01 r0 1/d 1/w
fadd st(5),st(0)        ;227u   A0 X/w+ A X A01 r0+A0 1/d 1/w
fxch st(1)              ;227v   X/w+ A0 A X A01 r0+A0 1/d 1/w
fsub f31                ;228u   X0 A0 A X A01 r0+A0 1/d 1/w
fxch st(1)              ;228v   A0 X0 A X A01 r0+A0 1/d 1/w
fmul d0                 ;229u   A00 X0 A X A01 r0+A0 1/d 1/w
fxch st(4)              ;229v   A01 X0 A X A00 r0+A0 1/d 1/w
fsubp st(2),st(0)       ;230u   X0 A-A01 X A00 r0+A0 1/d 1/w
fld w1                  ;231u   w1 X0 A-A01 X A00 r0+A0 1/d 1/w
fmul st(0),st(1)        ;232u   X01 X0 A-A01 X A00 r0+A0 1/d 1/w
fxch st(4)              ;232v   A00 X0 A-A01 X X01 r0+A0 1/d 1/w
fsubp st(2),st(0)       ;233u   X0 A-A0*d X X01 r0+A0 1/d 1/w
fadd st(4),st(0)        ;234u   X0 A-A0*d X X01 R0 1/d 1/w
fmul w0                 ;235u   X00 A-A0*d X X01 R0 1/d 1/w
fld st(5)               ;236u   1/d X00 A-A0*d X X01 R0 1/d 1/w
fmul st(0),st(2)        ;237u   A/d-A0 X00 A-A0*d X X01 R0 1/d 1/w
fxch st(4)              ;237v   X01 X00 A-A0*d X A/d-A0 R0 1/d 1/w
fsubp st(3),st(0)       ;238u   X00 A-A0*d X-X01 A/d-A0 R0 1/d 1/w
fld d1                  ;239u   d1 X00 A-A0*d X-X01 A/d-A0 R0 1/d 1/w
fxch st(4)              ;239v   A/d-A0 X00 A-A0*d X-X01 d1 R0 1/d 1/w
fadd fm1                ;240u   A1+ X00 A-A0*d X-X01 d1 R0 1/d 1/w
fxch st(1)              ;240v   X00 A1+ A-A0*d X-X01 d1 R0 1/d 1/w
fsubp st(3),st(0)       ;241u   A1+ A-A0*d X-X0*w d1 R0 1/d 1/w
fld st(6)               ;242u   1/w A1+ A-A0*d X-X0*w d1 R0 1/d 1/w
fxch st(1)              ;242v   A1+ 1/w A-A0*d X-X0*w d1 R0 1/d 1/w
fsub fm1                ;243u   A1 1/w A-A0*d X-X0*w d1 R0 1/d 1/w
fxch st(1)              ;243v   1/w A1 A-A0*d X-X0*w d1 R0 1/d 1/w
fmul st(0),st(3)        ;244u   X/w-X0 A1 A-A0*d X-X0*w d1 R0 1/d 1/w
fxch st(5)              ;245u   R0 A1 A-A0*d X-X0*w d1 X/w-X0 1/d 1/w
fstp qword _result      ;246-7  A1 A-A0*d X-X0*w d1 X/w-X0 1/d 1/w
fmul st(3),st(0)        ;248u   A1 A-A0*d X-X0*w A11 X/w-X0 1/d 1/w
fxch st(4)              ;248v   X/w-X0 A-A0*d X-X0*w A11 A1 1/d 1/w
fadd fm1                ;249u   X1+ A-A0*d X-X0*w A11 A1 1/d 1/w
fld qword _result+8     ;250u   r1 X1+ A-A0*d X-X0*w A11 A1 1/d 1/w
fxch st(5)              ;250v   A1 X1+ A-A0*d X-X0*w A11 r1 1/d 1/w
fadd st(5),st(0)        ;251u   A1 X1+ A-A0*d X-X0*w A11 r1+A1 1/d 1/w
fxch st(1)              ;251v   X1+ A1 A-A0*d X-X0*w A11 r1+A1 1/d 1/w
fsub fm1                ;252u   X1 A1 A-A0*d X-X0*w A11 r1+A1 1/d 1/w
fxch st(1)              ;252v   A1 X1 A-A0*d X-X0*w A11 r1+A1 1/d 1/w
fmul d0                 ;253u   A10 X1 A-A0*d X-X0*w A11 r1+A1 1/d 1/w
fxch st(4)              ;253v   A11 X1 A-A0*d X-X0*w A10 r1+A1 1/d 1/w
fsubp st(2),st(0)       ;254u   X1 A-A0*d-A11 X-X0*w A10 r1+A1 1/d 1/w
fld w1                  ;255u   w1 X1 A-A0*d-A11 X-X0*w A10 r1+A1 1/d 1/w
fmul st(0),st(1)        ;256u   X11 X1 A-A0*d-A01 X-X0*w A10 r1+A1 1/d 1/w
fxch st(4)              ;256v   A10 X1 A-A0*d-A01 X-X0*w X11 r1+A1 1/d 1/w
fsubp st(2),st(0)       ;257u   X1 A-(A0+A1)*d X-X0*w X11 r1+A1 1/d 1/w
fadd st(4),st(0)        ;258u   X1 A-(A0+A1)*d X-X0*w X11 R1 1/d 1/w
fmul w0                 ;259u   X10 A-(A0+A1)*d X-X0*w X11 R1 1/d 1/w
fld st(5)               ;260u   1/d X10 A-(A0+A1)*d X-X0*w X11 R1 1/d 1/w
fmul st(0),st(2)        ;261u   A/d-A0-A1 X10 A-(A0+A1)*d X-X0*w X11 R1 1/d 1/w
fxch st(4)              ;261v   X11 X10 A-(A0+A1)*d X-X0*w A/d-A0-A1 R1 1/d 1/w
fsubp st(3),st(0)       ;262u   X10 A-(A0+A1)*d X-X0*w-X11 A/d-A0-A1 R1 1/d 1/w
fld d1                  ;263u   d1 X10 A-(A0+A1)*d X-X0*w-X11 A/d-A0-A1 R1 1/d 1/w
fxch st(4)              ;263v   A/d-A0-A1 X10 A-(A0+A1)*d X-X0*w-X11 d1 R1 1/d 1/w
fadd fm33               ;264u   A2+ X10 A-(A0+A1)*d X-X0*w-X11 d1 R1 1/d 1/w
fxch st(1)              ;264v   X10 A2+ A-(A0+A1)*d X-X0*w-X11 d1 R1 1/d 1/w
fsubp st(3),st(0)       ;265u   A2+ A-(A0+A1)*d X-(X0+X1)*w d1 R1 1/d 1/w
fld st(6)               ;266u   1/w A2+ A-(A0+A1)*d X-(X0+X1)*w d1 R1 1/d 1/w
fxch st(1)              ;266v   A2+ 1/w A-(A0+A1)*d X-(X0+X1)*w d1 R1 1/d 1/w
fsub fm33               ;267u   A2 1/w A-(A0+A1)*d X-(X0+X1)*w d1 R1 1/d 1/w
fxch st(1)              ;267v   1/w A2 A-(A0+A1)*d X-(X0+X1)*w d1 R1 1/d 1/w
fmul st(0),st(3)        ;268u   X/w-X0-X1 A2 A-(A0+A1)*d X-(X0+X1)*w d1 R1 1/d 1/w
fxch st(5)              ;269u   R1 A2 A-(A0+A1)*d X-(X0+X1)*w d1 X/w-X0-X1 1/d 1/w
fstp qword _result+8    ;270-1  A2 A-(A0+A1)*d X-(X0+X1)*w d1 X/w-X0-X1 1/d 1/w
fmul st(3),st(0)        ;272u   A2 A-(A0+A1)*d X-(X0+X1)*w A21 X/w-X0-X1 1/d 1/w
fxch st(4)              ;272v   X/w-X0-X1 A-(A0+A1)*d X-(X0+X1)*w A21 A2 1/d 1/w
fadd fm33               ;273u   X2+ A-(A0+A1)*d X-(X0+X1)*w A21 A2 1/d 1/w
fld qword _result+16    ;274u   r2 X2+ A-(A0+A1)*d X-(X0+X1)*w A21 A2 1/d 1/w
fxch st(5)              ;274v   A2 X2+ A-(A0+A1)*d X-(X0+X1)*w A21 r2 1/d 1/w
fadd st(5),st(0)        ;275u   A2 X2+ A-(A0+A1)*d X-(X0+X1)*w A21 r2+A2 1/d 1/w
fxch st(1)              ;275v   X2+ A2 A-(A0+A1)*d X-(X0+X1)*w A21 r2+A2 1/d 1/w
fsub fm33               ;276u   X2 A2 A-(A0+A1)*d X-(X0+X1)*w A21 r2+A2 1/d 1/w
fxch st(4)              ;276v   A21 A2 A-(A0+A1)*d X-(X0+X1)*w X2 r2+A2 1/d 1/w
fsubp st(2),st(0)       ;277u   A2 A-(A0+A1)*d-A21 X-(X0+X1)*w X2 r2+A2 1/d 1/w
fld w1                  ;278u   w1 A2 A-(A0+A1)*d-A21 X-(X0+X1)*w X2 r2+A2 1/d 1/w
fmul st(0),st(4)        ;279u   X21 A2 A-(A0+A1)*d-A21 X-(X0+X1)*w X2 r2+A2 1/d 1/w
fxch st(4)              ;279v   X2 A2 A-(A0+A1)*d-A21 X-(X0+X1)*w X21 r2+A2 1/d 1/w
fadd st(5),st(0)        ;280u   X2 A2 A-(A0+A1)*d-A21 X-(X0+X1)*w X21 R2 1/d 1/w
fxch st(1)              ;280v   A2 X2 A-(A0+A1)*d-A21 X-(X0+X1)*w X21 R2 1/d 1/w
fmul d0                 ;281u   A20 X2 A-(A0+A1)*d-A21 X-(X0+X1)*w X21 R2 1/d 1/w
fxch st(4)              ;281v   X21 X2 A-(A0+A1)*d-A21 X-(X0+X1)*w A20 R2 1/d 1/w
fsubp st(3),st(0)       ;282u   X2 A-(A0+A1)*d-A21 X-(X0+X1)*w-X21 A20 R2 1/d 1/w
fmul w0                 ;283u   X20 A-(A0+A1)*d-A21 X-(X0+X1)*w-X21 A20 R2 1/d 1/w
fxch st(3)              ;283v   A20 A-(A0+A1)*d-A21 X-(X0+X1)*w-X21 X20 R2 1/d 1/w
fsubp st(1),st(0)       ;284u   A-(A0+A1+A2)*d X-(X0+X1)*w-X21 X20 R2 1/d 1/w
fxch st(2)              ;284v   X20 X-(X0+X1)*w-X21 A-(A0+A1+A2)*d R2 1/d 1/w
fld qword _result+24    ;285u   r3 X20 X-(X0+X1)*w-X21 A-(A0+A1+A2)*d R2 1/d 1/w
fxch st(1)              ;285v   X20 r3 X-(X0+X1)*w-X21 A-(A0+A1+A2)*d R2 1/d 1/w
fsubp st(2),st(0)       ;286u   r3 X-(X0+X1+X2)*w A-(A0+A1+A2)*d R2 1/d 1/w
fxch st(2)              ;286v   A-(A0+A1+A2)*d X-(X0+X1+X2)*w r3 R2 1/d 1/w
fmulp st(4),st(0)       ;287u   X-(X0+X1+X2)*w r3 R2 A3 1/w
mov eax,_dinc           ;288u
mov ebx,_d              ;288v
fmulp st(4),st(0)       ;289u   r3 R2 A3 X3
faddp st(2),st(0)       ;290u   R2 r3+A3 X3
fstp qword _result+16   ;291-2  r3+A3 X3
faddp st(1),st(0)       ;293u   R3
mov ecx,_d+4            ;294u
add ebx,eax             ;294v
adc ecx,0               ;295u
mov _d,ebx              ;295v
mov ebx,_w              ;296u
mov eax,_winc           ;296v
fstp qword _result+24   ;297-8
add ebx,eax             ;299u
mov edx,_w+4            ;299v
adc edx,0               ;300u
mov _d+4,ecx            ;300v
mov _dhi+4,ecx          ;301u
mov _w,ebx              ;301v
mov _w+4,edx            ;302u
mov _whi+4,edx          ;302v
mov eax,_power          ;303u
mov ebx,_powerinc       ;303v
sub eax,ebx             ;304u
mov edx,_power+4        ;304v
sbb edx,0               ;305u
mov _power,eax          ;305v
mov ecx,_count          ;306u
mov _power+4,edx        ;306v
dec ecx                 ;307u
mov _count,ecx          ;308u
jne mainloop            ;308v

fld qword _result+24    ;1u     r3
fadd fm33               ;2u     r3hi+
fld qword _result+24    ;3u     r3 r3hi+
fld qword _result+16    ;4u     r2 r3 r3hi+
fxch st(2)              ;4v     r3hi+ r3 r2
fsub fm33               ;5u     r3hi r3 r2
fadd st(2),st(0)        ;8u     r3hi r3 r2+r3hi
fsubp st(1),st(0)       ;9u     R3 r2+r3hi
fld fm1                 ;10u    fm1 R3 r2+r3hi
fadd st(0),st(2)        ;11u    r2hi+ R3 r2+r3hi
fxch st(1)              ;11v    R3 r2hi+ r2+r3hi
fstp qword _result+24   ;12-13  r2hi+ r2+r3hi
fsub fm1                ;14u    r2hi r2+r3hi
fld qword _result+8     ;15u    r1 r2hi r2+r3hi
fxch st(1)              ;15v    r2hi r1 r2+r3hi
fadd st(1),st(0)        ;17u    r2hi r1+r2hi r2+r3hi
fsubp st(2),st(0)       ;18u    r1+r2hi R2
fxch st(1)              ;18v    R2 r1+r2hi 
fld f31                 ;19u    f31 R2 r1+r2hi
fadd st(0),st(2)        ;20u    r1hi+ R2 r1+r2hi
fxch st(1)              ;20v    R2 r1hi+ r1+r2hi
fstp qword _result+16   ;21-22  r1hi+ r1+r2hi
fsub f31                ;23u    r1hi r1+r2hi
fld qword _result       ;24u    r0 r1hi r1+r2hi
fxch st(1)              ;24v    r1hi r0 r1+r2hi
fadd st(1),st(0)        ;26u    r1hi r0+r1hi r1+r2hi
fsubp st(2),st(0)       ;27u    r0+r1hi R1
fxch st(1)              ;27v    R1 r0+r1hi
fld f63                 ;28u    f63 R1 r0+r1hi
fadd st(0),st(2)        ;29u    r0hi+ R1 r0+r1hi
fxch st(1)              ;29v    R1 r0hi+ r0+r1hi
fstp qword _result+8    ;30-31  r0hi+ r0+r1hi
fsub f63                ;32u    r0hi r0+r1hi
fsubp st(1),st(0)       ;35u    R0
fstp qword _result      ;38-39

exit_powmod:

pop ebp
pop edi
pop esi

ret 4

;
; TOTAL time taken (per term): 308+inner*60+outer*5
; = 308+(n-7)*60+(ceil(n/6)-2)*5
; = n*60+ceil(n/6)*5-122 ~~ n*60.833 - 122
;
; For n=ceil(log[2](5*10^12))=, this is 2498 clocks.
;

;******************************************************************************************
fcm8    dd      -8.
fcm4    dd      -4.
fcm1    dd      -1.
fcp75   dd      0.75
f1      dd      2.
fc3     dd      3.
fc5     dd      5.
fc7     dd      7.
f3      dd      8.
fc9     dd      9.
fc10    dd      10.
fc11    dd      11.
fc12    dd      12.
fc13    dd      13.
fc15    dd      15.
f4      dd      16.
fc17    dd      17.
fc18    dd      18.
fc19    dd      19.
fc20    dd      20.
fc100   dd      100.
;******************************************************************************************

EXTRN CALC_PROGRESS:PROC


PUBLIC CALC_THREAD_DOWORK_P5
CALC_THREAD_DOWORK_P5:
        push ebp
        mov ebp,esp
        push edi
        push esi
        mov edi,[ebp+8]
        
        push edi
        call CALC_PROGRESS

        cmp dword [edi],-1
        jne cont
        
        pop esi
        pop edi
        pop ebp
        ret 4
        

cont:        
        add edi,104

;struct threaddat {
;-104  long threadnum;
;-100  long outputf;
;-96   unsigned long where[2];
;-88   unsigned long start_pos[2];                         //start of this session's execution -- not start of subrange
;-80   unsigned long current_pos[2];
;-72   unsigned long end_pos[2];
;-64   double pisum[4];
;-32   long internal[8];
;0     polylogdat pldat;
;0            long d[2];
;8            long dinc;
;12           float amul;
;16           long w[2];
;24           long winc;
;28           float xmul;
;32           long power[2];
;40           long powerinc;
;44           long termcount;
;48           long internal[6];
;};
;
        fild qword [edi-96]     ;       w
        fmul f2                 ;       4*w
        fild qword [edi-80]     ;       c 4*w
        fild qword [edi-72]     ;       e c 4*w
        fsub st(0),st(1)        ;       e-c c 4*w
        fstp dword [edi-32]     ;       c 4*w
        mov eax,f4
        mov ebx,[edi-32]
        sub ebx,eax             ;       ebx>0 <==> e-c>256
        mov ecx,ebx
        sar ebx,31              ;       ebx=0 <==> e-c>256
        and ecx,ebx
        add eax,ecx
        mov [edi-32],eax
        fld dword [edi-32]      ;       numterms tirstterm 4*w
        fld fc20                ;       20 numterms firstterm 4*where
        fmul st(0),st(2)        ;       20*firstterm numterms firstterm 4*where
        fsubp st(3),st(0)       ;       numterms firstterm 4*where-20*firstterm
        fistp qword [edi-16]    ;       [edi-16]=numterms
        fistp qword [edi-24]    ;       [edi-24]=firstterm
        fistp qword [edi-32]    ;       [edi-32]=4*where-20*firstterm
        fild qword [edi-16]     ;       numterms
        fild qword [edi-80]
        faddp st(1),st(0)
        fistp qword [edi-80]
;0            long d[2];
;8            long dinc;
;12           float amul;
;16           long w[2];
;24           long winc;
;28           float xmul;
;32           long power[2];
;40           long powerinc;
;44           long termcount;

        fild qword [edi-32]     ;       where*4-firstterm*20
        fsub f2                 ;       where*4-firstterm*20-4
        fistp qword [edi+32]    ;       power=where*4-firstterm*20-4
        mov dword [edi+40],20   ;       powerinc=20
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd f0                 ;       firstterm*20+1
        fistp qword [edi]       ;       d=firstterm*20+1
        mov dword [edi+8],20    ;       dinc=20
        fld f2                  ;       4
        fstp dword [edi+12]     ;       amul=4
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc3                ;       firstterm*20+3
        fistp qword [edi+16]    ;       w=firstterm*20+3
        mov dword [edi+24],20   ;       winc=20
        fld fcm1                ;       -1
        fstp dword [edi+28]     ;       xmul=-1
        fild qword [edi-16]     ;       numterms
        fistp qword [edi+44]    ;       numterms=numterms
        push edi
        call POWERFUNC_P5

        fild qword [edi-32]     ;       where*4-firstterm*20
        fsub f3                 ;       where*4-firstterm*20-8
        fistp qword [edi+32]    ;       power=where*4-firstterm*20-8
        mov dword [edi+40],20   ;       powerinc=20
        fild qword [edi-24]     ;       firstterm
        fmul f3                 ;       firstterm*8
        fadd f0                 ;       firstterm*8+1
        fistp qword [edi]       ;       d=firstterm*8+1
        mov dword [edi+8],8     ;       dinc=8
        fld fcm8                ;       -8
        fstp dword [edi+12]     ;       amul=-8
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc5                ;       firstterm*20+5
        fistp qword [edi+16]    ;       w=firstterm*20+5
        mov dword [edi+24],20   ;       winc=20
        fld fcm1                ;       -1
        fstp dword [edi+28]     ;       xmul=-1
        fild qword [edi-16]     ;       numterms
        fistp qword [edi+44]    ;       numterms=numterms
        push edi
        call POWERFUNC_P5
        
        fild qword [edi-32]     ;       where*4-firstterm*20
        fsub fc10               ;       where*4-firstterm*20-10
        fistp qword [edi+32]    ;       power=where*4-firstterm*20-10
        mov dword [edi+40],20   ;       powerinc=20
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc7                ;       firstterm*20+7
        fistp qword [edi]       ;       d=firstterm*20+7
        mov dword [edi+8],20    ;       dinc=20
        fld fcm4                ;       -4
        fstp dword [edi+12]     ;       amul=-4
        fild qword [edi-24]     ;       firstterm
        fmul f3                 ;       firstterm*8
        fadd fc3                ;       firstterm*8+3
        fistp qword [edi+16]    ;       w=firstterm*8+3
        mov dword [edi+24],8    ;       winc=8
        fld fcm1                ;       -1
        fstp dword [edi+28]     ;       xmul=-1
        fild qword [edi-16]     ;       numterms
        fistp qword [edi+44]    ;       numterms=numterms
        push edi
        call POWERFUNC_P5

        fild qword [edi-32]     ;       where*4-firstterm*20
        fsub fc12               ;       where*4-firstterm*20-12
        fistp qword [edi+32]    ;       power=where*4-firstterm*20-12
        mov dword [edi+40],20   ;       powerinc=20
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc9                ;       firstterm*20+9
        fistp qword [edi]       ;       d=firstterm*20+9
        mov dword [edi+8],20    ;       dinc=20
        fld f2                  ;       4
        fstp dword [edi+12]     ;       amul=4
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc11               ;       firstterm*20+11
        fistp qword [edi+16]    ;       w=firstterm*20+11
        mov dword [edi+24],20   ;       winc=20
        fld fcm1                ;       -1
        fstp dword [edi+28]     ;       xmul=-1
        fild qword [edi-16]     ;       numterms
        fistp qword [edi+44]    ;       numterms=numterms
        push edi
        call POWERFUNC_P5

        fild qword [edi-32]     ;       where*4-firstterm*20
        fsub fc15               ;       where*4-firstterm*20-15
        fistp qword [edi+32]    ;       power=where*4-firstterm*20-15
        mov dword [edi+40],20   ;       powerinc=20
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc13               ;       firstterm*20+13
        fistp qword [edi]       ;       d=firstterm*20+13
        mov dword [edi+8],20    ;       dinc=20
        fld f1                  ;       2
        fstp dword [edi+12]     ;       amul=2
        fild qword [edi-24]     ;       firstterm
        fmul f3                 ;       firstterm*8
        fadd fc5                ;       firstterm*8+5
        fistp qword [edi+16]    ;       w=firstterm*8+5
        mov dword [edi+24],8    ;       winc=8
        fld f0                  ;       1
        fstp dword [edi+28]     ;       xmul=1
        fild qword [edi-16]     ;       numterms
        fistp qword [edi+44]    ;       numterms=numterms
        push edi
        call POWERFUNC_P5

        fild qword [edi-32]     ;       where*4-firstterm*20
        fsub fc18               ;       where*4-firstterm*20-18
        fistp qword [edi+32]    ;       power=where*4-firstterm*20-18
        mov dword [edi+40],20   ;       powerinc=20
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc17               ;       firstterm*20+17
        fistp qword [edi]       ;       d=firstterm*20+17
        mov dword [edi+8],20    ;       dinc=20
        fld f0                  ;       1
        fstp dword [edi+12]     ;       amul=1
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc15               ;       firstterm*20+15
        fistp qword [edi+16]    ;       w=firstterm*20+15
        mov dword [edi+24],20   ;       winc=20
        fld f0                  ;       1
        fstp dword [edi+28]     ;       xmul=1
        fild qword [edi-16]     ;       numterms
        fistp qword [edi+44]    ;       numterms=numterms
        push edi
        call POWERFUNC_P5

        fild qword [edi-32]     ;       where*4-firstterm*20
        fsub fc20               ;       where*4-firstterm*20-20
        fistp qword [edi+32]    ;       power=where*4-firstterm*20-20
        mov dword [edi+40],20   ;       powerinc=20
        fild qword [edi-24]     ;       firstterm
        fmul f3                 ;       firstterm*8
        fadd fc7                ;       firstterm*8+7
        fistp qword [edi]       ;       d=firstterm*8+7
        mov dword [edi+8],8     ;       dinc=8
        fld f0                  ;       1
        fstp dword [edi+12]     ;       amul=1
        fild qword [edi-24]     ;       firstterm
        fmul fc20               ;       firstterm*20
        fadd fc19               ;       firstterm*20+19
        fistp qword [edi+16]    ;       w=firstterm*20+19
        mov dword [edi+24],20   ;       winc=20
        fld fcm1                ;       -1
        fstp dword [edi+28]     ;       xmul=-1
        fild qword [edi-16]     ;       numterms
        fistp qword [edi+44]    ;       numterms=numterms
        push edi
        call POWERFUNC_P5



        pop esi
        pop edi
        pop ebp
        ret 4

        _TEXT32 ENDS
        END

