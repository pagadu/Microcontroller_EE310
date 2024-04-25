/* 
 * File:   SafeboxFunction.h
 * Author: alexa
 *
 * Created on April 19, 2024, 3:07 PM
 */

#ifndef SAFEBOXFUNCTION_H
#define	SAFEBOXFUNCTION_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4


// Define macros for GPIO ports
#define KEYPAD_PORT  PORTA
#define KEYPAD_TRIS  TRISA
#define KEYPAD_LAT   LATA
#define KEYPAD_ANSEL ANSELA
#define KEYPAD_ROW_PORT  PORTB

#define PR_PORT  PORTB
#define PR_TRIS  TRISB
#define PR_LAT   LATB
#define PR_ANSEL ANSELB

#define LED_PORT  PORTC
#define LED_TRIS  TRISC
#define LED_LAT   LATC
#define LED_ANSEL ANSELC

#define SEGDIS_PORT  PORTD
#define SEGDIS_TRIS  TRISD
#define SEGDIS_LAT   LATD
#define SEGDIS_ANSEL ANSELD

// Define macros for keypad buttons
#define ROWS 4
#define COLS 4

//This is not used but it is a good visual of the keypad
const char keypad[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//This is equivalent to the one above but is in decimal.
//This is used
const int keypad_dec[ROWS][COLS] = {
    {1, 2, 3, 10},
    {4, 5, 6, 11},
    {7, 8, 9, 12},
    {14, 0, 15, 13}
};

//This is equivalent to the one above but is in hex-decimal.
//This is used
const int keypad_dis[ROWS][COLS] = {
    {0x06, 0x5B, 0x4F, 0x77},
    {0x66, 0x6D, 0x7D, 0x7C},
    {0x07, 0x7F, 0x67, 0x39},
    {0x79, 0x3f, 0x71, 0x5E}
};

        
// Function prototypes
void delay(int num1, int num2);
void motor(int num1, int num2);
void buzzer(int num1, int num2);
void melody(int num1, int num2);
int keypad_dec_to_seg(int input_val);
void initializeVars(void);
int readKeypad();
void updateSegDis(int input_val);
void configure_ports();
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void);
void __interrupt(irq(default), base(0x4008)) DEFAULT_ISR(void);
void INTERRUPT_Initialize (void);
void PWM_Initialize (void);


void delay(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
       for (cnt2=0; cnt2<num2; cnt2++)
       {}
    }
}

void motor(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
       for (cnt2=0; cnt2<num2; cnt2++) {
           PORTAbits.RA5 = ((cnt2 < 1)) ;
       }
    }
}

void buzzer(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
       for (cnt2=0; cnt2<num2; cnt2++) {
           PORTAbits.RA4 = cnt1 & 0b00000001;
       }
    }
}

void melody(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
        buzzer(10,1000);
        delay (10,1000);
        buzzer(4,1000);
        delay (4,1000);
        buzzer(20,1000);
        delay (4,1000);
    }
}

int keypad_dec_to_seg(int input_val) {
    if (input_val == 0)  { return 0x3f; }
    if (input_val == 1)  { return 0x06; }
    if (input_val == 2)  { return 0x5B; }
    if (input_val == 3)  { return 0x4F; }
    if (input_val == 4)  { return 0x66; }
    if (input_val == 5)  { return 0x6D; }
    if (input_val == 6)  { return 0x7D; }
    if (input_val == 7)  { return 0x07; }
    if (input_val == 8)  { return 0x7F; }
    if (input_val == 9)  { return 0x67; }
    if (input_val == 10) { return 0x77; }
    if (input_val == 11) { return 0x7C; }
    if (input_val == 12) { return 0x39; }
    if (input_val == 13) { return 0x5E; }
    if (input_val == 14) { return 0x79; }
    if (input_val == 15) { return 0x71; }
    return 0x00;
}

//Reads the keypad
int readKeypad() {
    int button_pressed = 0; // Used to wait for button to be depressed
    int row, col; 
    int keypad_val;
    
    //Cycle through the columns
    for (col = 0; col < COLS; col++) {
        //clear all columns
        //KEYPAD_PORT = ((0b00000000) & 0b00001111);
        PORTAbits.RA0 = 0;
        PORTAbits.RA1 = 0;
        PORTAbits.RA2 = 0;
        PORTAbits.RA3 = 0;
        
        //Set only 1 column based on COL variable
        //KEYPAD_PORT = ((0b00000001 << col) & 0b00001111);
        PORTAbits.RA0 = (col == 0);
        PORTAbits.RA1 = (col == 1);
        PORTAbits.RA2 = (col == 2);
        PORTAbits.RA3 = (col == 3);
        
        //Read keypad rows 
        keypad_val = ((KEYPAD_ROW_PORT & 0b11110000) << 0);
        
        //Cycle through the rows
        for (row = 0; row < ROWS; row++) {
            //check to see if row is detected
            if ((keypad_val) == (0b00010000 << row)) {
                //Detected button pressed
                //LED_PORT = ((keypad_dec[row][col]) & 0b00001111); // DEBUG
                button_pressed = 1;
                
                //KEYPAD_PORT = 0b11110000;
                
                // Wait for button to be depressed
                while (button_pressed == 1) { 
                    keypad_val = ((KEYPAD_ROW_PORT & 0b11110000) << 0);
                    //if keypad is zero then the button was depressed
                    if (keypad_val == 0) {
                        button_pressed = 0;
                    }
                }
                
                //returns keypad decimal value
                return keypad_dec[row][col];
            }
        }
    }
    //reserved 16 as no button pressed
    return 16; // No key pressed
}

    void updateSegDis(int input_val) {
    // Convert result to binary and display on LEDs
    SEGDIS_PORT = (unsigned char) input_val;
    delay(1,10);
}

/////////////////////////////////////////////
//CONFIGURE PORTS
void configure_ports() {
    // Initialize ports for keypad
    KEYPAD_PORT = 0;
    KEYPAD_LAT = 0;
    KEYPAD_ANSEL = 0b00010000;
    KEYPAD_TRIS = 0b00000000; // Set first 4 pins as outputs for COL
                              // RA4 is Output for buzzer

    // Initialize ports for Photo-resistors
    PR_PORT = 0;
    PR_LAT = 0;
    PR_ANSEL = 0;
    INLVLB = 0b00000111; // trips at a higher voltage
    //INLVLB = 0b00000000; // trips at 1.4V, too sensitive
    PR_TRIS = 0b11111111; // Inputs for monitoring PRs
                          // bits[7:4] used for keypad row]
    
    // Initialize ports for LED, BUZZER, MOTOR
    LED_PORT = 0;
    LED_LAT = 0;
    LED_ANSEL = 0;
    LED_TRIS = 0b00000000; // Inputs for monitoring PRs
    
    // Initialize ports for 7-SEGMENT DISPLAY
    SEGDIS_PORT = 0;
    SEGDIS_LAT = 0;
    SEGDIS_ANSEL = 0;
    SEGDIS_TRIS = 0b00000000; // Outputs for 7 Segment Display
    
    }

// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
            if (PIR1bits.INT0IF == 1) { 
                if (keypad_done == 0) { // motor affect interrupts
                    if (num_pr2_pressed != 0) {
                        num_pr1_pressed = 1;
                        num_pr2_pressed = 0;
                        SEGDIS_val = keypad_dec_to_seg(num_pr1_pressed & 0xf);
                    } else {
                        num_pr1_pressed = num_pr1_pressed + 1;
                        SEGDIS_val = keypad_dec_to_seg(num_pr1_pressed & 0xf);
                    }
                }
                //PORTCbits.RC6 = 1;
                //delay(8,1000);
                //PORTCbits.RC6 = 0;
                PIR1bits.INT0IF = 0;
                PIR5bits.INT1IF = 0;
            }
}

// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT1),base(0x4008)) INT1_ISR(void)
{
            if (PIR5bits.INT1IF == 1) { 
                if (keypad_done == 0) { // motor affect interrupts
                    if (num_pr1_pressed != 0) {
                        num_pr2_pressed = num_pr2_pressed + 1;
                        SEGDIS_val = keypad_dec_to_seg(num_pr2_pressed & 0xf);
                    } else {
                        num_pr2_pressed = 0;
                        SEGDIS_val = keypad_dec_to_seg(num_pr2_pressed & 0xf);
                    }
                }
                //PORTCbits.RC6 = 1;
                //delay(8,1000);
                //PORTCbits.RC6 = 0;
                PIR1bits.INT0IF = 0;
                PIR5bits.INT1IF = 0;
            }
}

// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT2),base(0x4008)) INT2_ISR(void)
{
            if (PIR7bits.INT2IF == 1) { 
                if (keypad_done == 0) { // motor affect interrupts
                    melody(5,1000);
                }
                //PORTCbits.RC6 = 1;
                //delay(8,1000);
                //PORTCbits.RC6 = 0;
                PIR7bits.INT2IF = 0;
            }
}


void __interrupt(irq(default), base(0x4008)) DEFAULT_ISR(void)
{
        // Unhandled interrupts go here
}

void INTERRUPT_Initialize (void)
{   
    INTCON0bits.IPEN = 1; // Enable interrupt priority
    INTCON0bits.GIEH = 1; // Enable high priority interrupts
    INTCON0bits.GIEL = 1; // Enable low priority interrupts
   
    INTCON0bits.INT0EDG = 1; // Interrupt on rising edge of INT0 pin
    IPR1bits.INT0IP = 1; // External Interrupt 0 Interrupt Priority bit
    PIE1bits.INT0IE = 1; // External Interrupt 0 Enable bit

    INTCON0bits.INT1EDG = 1; // Interrupt on rising edge of INT1 pin
    IPR5bits.INT1IP = 1; //  External Interrupt 1 Interrupt Priority bit
    PIE5bits.INT1IE = 1; // External Interrupt 1 Enable bit
    
    INTCON0bits.INT2EDG = 1; // Interrupt on rising edge of INT2 pin
    IPR7bits.INT2IP = 1; //  External Interrupt 2 Interrupt Priority bit
    PIE7bits.INT2IE = 1; // External Interrupt 2 Enable bit
    
    PIR1bits.INT0IF = 0;  // Clear Interrupt 0
    PIR5bits.INT1IF = 0;  // Clear Interrupt 1
    PIR7bits.INT2IF = 0;  // Clear Interrupt 2
    
    // Change IVTBASE by doing the following
    // Set IVTBASEU to 0x00
    IVTBASEU = 0b00000000;
    // Set IVTBASEH to  0x40; 
    IVTBASEH = 0b01000000;

    IVTBASEL = 0b00001000;
}
 

#endif	/* SAFEBOXFUNCTION_H */

