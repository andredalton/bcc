;
;  This code is Copyright 1997-1998 by Colin Percival.
;
;  This code may be copied, modified, and used for non-commercial purposes
;  only.  If source code to a program using this code is made publicly available, 
;  it must include a valid copyright notice.
;

.387
.586

_POWMODDATA SEGMENT USE32 PARA PUBLIC 'BSS'     ;internal variables used by
DGROUP GROUP _POWMODDATA                        ;powmod.
_POWMODDATA ENDS


_TEXT32 SEGMENT USE32 PUBLIC 'CODE'
EXTRN _overheat_err:DWORD
EXTRN _where:QWORD




PUBLIC POWERINIT_P6
POWERINIT_P6:
        ret

_result EQU     [ebp-64]
_d      EQU     [ebp]
_a_lo   EQU     [ebp+32]
_a_hi   EQU     [ebp+64]
_b      EQU     [ebp+96]
d_inc   EQU     [ebp+128]
a_mul   EQU     [ebp+144]
power   EQU     [ebp+176]
powinc  EQU     [ebp+184]
terms   EQU     [ebp+188]
pow     EQU     [ebp+192]
temp    EQU     [ebp+208]

PUBLIC POLYLOGCONVOUT_P6:
POLYLOGCONVOUT_P6:
        push ebp
        mov ebp,[esp+8]

        fld qword _result+24
        fistp qword _result+24

        mov eax,_result+28
        add eax,_result+16
        mov _result+16,eax
        mov eax,_result+20
        adc eax,_result+8
        mov _result+8,eax
        mov eax,_result+12
        adc eax,_result
        mov _result,eax
        sub eax,eax

        mov _result+4,eax
        mov _result+12,eax
        mov _result+20,eax
        mov _result+28,eax

        fild qword _result
        fstp qword _result
        fild qword _result+8
        fstp qword _result+8
        fild qword _result+16
        fstp qword _result+16
        fild qword _result+24
        fstp qword _result+24

        pop ebp
        ret 4


PUBLIC POWERFUNC_P6
POWERFUNC_P6:
        mov _a_lo+4,dword 043300000H
        mov _a_lo+12,dword 043300000H
        mov _a_lo+20,dword 043300000H
        mov _a_lo+28,dword 043300000H
        mov _a_hi+4,dword 045300000H
        mov _a_hi+12,dword 045300000H
        mov _a_hi+20,dword 045300000H
        mov _a_hi+28,dword 045300000H
        mov pow+8,dword 0H
        
pftloop:
        fild qword power
        fadd dword half
        fstp tbyte pow
        mov ecx,pow+8
        sub ecx,00003FFEH
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Check for FPU errors/overheating here. if ecx is not within reasonable bounds, raise
; error.

cmp ecx,64
jbe not_overheat

mov _overheat_err, 1
finit
jmp exit_powmod

not_overheat:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        fld qword blah
        fild qword _d+24
        fld1
        fdivrp st(1),st(0)

        mov edx,pow+4
        mov eax,pow

        mov ebx,03F800000H
        cmp ecx,7
        jnae l5
        
        mov esi,edx
        shr esi,3
        and esi,01F800000H
        add ebx,esi
        
        shld edx,eax,6
        shl eax,6
l5:
        mov pow,eax
        mov pow+4,edx

        fild qword _d+16
        fld1
        fdivrp st(1),st(0)

        mov temp,ebx
        fld dword temp
        fistp qword temp
        mov ebx,temp
        mov _a_lo,ebx
        mov _a_lo+8,ebx
        mov _a_lo+16,ebx
        mov _a_lo+24,ebx
        mov ebx,temp+4
        mov _a_hi,ebx
        mov _a_hi+8,ebx
        mov _a_hi+16,ebx
        mov _a_hi+24,ebx

        fild qword _d+8
        fld1
        fdivrp st(1),st(0)
        fild qword _d
        fld1
        fdivrp st(1),st(0)

        fld qword _a_lo
        fld qword _a_hi
        fsub st(0),st(6)
        faddp st(1),st(0)
        fmul st(0),st(1)
        fistp qword _b
        mov eax,_b
        mov ebx,_d+4
        imul ebx,eax

        fld qword _a_lo+8
        mov edx,_d
        mov esi,_b+4
        imul esi,edx
        fld qword _a_hi+8
        mul edx
        fsub st(0),st(6)
        add esi,ebx
        add esi,edx
        faddp st(1),st(0)
        sub _a_lo,eax
        fmul st(0),st(2)
        sbb _a_hi,esi
        fistp qword _b+8

        mov eax,_b+8
        mov ebx,_d+12
        imul ebx,eax

        fld qword _a_lo+16
        mov edx,_d+8
        mov esi,_b+12
        imul esi,edx
        fld qword _a_hi+16
        mul edx
        fsub st(0),st(6)
        add esi,ebx
        faddp st(1),st(0)
        add esi,edx
        fmul st(0),st(3)
        sub _a_lo+8,eax
        sbb _a_hi+8,esi
        fistp qword _b+16

        mov eax,_b+16
        mov ebx,_d+20
        imul ebx,eax
        
        fld qword _a_lo+24
        mov edx,_d+16
        mov esi,_b+20
        imul esi,edx
        fld qword _a_hi+24
        mul edx
        fsub st(0),st(6)
        add esi,ebx
        faddp st(1),st(0)
        add esi,edx
        fmul st(0),st(4)
        sub _a_lo+16,eax
        sbb _a_hi+16,esi
        fistp qword _b+24

        mov eax,_b+24
        mov ebx,_d+28
        imul ebx,eax

        fld qword _a_lo         ;
        mov edx,_d+24
        mov esi,_b+28
        imul esi,edx
        fld qword _a_hi         ;
        mul edx
        fsub st(0),st(6)        ;
        add esi,ebx
        faddp st(1),st(0)       ;
        add esi,edx
        fmul st(0),st(0)        ;
        sub _a_lo+24,eax
        sbb _a_hi+24,esi
        fmul st(0),st(1)        ;

        fld qword _a_lo+8       ;
        fld qword _a_hi+8       ;
        fsub st(0),st(7)        ;
        faddp st(1),st(0)       ;
        fxch st(1)
        mov eax,pow
        mov edx,pow+4
        fistp qword _b
        add eax,eax
        fmul st(0),st(0)        ;
        adc edx,edx
        setc cl
        mov pow,eax
        fmul st(0),st(2)        ;
        mov pow+4,edx

align 16
p6iloop:
        fld qword _a_lo+16      ;
        fld qword _a_hi+16      ;
        fsub st(0),st(7)        ;
        mov edi,dword _a_hi     ;
        mov eax,dword _a_lo     ;
        imul edi,eax            ;
        add edi,edi             ;
        mul eax                 ;
        faddp st(1),st(0)       ;
        fxch st(1)
        fistp qword _b+8
        mov esi,eax             ;
        mov ebx,_d+4            ;
        mov eax,_b              ;
        imul ebx,eax            ;
        sub edi,ebx             ;
        add edi,edx             ;
        mov edx,_d              ;
        mov ebx,_b+4            ;
        imul ebx,edx            ;
        fmul st(0),st(0)        ;
        sub edi,ebx             ;
        mul edx                 ;
        sub esi,eax             ;
        sbb edi,edx             ;
        fmul st(0),st(3)        ;
        shld edi,esi,cl         ;
        shl esi,cl              ;
        mov dword _a_lo,esi     ;
        mov dword _a_hi,edi     ;


        fld qword _a_lo+24      ;
        fld qword _a_hi+24      ;
        fsub st(0),st(7)        ;
        mov edi,dword _a_hi+8   ;
        mov eax,dword _a_lo+8   ;
        imul edi,eax            ;
        add edi,edi             ;
        mul eax                 ;
        faddp st(1),st(0)       ;
        fxch st(1)
        fistp qword _b+16
        mov esi,eax             ;
        mov ebx,_d+12           ;
        mov eax,_b+8            ;
        imul ebx,eax            ;
        sub edi,ebx             ;
        add edi,edx             ;
        mov edx,_d+8            ;
        mov ebx,_b+12           ;
        imul ebx,edx            ;
        fmul st(0),st(0)        ;
        sub edi,ebx             ;
        mul edx                 ;
        sub esi,eax             ;
        sbb edi,edx             ;
        fmul st(0),st(4)        ;
        shld edi,esi,cl         ;
        shl esi,cl              ;
        mov dword _a_lo+8,esi   ;
        mov dword _a_hi+8,edi   ;

        fld qword _a_lo         ;
        fld qword _a_hi         ;
        fsub st(0),st(7)        ;
        mov edi,dword _a_hi+16  ;
        mov eax,dword _a_lo+16  ;
        imul edi,eax            ;
        add edi,edi             ;
        mul eax                 ;
        faddp st(1),st(0)       ;
        fxch st(1)
        fistp qword _b+24
        mov esi,eax             ;
        mov ebx,_d+20           ;
        mov eax,_b+16           ;
        imul ebx,eax            ;
        sub edi,ebx             ;
        add edi,edx             ;
        mov edx,_d+16           ;
        mov ebx,_b+20           ;
        imul ebx,edx            ;
        fmul st(0),st(0)        ;
        sub edi,ebx             ;
        mul edx                 ;
        sub esi,eax             ;
        sbb edi,edx             ;
        fmul st(0),st(1)        ;
        shld edi,esi,cl         ;
        shl esi,cl              ;
        mov dword _a_lo+16,esi  ;
        mov dword _a_hi+16,edi  ;


        fld qword _a_lo+8       ;
        fld qword _a_hi+8       ;
        mov edi,dword _a_hi+24  ;
        mov eax,dword _a_lo+24  ;
        imul edi,eax            ;
        fsub st(0),st(7)        ;
        add edi,edi             ;
        mul eax                 ;
        mov esi,eax             ;
        mov ebx,_d+28           ;
        mov eax,_b+24           ;
        imul ebx,eax            ;
        faddp st(1),st(0)       ;
        fxch st(1)
        fistp qword _b
        sub edi,ebx             ;
        add edi,edx             ;
        mov edx,_d+24           ;
        mov ebx,_b+28           ;
        imul ebx,edx            ;
        sub edi,ebx             ;
        mul edx                 ;
        sub esi,eax             ;
        sbb edi,edx             ;
        mov eax,pow
        mov edx,pow+4
        fmul st(0),st(0)        ;
        shld edi,esi,cl         ;
        shl esi,cl              ;
        add eax,eax
        adc edx,edx
        setc cl
        fmul st(0),st(2)        ;
        mov dword _a_lo+24,esi  ;
        mov pow+4,edx
        or edx,eax
        mov pow,eax
        mov dword _a_hi+24,edi  ;
        jnz p6iloop

        fcomp

        fld qword _a_lo
        fld qword _a_hi
        fsub st(0),st(6)
        faddp st(1),st(0)
        fimul dword a_mul
        fmul st(0),st(1)
        fistp qword _b

        fld qword _a_lo+8
        fld qword _a_hi+8
        fsub st(0),st(6)
        faddp st(1),st(0)
        fimul dword a_mul+8
        fmul st(0),st(2)
        fistp qword _b+8

        fld qword _a_lo+16
        fld qword _a_hi+16
        fsub st(0),st(6)
        faddp st(1),st(0)
        fimul dword a_mul+16
        fmul st(0),st(3)
        fistp qword _b+16

        fld qword _a_lo+24
        fld qword _a_hi+24
        fsub st(0),st(6)
        faddp st(1),st(0)
        fimul dword a_mul+24
        fmul st(0),st(4)
        fistp qword _b+24

        mov ecx,3
mul_loop:
        mov eax,_a_lo[ecx*8]
        mov ebx,a_mul[ecx*8]
        mov esi,a_mul+4[ecx*8]
        and esi,eax
        mul ebx
        mov _a_lo[ecx*8],eax
        imul ebx,_a_hi[ecx*8]
        add edx,ebx
        sub edx,esi
        mov _a_hi[ecx*8],edx

        mov eax,_b[ecx*8]
        mov ebx,_d+4[ecx*8]
        imul ebx,eax
        mov edx,_d[ecx*8]
        mov esi,_b+4[ecx*8]
        imul esi,edx
        mul edx
        add esi,ebx
        add esi,edx
        sub _a_lo[ecx*8],eax
        sbb _a_hi[ecx*8],esi

        dec ecx
        jns mul_loop

        mov ecx,-3
res_loop:
        mov esi,_result+24[ecx*8]
        mov edi,_result+28[ecx*8]

        fld qword _a_lo
        fld qword _a_hi
        fsub st(0),st(6)
        faddp st(1),st(0)
        fmul dword f32

        fld qword _a_lo+8
        
        fxch st(1)
        fmul st(0),st(2)
        fxch st(1)
        
        fld qword _a_hi+8
        fsub st(0),st(7)
        faddp st(1),st(0)

        fxch st(1)
        fistp qword _b

        mov eax,_b
        mov ebx,_d+4
        imul ebx,eax
        mov edx,_d

        fmul dword f32

        add esi,eax
        adc edi,_b+4
        mul edx

        fld qword _a_lo+16

        add ebx,edx
        mov edx,_b+4
        imul edx,_d
        add ebx,edx
        mov edx,_a_lo
        
        fxch st(1)
        fmul st(0),st(3)
        fxch st(1)
        
        neg eax
        sbb edx,ebx

        fld qword _a_hi+16
        fsub st(0),st(7)
        faddp st(1),st(0)

        mov _a_lo,eax
        mov _a_hi,edx
        
        fxch st(1)
        fistp qword _b+8
        
        mov eax,_b+8
        mov ebx,_d+12
        imul ebx,eax
        mov edx,_d+8

        fmul dword f32

        add esi,eax
        adc edi,_b+12
        mul edx

        fld qword _a_lo+24

        add ebx,edx
        mov edx,_b+12
        imul edx,_d+8
        add ebx,edx
        mov edx,_a_lo+8

        fxch st(1)
        fmul st(0),st(4)
        fxch st(1)        

        neg eax
        sbb edx,ebx

        fld qword _a_hi+24
        fsub st(0),st(7)
        faddp st(1),st(0)
        
        mov _a_lo+8,eax
        mov _a_hi+8,edx

        fxch st(1)
        fistp qword _b+16

        mov eax,_b+16
        mov ebx,_d+20
        imul ebx,eax
        mov edx,_d+16
        
        fmul dword f32

        add esi,eax
        adc edi,_b+20

        mul edx
        add ebx,edx
        mov edx,_b+20
        imul edx,_d+16
        add ebx,edx
        mov edx,_a_lo+16

        fmul st(0),st(4)

        neg eax
        sbb edx,ebx
        mov _a_lo+16,eax
        mov _a_hi+16,edx

        fistp qword _b+24
        
        mov eax,_b+24
        mov ebx,_d+28
        imul ebx,eax
        mov edx,_d+24
        add esi,eax
        adc edi,_b+28
        mul edx
        add ebx,edx
        mov edx,_b+28
        imul edx,_d+24
        add ebx,edx
        mov edx,_a_lo+24
        neg eax
        sbb edx,ebx
        mov _a_lo+24,eax
        mov _a_hi+24,edx

        mov _result+24[ecx*8],esi
        mov _result+28[ecx*8],edi

        inc ecx
        jnz res_loop

        fld qword _result+24
        
        fld qword _a_lo
        fld qword _a_hi
        fsub st(0),st(7)
        faddp st(1),st(0)
        fmul dword f32                ; a r3 1/d 1/d 1/d 1/d blah
        fmulp st(2),st(0)
        faddp st(1),st(0)

        fld qword _a_lo+8
        fld qword _a_hi+8
        fsub st(0),st(6)
        faddp st(1),st(0)
        fmul dword f32
        fmulp st(2),st(0)
        faddp st(1),st(0)

        fld qword _a_lo+16
        fld qword _a_hi+16
        fsub st(0),st(5)
        faddp st(1),st(0)
        fmul dword f32
        fmulp st(2),st(0)
        faddp st(1),st(0)

        fld qword _a_lo+24 ; a0 r3 1/d3 blah
        fld qword _a_hi+24 ; a1 a0 r3 1/d3 blah
        fsubrp st(4),st(0) ; a0 r3 1/d3 a1-blah
        faddp st(3),st(0)  ; r3 1/d3 a
        fxch st(2)         ; a 1/d3 r3
        fmul dword f32
        fmulp st(1),st(0)
        faddp st(1),st(0)

        fstp qword _result+24        
                

        sub edx,edx

        mov eax,powinc
        mov ebx,power
        mov ecx,power+4
        add ebx,eax
        adc ecx,edx
        dec ecx
        mov power,ebx
        mov power+4,ecx
        
        mov eax,d_inc
        mov ebx,_d
        mov ecx,_d+4
        add ebx,eax
        adc ecx,edx
        mov _d,ebx
        mov _d+4,ecx

        mov eax,d_inc+4
        mov ebx,_d+8
        mov ecx,_d+12
        add ebx,eax
        adc ecx,edx
        mov _d+8,ebx
        mov _d+12,ecx

        mov eax,d_inc+8
        mov ebx,_d+16
        mov ecx,_d+20
        add ebx,eax
        adc ecx,edx
        mov _d+16,ebx
        mov _d+20,ecx

        mov eax,d_inc+12
        mov ebx,_d+24
        mov ecx,_d+28
        add ebx,eax
        adc ecx,edx
        mov _d+24,ebx
        mov _d+28,ecx
        
        mov eax,terms
        dec eax
        mov terms,eax
        jne pftloop
exit_powmod:        
ret

        


;******************************************************************************************
pi_formula      dd  64,  1, 40,  -16,  3, 40,  -8,  1, 16,  -1,  5, 40,    -4-4, -40
                dd -16,  7, 40,   -4,  3, 16,   4,  9, 40,  -1, 11, 40,    -8-4, -40
                dd  16, 13, 40,    8,  5, 16,   1, 15, 40,   1, 17, 40,   -14-4, -40
                dd  16,  7, 16,  -16, 19, 40,   4, 21, 40,  -1, 23, 40,   -20-4, -40
                dd -32,  9, 16,   -4, 25, 40,  -4, 27, 40,  -1, 11, 16,   -26-4, -40
                dd  32, 29, 40,   -8, 31, 40,   2, 33, 40,   1, 13, 16,   -31-4, -40
                dd   4, 35, 40,    4, 37, 40,   1, 15, 16,  -1, 39, 40,   -36-4, -40

half            dd 0.5
f2              dd 4.0
f4              dd 16.0
f32             dd 04F800000H
blah            dd 000100000H,045300000H
; (amul,d_start,d_inc)x4 ,powstart,powinc
;******************************************************************************************

EXTRN CALC_PROGRESS:PROC


PUBLIC CALC_THREAD_DOWORK_P6
CALC_THREAD_DOWORK_P6:
        push ebp
        mov ebp,esp
        push edi
        push esi
        mov ebp,[ebp+8]
        
        push ebp
        call CALC_PROGRESS
        cmp dword [ebp],-1
        jne cont
        pop esi
        pop edi
        pop ebp
        ret 4
        
cont:        
        add ebp,104


;struct threaddat {
;-104  long threadnum;
;-100  long outputf;
;-96   unsigned long where[2];
;-88   unsigned long start_pos[2];
;-80   unsigned long current_pos[2];
;-72   unsigned long end_pos[2];
;-64   double pisum[4];
;-32   long internal[8];
;-32   numterms
;-24   firstterm
;-16   where (in bits)

        fild qword [ebp-96]     ;       w
        fmul f2                 ;       4*w
        fild qword [ebp-80]     ;       c 4*w
        fild qword [ebp-72]     ;       e c 4*w
        fsub st(0),st(1)        ;       e-c c 4*w
        fstp dword [ebp-32]     ;       c 4*w
        mov eax,f4
        mov ebx,[ebp-32]
        sub ebx,eax             ;       ebx>0 <==> e-c>256
        mov ecx,ebx
        sar ebx,31              ;       ebx=0 <==> e-c>256
        and ecx,ebx
        add eax,ecx
        sub eax,000800000H
        mov [ebp-32],eax
        fld dword [ebp-32]      ;       numterms tirstterm 4*w
        fistp qword [ebp-32]    ;       [ebp-32]=numterms
        fmul half               ;
        fistp qword [ebp-24]    ;       [ebp-24]=firstterm
        fistp qword [ebp-16]    ;       [ebp-16]=where
        fild qword [ebp-32]     ;       numterms
        fild qword [ebp-80]
        fadd st(0),st(1)
        faddp st(1),st(0)
        fistp qword [ebp-80]


;0      qword d[4]
;32     qword a_lo[4]
;64     qword a_hi[4]
;96     qword b[4]
;128    dword d_inc[4]
;144    qword a_mul[4]
;176    qword power
;184    dword powerinc
;188    dword numterms

        mov ebx,6*56
termloop:
        fild qword [ebp-16]     ;       where
        fild qword [ebp-24]     ;       firstterm where
        fimul dword [ebx+pi_formula+52] ; ft*(-40) where
        faddp st(1),st(0)       ;       startpow
        fiadd dword [ebx+pi_formula+48]
        fistp qword [ebp+176]
        fild dword [ebx+pi_formula+52]
        fistp dword [ebp+184]
        fild qword [ebp-32]
        fistp qword [ebp+188]
        
        fild qword [ebp-24]
        fild dword [ebx+pi_formula+8]
        fmul st(1),st(0)
        fistp dword [ebp+128]
        fiadd dword [ebx+pi_formula+4]
        fistp qword [ebp]
        fild dword [ebx+pi_formula]
        fistp qword [ebp+144]

        fild qword [ebp-24]
        fild dword [ebx+pi_formula+20]
        fmul st(1),st(0)
        fistp dword [ebp+132]
        fiadd dword [ebx+pi_formula+16]
        fistp qword [ebp+8]
        fild dword [ebx+pi_formula+12]
        fistp qword [ebp+152]

        fild qword [ebp-24]
        fild dword [ebx+pi_formula+32]
        fmul st(1),st(0)
        fistp dword [ebp+136]
        fiadd dword [ebx+pi_formula+28]
        fistp qword [ebp+16]
        fild dword [ebx+pi_formula+24]
        fistp qword [ebp+160]

        fild qword [ebp-24]
        fild dword [ebx+pi_formula+44]
        fmul st(1),st(0)
        fistp dword [ebp+140]
        fiadd dword [ebx+pi_formula+40]
        fistp qword [ebp+24]
        fild dword [ebx+pi_formula+36]
        fistp qword [ebp+168]

        push ebx
        call POWERFUNC_P6
        pop ebx
        sub ebx,56
        jns termloop


        pop esi
        pop edi
        pop ebp
        ret 4

        _TEXT32 ENDS
        END

