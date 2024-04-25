//---------------------
// Title: SafeboxProject
//---------------------
// Program Details: The purpose of this program is to take inputs from
// photoresistors that are used to crack the SECRET_CODE that the keypad sets.
// When the SECRET_CODE is inputed correctly then it will trigger the motor.
// When the SECRET_CODE is inputed incorrectly then it will trigger the buzzer.
// 

// Inputs: RA0, RA1, RA2, RA3, RB0, RB1
// Outputs: RC4, RC5, RC6, RB2, RB3, RB4, RB5, RD0, RD1, RD2, RD3, RD4, RD5
//          RD6, RD7, RA4, RC5
// Date: April 23, 2024
// File Dependencies / Libraries: SafeboxConfig.h, SafeboxFunction.h,
//                                SafeboxInitialization.h
// Compiler: xc8, 2.46
// Author: Alexander Pagaduan
// Versions:
//       V1.0: Initial Draft
// Useful links: 
//       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
//       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
//       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 


//---------------------
// INITIALIZATION
//---------------------
#include<stdio.h>
#include<stdlib.h>
#include <xc.h>
#include "SafeboxConfig.h"
#include "SafeboxInitialization.h"
#include "SafeboxFunction.h"

void main() {
    //printf ("Start main\n");
    initializeVars();
    configure_ports();
    INTERRUPT_Initialize();
    
    chk_no_pressed_cnt = 0;
    chk_no_pressed_max_cnt = 4000;
    while (1) {
        updateSegDis(SEGDIS_val);
        keypad_code = readKeypad();
    
        if (keypad_code != 16) {
            SEGDIS_val = 0;
            num_pr1_pressed = 0;
            num_pr2_pressed = 0;
            num_pr1_pressed_prev = num_pr1_pressed;
            num_pr2_pressed_prev = num_pr2_pressed;
            something_pressed = 0;
            chk_no_pressed_cnt = 0;
            PORTCbits.RC4 = 0;
            PORTCbits.RC5 = 0;
            if (keypad_code == 10) {
                safe_code = 0x00;
            } else if (keypad_code == 14) {
                key_start = 1;
                key1 = 0;
                key2 = 0;
                key_end = 0;
            } else if ((key_start == 1) && (key2 == 0) && (keypad_code > 0) && (keypad_code < 5)) {
                key_start = 1;
                key1 = 0;
                key2 = keypad_code;
                key_end = 0;
            } else if ((key_start == 1) && (key2 != 0) && (keypad_code > 0) && (keypad_code < 5)) {
                key_start = 1;
                key1 = keypad_code;
                key2 = key2;
                key_end = 0;
            } else if ((key_start == 1) && (key1 != 0)  && (key2 != 0) && (keypad_code == 15)) {
                key_start = 1;
                key1 = key1;
                key2 = key2;
                key_end = 1;
                safe_code = ((key2 << 4) | (key1 << 0));
            } else if (keypad_code == 12) {
                 SEGDIS_val = keypad_dec_to_seg((safe_code >> 0) & 0x0f);
            } else if (keypad_code == 11) {
                 SEGDIS_val = keypad_dec_to_seg((safe_code >> 4) & 0x0f);
            }
        } 
        
        if ((num_pr1_pressed != 0) && (num_pr1_pressed == ((safe_code >> 0) & 0x0f))) {
            //PORTCbits.RC4 = 1;
        } else {
            //PORTCbits.RC4 = 0;
        }
        if ((num_pr2_pressed != 0) && (num_pr2_pressed == ((safe_code >> 4) & 0x0f))) {
            //PORTCbits.RC5 = 1;
        } else {
            //PORTCbits.RC5 = 0;
        }
  
        something_pressed = 0;
        if ((num_pr1_pressed != 0) && (num_pr1_pressed != num_pr1_pressed_prev)) {
            something_pressed = 1;
            num_pr1_pressed_prev = num_pr1_pressed;
        }
        if ((num_pr1_pressed != 0) && (num_pr1_pressed != num_pr1_pressed_prev)) {
            something_pressed = 1;
            num_pr2_pressed_prev = num_pr2_pressed;
        }
        
        if (something_pressed) {
            chk_no_pressed_cnt = 0;
        } else if (num_pr1_pressed != 0) {
            chk_no_pressed_cnt = chk_no_pressed_cnt + 1;
        }


        if (((num_pr1_pressed != 0) && (num_pr1_pressed == ((safe_code >> 0) & 0x0f))) &&
            ((num_pr2_pressed != 0) && (num_pr2_pressed == ((safe_code >> 4) & 0x0f)))) {
            keypad_done = 1;
            delay(1,1000);
            PORTCbits.RC4 = 0;
            PORTCbits.RC5 = 1;
            SEGDIS_val = keypad_dec_to_seg(8);
            updateSegDis(SEGDIS_val);
            motor(10,1000);
            PORTCbits.RC4 = 0;
            PORTCbits.RC5 = 0;
            SEGDIS_val = 0;
            num_pr1_pressed = 0;
            num_pr2_pressed = 0;
            num_pr1_pressed_prev = num_pr1_pressed;
            num_pr2_pressed_prev = num_pr2_pressed;
            something_pressed = 0;
            chk_no_pressed_cnt = 0;
            delay(1,1000);
            keypad_done = 0;
        }
        
        if ((((num_pr1_pressed != 0) && (num_pr1_pressed != ((safe_code >> 0) & 0x0f))) &&
             ((num_pr2_pressed != 0) && (num_pr2_pressed >  ((             4) & 0x0f)))) ||
            (chk_no_pressed_cnt >= chk_no_pressed_max_cnt)) {
            keypad_done = 1;
            delay(1,1000);
            PORTCbits.RC4 = 1;
            PORTCbits.RC5 = 0;
            SEGDIS_val = keypad_dec_to_seg(9);
            updateSegDis(SEGDIS_val);
            buzzer(10,1000);
            PORTCbits.RC4 = 0;
            PORTCbits.RC5 = 0;
            SEGDIS_val = 0;
            num_pr1_pressed = 0;
            num_pr2_pressed = 0;
            num_pr1_pressed_prev = num_pr1_pressed;
            num_pr2_pressed_prev = num_pr2_pressed;
            something_pressed = 0;
            chk_no_pressed_cnt = 0;
            delay(1,1000);
            keypad_done = 0;
        }

    }
    
}
