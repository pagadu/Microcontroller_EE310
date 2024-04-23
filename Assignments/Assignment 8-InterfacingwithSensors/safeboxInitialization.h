/* 
 * File:   SafeboxInitialization.h
 * Author: alexa
 *
 * Created on April 19, 2024, 3:05 PM
 */

#ifndef SAFEBOXINITIALIZATION_H
#define	SAFEBOXINITIALIZATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

int SEGDIS_val;
int LED_val;
int key_start;
int key1;
int key2;
int key_end;
int key_reset;
int key1_display;
int key2_display;
int num_pr1_pressed;
int num_pr2_pressed;
int safe_code;
int keypad_code;
int keypad_done;

void initializeVars() {
    SEGDIS_val = 0;
    LED_val = 0;
    key_start = 0;
    key1 = 0;
    key2 = 0;
    key_end = 0;
    key_reset = 0;
    key1_display = 0;
    key2_display = 0;
    num_pr1_pressed = 0;
    num_pr2_pressed = 0;
    safe_code = 0x00;
    keypad_code = 0x00;
    keypad_done = 0;
}


#endif	/* SAFEBOXINITIALIZATION_H */

