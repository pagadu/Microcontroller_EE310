;---------------------
; Title: ThermostatProject
;---------------------
; Program Details:
; The purpose of this project is to activate a cooling and heating system
; based on the measured temperatures and reference temperatures we set

; Inputs: refTemp and measuredTemp 
; Outputs: PORTD
; Date: March 6, 2024
; File Dependencies / Libraries: ThermostatConfig.inc 
; Compiler: xc8, 2.46
; Author: Alexander Pagaduan
; Versions:
;       V1.0: Initial Draft
;	V1.1: Program is working with fixed Inputs
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 


;---------------------
; INITIALIZATION
;---------------------
#include "ThermostatConfig.inc"
#include <xc.inc>
    
;---------------------
; PROGRAM INPUTS
;---------------------
#define refTemp 15
#define measuredTemp 5
    
;---------------------
; DEFINITIONS
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1	  PORTD,1
    
;---------------------
; PROGRAM CONSTANTS
;---------------------

REG20 EQU 0x20	;refTemp
REG21 EQU 0x21	;measuredTemp
REG22 EQU 0x22	;contReg
REG23 EQU 0x23	;EndTask

REG60 EQU 0x60	;refTempDigitOne
REG61 EQU 0x61	;refTempDigitTwo
REG62 EQU 0x62	;refTempDigitThree

REG70 EQU 0x70	;measuredTempDigitOne
REG71 EQU 0x71	;measuredTempDigitTwo
REG72 EQU 0x72	;measuredTempDigitThree

NUME	EQU 0x40    ;RAM location for NUME
QU	EQU 0x42    ;RAM location for quotient
RMND_L	EQU 0x44
RMND_M	EQU 0x46
RMND_H	EQU 0x48
MYNUM	EQU 0x4A    ;Store value to Convert Here
MYDEN	EQU 10	    ;Divide by 10


;---------------------
; MAIN
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG         0                ;Reset vector
    GOTO        GetInputs
    ORG         0020H           ; Begin assembly at 0020H


;---------------------
; TASK; Convert Hex value to decimal
;---------------------
 
Converter:
	MOVFF	MYNUM, NUME
	MOVLW	MYDEN
	CLRF	QU,a
	
D_1:	
	INCF	QU,a
	SUBWF	NUME
	BC	D_1
	ADDWF	NUME
	DECF	QU,a
	MOVFF	NUME, RMND_L
	MOVFF	QU,NUME
	CLRF	QU,a
	
D_2:	
	INCF	QU,a
	SUBWF	NUME
	BC	D_2
	ADDWF	NUME
	DECF	QU,a
	MOVFF	NUME,RMND_M
	MOVFF	QU,RMND_H
    
    RETURN
    

    
;---------------------
; TASK; Get Inputs
;---------------------
 
GetInputs:
    ;Reference Temp and Measured Temp
    
    ; Set EndTask
    MOVLW 0xDE
    MOVWF REG23
    
    ; SET PORTD[7:2] to inputs
    ; SET PORTD[1:0] to outputs
    MOVLW       0b11111100
    MOVWF       TRISD,0
    
    ;Store refTemp into REG20
    MOVLW refTemp
    MOVWF REG20
    
    ;Store measuredTemp into REG21
    MOVLW measuredTemp
    MOVWF REG21
    
    ;Store contTemp into REG22 
    MOVLW 0x00
    MOVWF REG22
     
    ;Convert refTemp
    MOVFF REG20, MYNUM
    CALL Converter
    MOVFF RMND_L, REG62
    MOVFF RMND_M, REG61
    MOVFF RMND_H, REG60
      
    ;Convert measuredTemp
    MOVFF REG21, MYNUM
    CALL Converter
    MOVFF RMND_L, REG72
    MOVFF RMND_M, REG71
    MOVFF RMND_H, REG70
    
    ;After storing and converting go to CompTemp
    GOTO CompTemp
    
    
;---------------------
; TASK; COMPARATOR; COMPARE TEMPATURE
;---------------------
 
CompTemp:
    ;if refTemp is less than measured then branch to enable cooling
    ;if refTemp is greater than measured then branch to enable heating
    ;if Temps are equal stay in CompTemp waiting for a change in refTemp 
    ;or measured temp
    GOTO CompDigitOne
	    
CompDigitOne:
    ;if refTempDigitOne is less than measuredDigitOne then branch to
    ;	enable cooling
    MOVF REG70,0,1
    CPFSLT REG60,1
    GOTO CompDigitOneA
    GOTO EnableCooling
 
 CompDigitOneA:
    ;if refTempDigitOne is greater than measuredDigitOne then branch to
    ;	enable heating
    MOVF REG70,0,1
    CPFSGT REG60
    GOTO CompDigitOneB
    GOTO EnableHeating
    
 CompDigitOneB:
    ;if Temps are equal jump to CompDigitTwo
    MOVF REG70,0,1
    CPFSEQ REG60
    GOTO CompTemp
    GOTO CompDigitTwo
    
    ;Just incase none of the above are true
    GOTO CompTemp

CompDigitTwo:
    ;if refTempDigitTwo is less than measuredDigitTwo then branch to
    ;	enable cooling
    MOVF REG71,0,1
    CPFSLT REG61,1
    GOTO CompDigitTwoA
    GOTO EnableCooling
 
 CompDigitTwoA:
    ;if refTempDigitTwo is greater than measuredDigitTwo then branch to
    ;	enable heating
    MOVF REG71,0,1
    CPFSGT REG61
    GOTO CompDigitTwoB
    GOTO EnableHeating
    
 CompDigitTwoB:
    ;if Temps are equal jump to CompDigitTwo
    MOVF REG71,0,1
    CPFSEQ REG61
    GOTO CompTemp
    GOTO CompDigitThree
    
    ;Just incase none of the above are true
    GOTO CompTemp
    
 CompDigitThree:
    ;if refTempDigitThree is less than measuredDigitThree then branch to
    ;	enable cooling
    MOVF REG72,0,1
    CPFSLT REG62,1
    GOTO CompDigitThreeA
    GOTO EnableCooling
 
 CompDigitThreeA:
    ;if refTempDigitThree is greater than measuredDigitThree then branch to
    ;	enable heating
    MOVF REG72,0,1
    CPFSGT REG62
    GOTO CompDigitThreeB
    GOTO EnableHeating
    
 CompDigitThreeB:
    ;if Temps are equal jump to CompDigitThree
    MOVF REG72,0,1
    CPFSEQ REG62
    GOTO CompTemp
    GOTO TurnOff
    
    ;Just incase none of the above are true
    GOTO CompTemp

;---------------------
; TASK; ENABLE COOLING SYSTEM
;---------------------
 
EnableCooling:
    MOVLW 0x01
    MOVWF REG22
    GOTO LEDS
    
;---------------------
; TASK; ENABLE HEATING SYSTEM
;---------------------
 
EnableHeating:
    MOVLW 0x02
    MOVWF REG22
    GOTO LEDS
    
;---------------------
; TASK; TURN OFF
;---------------------
 
TurnOff: ;turn off and go back to CompTemp
    MOVLW 0x00
    MOVWF REG22
    GOTO LEDS
    
;---------------------
; TASK; SET LEDS
;---------------------
 
LEDS: ;set LEDS
    MOVF REG22,0,1 ;Load contents of REG22 into WREG
    MOVFF       REG22,PORTD ; Load contents from WREG into PORTD
    ;GOTO EndTask
    GOTO CompTemp

 ;---------------------
; TASK; END
;---------------------
 
EndTask: ;EndTask
    MOVLW 0xED
    MOVWF REG23
    SLEEP
    
END
    
