;---------------------
; Title: SevensegmentProject
;---------------------
; Program Details:
; The purpose of this project is to display 0x0 to 0xF. One switch increments
; and another switch decrements

; Inputs: PORTA
; Outputs: PORTD
; Date: March 23, 2024
; File Dependencies / Libraries: SevensegmentConfig.inc 
; Compiler: xc8, 2.46
; Author: Alexander Pagaduan
; Versions:
;       V1.0: Initial Draft
;	      V1.1: Program with comments
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 


;---------------------
; INITIALIZATION
;---------------------
#include "SevensegmentConfig.inc"
#include <xc.inc>
    
;---------------------
; PROGRAM INPUTS
;---------------------

    
;---------------------
; DEFINITIONS
;---------------------

Inner_loop  equ 255 // in decimal
Outer_loop  equ 255
		
    
;---------------------
; PROGRAM CONSTANTS
;---------------------
  
REG10 EQU 0x10	;LoopDelay value stored in REG10
REG11 EQU 0x11	;LoopDelay value stored in REG11
REG20 EQU 0x20	;SwitchA value stored in REG20
REG21 EQU 0x21	;SwitchB value stored in REG21
REG22 EQU 0x22	;SegmentDisplay value stored in REG22
REG23 EQU 0x23	;Table value stored in REG23
REG24 EQU 0x24	;Comparator value stored in REG24
REG25 EQU 0x25	;PORTA value stored in REG25

 
    

;---------------------
; MAIN
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG         0                ;Reset vector
    
    MOVLW 0x00 ;set wreg to 0x00
    MOVWF 0x20 ; move wreg (0x00) into 0x20
    ;Set SwitchA to 0x00
    
    MOVLW 0x00 ;set wreg to 0x00
    MOVWF 0x21 ; move wreg (0x00) into 0x21
    ;Set SwitchB to 0x00
   
    MOVLW 0x00 ;set wreg to 0x00
    MOVWF 0x22 ;move wreg (0x00) into 0x22
    ;Set Segment Display to 0x00
    
    MOVLW 0x00 ;set wreg to 0x00
    MOVWF 0x23 ;move wreg (0x00) into 0x23
    ;Set Table value to 0x00
    
    MOVLW 0x00 ;set wreg to 0x00
    MOVWF 0x24 ;move wreg (0x00) into 0x24
    ;Set Comparator value to 0x00
 
    MOVLW 0x00 ;set wreg to 0x00
    MOVWF 0x25 ;move wreg (0x00) into 0x25
    ;Set ButtonsPressed value to 0x00
 
    
    ;Go to the first task get Inputs
    CALL       SetUpInputs
    CALL       SetUpOutputs
    
     GOTO       START_MAIN_LOOP
;---------------------
; The Delay Subroutine 
;---------------------
loopDelay: 
    MOVLW       Inner_loop
    MOVWF       REG10
    MOVLW       Outer_loop
    MOVWF       REG11
_loop1:
    DECF        REG10,1
    BNZ         _loop1
    MOVLW	Inner_loop
    MOVWF	REG10
    DECF        REG11,1 // outer loop
    BNZ        _loop1
    RETURN
 
START_MAIN_LOOP:
    CALL       ReadSegmentDisplayTable
    CALL       UpdateSegmentDisplay
    CALL	loopDelay ; we can use RCALL
    GOTO       SCAN_BUTTONS
    GOTO       START_MAIN_LOOP
    
    ;ORG         0020H        ; Begin assembly at 0020H
    
 SetUpInputs:
    BANKSEL	PORTA ;
    CLRF	PORTA ;Init PORTA
    BANKSEL	LATA ;Data Latch
    CLRF	LATA ;
    BANKSEL	ANSELA ;
    CLRF	ANSELA ;digital I/O
    BANKSEL	TRISA ;
    MOVLW	0b00000011 ;Set RA[1:0] as inputs
    MOVWF	TRISA ;and set RA as ouput
    RETURN
    
 SetUpOutputs:
    BANKSEL	PORTD ;
    CLRF	PORTD ;Init PORTB
    BANKSEL	LATD ;Data Latch
    CLRF	LATD ;
    BANKSEL	ANSELD ;
    CLRF	ANSELD ;digital I/O
    BANKSEL	TRISD ;
    MOVLW	0b00000000 ; ;Set RD[7:0] as output 
    MOVWF	TRISD ; and set RD as output
    RETURN

SCAN_BUTTONS: ; If statements
    

;If REG24 is not equal to 0 then 
    MOVF PORTA,0,1
    MOVWF REG25
    
CheckNothingPressed:
    ; Check nothing was pressed
    MOVLW 0x00
    MOVWF REG24
    ;MOVF    REG24,0,1
    CPFSEQ REG25
    GOTO CheckButtonAPressed
    GOTO NoUpdateVal
    
CheckButtonAPressed:
    ; Check if ButtonA was pressed
    MOVLW 0x01
    MOVWF REG24
    ;MOVF    REG24,0,1
    CPFSEQ REG25
    GOTO CheckButtonBPressed
    GOTO IncreaseVal   
    
CheckButtonBPressed:
    MOVLW 0x02
    MOVWF REG24
    ;MOVF    REG24,0,1
    CPFSEQ REG25
    GOTO CheckBothButtonsPressed
    GOTO DecreaseVal

CheckBothButtonsPressed:
    MOVLW 0x03
    MOVWF REG24
    ;MOVF    REG24,0,1
    CPFSEQ REG25
    GOTO       START_MAIN_LOOP
    GOTO ResetVal

    
    
    
ReadSegmentDisplayTable:
    ;MOVF    REG22,0,1
    MOVFF   REG22,WREG
    ADDLW   0xF0
    MOVWF   TBLPTRL
    MOVLW   0x0F
    MOVWF   TBLPTRH
    MOVLW   0x0
    MOVWF   TBLPTRU
    TBLRD*
    MOVFF   TABLAT,WREG //OUTPUT
    MOVFF   WREG, REG23
    RETURN
    
UpdateSegmentDisplay:
    MOVFF REG23, PORTD
    RETURN
    
;---------------------
; ButtonsPressed
;---------------------
    
IncreaseVal:
    MOVLW 0x0F
    MOVWF REG24
    ;MOVF    REG24,0,1
    CPFSEQ REG22
    GOTO INCVAL
    GOTO SetValToZero

    
INCVAL:
    INCF REG22
    GOTO       START_MAIN_LOOP
    
DecreaseVal:
    MOVLW 0x00
    MOVWF REG24
    ;MOVF    REG24,0,1
    CPFSEQ REG22
    GOTO DECVAL
    GOTO SetValTo15

DECVAL:
    DECF REG22
    GOTO       START_MAIN_LOOP
    
ResetVal:
    MOVLW 0x00 ;set wreg to 0x00
    MOVWF 0x22 ;move wreg (0x00) into 0x22
    GOTO       START_MAIN_LOOP
    
NoUpdateVal:
    GOTO       START_MAIN_LOOP
    
SetValToZero:
    MOVLW 0x00 ;set wreg to 0x00
    MOVWF 0x22 ;move wreg (0x00) into 0x22
    GOTO       START_MAIN_LOOP
    
SetValTo15:
    MOVLW 0x0F ;set wreg to 0x0F
    MOVWF 0x22 ;move wreg (0x00) into 0x22
    GOTO       START_MAIN_LOOP

;---------------------
; SegmentData
;---------------------
    ORG 0x0FF0 ; Store 7-Segment Values starting at location 0x1FF0
SegmentDigits: DB 0x3f, 0x06, 0x5B, 0x4F ; 0-3
	       DB 0x66, 0x6D, 0x7D, 0x07 ; 4-7
 	       DB 0x7F, 0x67, 0x77, 0x7C ; 8-11
 	       DB 0x39, 0x5E, 0x79, 0x71 ; 12-15


