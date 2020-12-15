/*
 * File:   IOs.c
 * Author: Francis
 *
 * Created on November 22, 2020, 9:36 PM
 */

#include "xc.h"
#include "IOs.h"

extern uint16_t multimeter_mode;

void IOinit(void){
    TRISBbits.TRISB8 = 0; //make GPIO RB8 as a digital output
    LATBbits.LATB8 = 0; //make GPIO RB8 as a digital output
    
    TRISAbits.TRISA4 = 1; //makes GPIO RA4 as a digital input (PB2)
    CNPU1bits.CN0PUE = 1; //enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1;  //enable CN on CN0
    
    TRISBbits.TRISB4 = 1; //makes GPIO RB4 as a digital input (PB3)
    CNPU1bits.CN1PUE = 1; //enables pull up resistor on RA4/CN0
    CNEN1bits.CN1IE = 1;  //enable CN on CN1
    
    TRISAbits.TRISA2 = 1; //makes GPIO RA2 as a digital input (PB1)
    CNPU2bits.CN30PUE = 1; //enables pull up resistor on RA4/CN0
    CNEN2bits.CN30IE = 1;  //enable CN on CN30
    
    //CN Interrupt settings
    IPC4bits.CNIP = 6; //7 is highest priority, 1 is lowest, 0 is disabled interrupt
    IFS1bits.CNIF = 0; //clear interrupt flag
    IEC1bits.CNIE = 1; //enable CN interrupts    
}

void IOcheck(void){
    

    if ((PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 0)){ //PB1(RA2) is pressed
        multimeter_mode = 1;
    }    
    else if ((PORTBbits.RB4 == 1) && (PORTAbits.RA4 == 0) && (PORTAbits.RA2 == 1)){ //PB2(RA4) is pressed
        multimeter_mode = 2;
    }  
    else if ((PORTAbits.RA2 == 1) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 0)){ //PB3(RB4) is pressed
        multimeter_mode = 3;
    }
    else {
        multimeter_mode = 0;
    }
}

//CN interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
	IFS1bits.CNIF = 0; //clear CN flag
    IOcheck();
    Nop();
    return;
}
