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
;	V1.2: Added comments and cleaned up code
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
#define refTemp 45 ;User sets the value to desired room temp
#define measuredTemp 20 ;the thermostat, measures the room temp
    
;---------------------
; DEFINITIONS
;---------------------
#define SWITCH    LATD,2  ;unused
#define LED0      PORTD,0 ;unused
#define LED1	  PORTD,1 ;unused
		
    
;---------------------
; PROGRAM CONSTANTS
;---------------------

REG20 EQU 0x20	;refTemp value stored in REG20
REG21 EQU 0x21	;measuredTemp value stored in REG21
REG22 EQU 0x22	;contReg Control Reg value stored in REG22
;storing value 00b means turn off both heating and cooling;
; storing 01b, bit[0] is on so that turns on POPRTD.0 (connected to heating)
; storing 10b, bit[1] is on so that turns on POPRTD.1 (connected to cooling)
; should make sure we never program 11b,
;     that will turn on both heating and cooling at the same time

;The values that come out of the converter are stored here.
REG60 EQU 0x60	;refTempDigitOne
 ;The most significant bit MSB for refTemp
REG61 EQU 0x61	;refTempDigitTwo
 ;The middle bit for refTemp
REG62 EQU 0x62	;refTempDigitThree
;The least significant bit LSB for refTemp

;The values that come out of the converter are stored here.
REG70 EQU 0x70	;measuredTempDigitOne
 ;The most significant bit MSB for measuredTemp
REG71 EQU 0x71	;measuredTempDigitTwo
  ;The middle bit for measuredTemp
REG72 EQU 0x72	;measuredTempDigitThree
;The least significant bit LSB for measuredTemp

;Used for math done in the converter (Division)
;The remainder values will be copy to the REGs above after processing
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
    
    ;Go to the first task GetInputs
    GOTO        GetInputs
    
    ORG         0020H           ; Begin assembly at 0020H
    
    ;Assignment said to start at 0020H


;---------------------
; TASK; Convert Hex value to decimal
;---------------------
 
;Taken code from Apendex page 180
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
    
    ; SET PORTD[7:2] to inputs
    ; SET PORTD[1:0] to outputs
    MOVLW       0b11111100 ; Setting the mode of D pins
			   ; We are using the bits 0 and 1, and setting
			   ; them to zero so we can control the outputs
			   ;to control the heating and cooling
    MOVWF       TRISD,0 ; apply the above mode to TRISD
			;tristate control to select the port if its input
			;or output
			; since its 0 turns on output
    
    ;Store refTemp into REG20
    MOVLW refTemp ;Move value in refTemp into WREG
    MOVWF REG20 ;Taking user desired temp and storing in REG20
    
    ;Store measuredTemp into REG21
    MOVLW measuredTemp ;Move value in measuredTemp thermosat reading into WREG
    MOVWF REG21 ;Taking measured data and storing into REG21
    
    ;Store contReg into REG22 
    MOVLW 0x00 ; Used this to store the controls
		; Initial state is 0x0 meaning both heating and cooling off
    MOVWF REG22 ;Storing contReg into REG22
     
    ;Convert refTemp
    MOVFF REG20, MYNUM ; Taking refTemp and copying into MYNUM
			;This is the input into the converter
    CALL Converter ;Jump to Converter task
    MOVFF RMND_L, REG62 ; Taking the conveter outputs LSB and storing into REGs
    MOVFF RMND_M, REG61 ; Taking the conveter outputs Mid and storing into REGs
    MOVFF RMND_H, REG60 ;; Taking the conveter outputs MSB and storing into REGs
      
    ;Convert measuredTemp
    MOVFF REG21, MYNUM ;Taking measuredTemp and copying into MYNUM
			;This is the input into the converter
    CALL Converter ;Jump to Converter task
    MOVFF RMND_L, REG72 ; Taking the conveter outputs LSB and storing into REGs
    MOVFF RMND_M, REG71 ; Taking the conveter outputs Mid and storing into REGs
    MOVFF RMND_H, REG70; Taking the conveter outputs MSB and storing into REGs
    
    ;After storing and converting go to CompTemp
    GOTO CompTemp ;Jump to CompTemp
    
    
;---------------------
; TASK; COMPARATOR; COMPARE TEMPATURE
;---------------------
 
CompTemp:
    ;if refTemp is less than measured then branch to enable cooling
    ;if refTemp is greater than measured then branch to enable heating
    ;if Temps are equal stay in CompTemp waiting for a change in refTemp 
    ;or measured temp
    GOTO CompDigitOne ; Go check the MS_Digit bits of REF and measured temp
	    
CompDigitOne:
    ;if refTempDigitOne is less than measuredDigitOne then branch to
    ;	enable cooling
    MOVF REG70,0,1 ; Moved MS_Digit of REF to WREG
    CPFSLT REG60,1 ; Compare MS_Digit of MEASURED to WREG, ref < measured =cool
    GOTO CompDigitOneA ;If above is not true skip
    GOTO EnableCooling ; Otherwise true so jump to EnableCooling
 
 CompDigitOneA:
    ;if refTempDigitOne is greater than measuredDigitOne then branch to
    ;	enable heating
    MOVF REG70,0,1 ; Moved MS_Digit of REF to WREG
    CPFSGT REG60 ; Compare MS_Digit of MEASURED to WREG, ref > measured
    GOTO CompDigitOneB ;If above is not true skip
    GOTO EnableHeating ; Otherwise true so jump to EnableHeating
    
 CompDigitOneB:
    ;if Temps are equal jump to CompDigitTwo
    MOVF REG70,0,1 ; Moved MS_Digit of REF to WREG
    CPFSEQ REG60 ; Compare MS_Digit of MEASURED to WREG, ref = measured
    GOTO CompTemp ;If above is not true skip
    GOTO CompDigitTwo ; Otherwise true go to next digit compare
    
    ;Wont happen
    ;Just incase none of the above are true
    GOTO CompTemp

CompDigitTwo:
    ;if refTempDigitTwo is less than measuredDigitTwo then branch to
    ;	enable cooling
    MOVF REG71,0,1 ; Moved Mid_Digit of REF to WREG
    CPFSLT REG61,1 ; Compare Mid_Digit of MEASURED to WREG, ref < measured
    GOTO CompDigitTwoA ;If above is not true skip
    GOTO EnableCooling ; Otherwise true so jump to EnableCooling
 
 CompDigitTwoA:
    ;if refTempDigitTwo is greater than measuredDigitTwo then branch to
    ;	enable heating
    MOVF REG71,0,1 ; Moved Mid_Digit of REF to WREG
    CPFSGT REG61 ; Compare Mid_Digit of MEASURED to WREG, ref > measured
    GOTO CompDigitTwoB ;If above is not true skip
    GOTO EnableHeating ; Otherwise true so jump to EnableHeating
    
 CompDigitTwoB:
    ;if Temps are equal jump to CompDigitTwo
    MOVF REG71,0,1 ; Moved Mid_Digit of REF to WREG
    CPFSEQ REG61 ; Compare Mid_Digit of MEASURED to WREG, ref = measured
    GOTO CompTemp ;If above is not true skip
    GOTO CompDigitThree ; Otherwise true go to next digit compare
    
    ;Just incase none of the above are true
    GOTO CompTemp
    
 CompDigitThree:
    ;if refTempDigitThree is less than measuredDigitThree then branch to
    ;	enable cooling
    MOVF REG72,0,1 ; Moved LS_Digit of REF to WREG
    CPFSLT REG62,1 ; Compare LS_Digit of MEASURED to WREG, ref < measured
    GOTO CompDigitThreeA ;If above is not true skip
    GOTO EnableCooling ; Otherwise true so jump to EnableCooling
 
 CompDigitThreeA:
    ;if refTempDigitThree is greater than measuredDigitThree then branch to
    ;	enable heating
    MOVF REG72,0,1 ; Moved LS_Digit of REF to WREG
    CPFSGT REG62 ; Compare LS_Digit of MEASURED to WREG, ref > measured
    GOTO CompDigitThreeB ;If above is not true skip
    GOTO EnableHeating ; Otherwise true so jump to EnableHeating
    
 CompDigitThreeB:
    ;if Temps are equal jump to CompDigitThree
    MOVF REG72,0,1 ; Moved LS_Digit of REF to WREG
    CPFSEQ REG62 ; Compare LS_Digit of MEASURED to WREG, ref = measured
    GOTO CompTemp  ;If above is not true skip
    GOTO TurnOff ; Otherwise true then turn off
    
    ;Just incase none of the above are true
    GOTO CompTemp

    
;---------------------
; TASK; ENABLE COOLING SYSTEM
;---------------------
 
EnableCooling:
     ;only using lowest two bits of PORTD
    MOVLW 0x02 ;Setting 0x02 (10b) into contReg
    MOVWF REG22
    GOTO LEDS
;---------------------
; TASK; ENABLE HEATING SYSTEM
;---------------------
 
EnableHeating:
      ;only using lowest two bits of PORTD
    MOVLW 0x01 ;Setting 0x01 (01b) into contReg
    MOVWF REG22
    GOTO LEDS

;---------------------
; TASK; TURN OFF
;---------------------
 
TurnOff: ;turn off and go back to CompTemp
     ;only using lowest two bits of PORTD
    MOVLW 0x00 ;Setting 0x00 (00b) into contReg
    MOVWF REG22
    GOTO LEDS 
    
;---------------------
; TASK; SET LEDS
;---------------------
 
LEDS: ;set LEDS and Control Settings Heater or Cooler
    MOVF    REG22,0,1 ;Load contents of REG22 into WREG
    MOVFF   REG22,PORTD ; Load contents from WREG into PORTD
    ;GOTO EndTask ;Debug: Break the loop and only run once
    GOTO CompTemp ; Jump back to CompTemp check to restart check


END
