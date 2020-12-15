 /*
 * File:   ADC.c
 * Author: Francis
 *
 * Created on November 23, 2020, 6:41 PM
 */


#include "xc.h"
#include "ADC.h"
#include "UART2.h"
#include "math.h"


uint16_t Do_ADC(uint16_t CH0SA_bits){   
    uint16_t ADCvalue; // 16 bit register used to hold ADC converted digital output ADC1BUF0
    /* -------------ADC INITIALIZATION ------------------*/
    AD1CON1bits.ADON = 1; //turn on ADC module
    AD1CON1bits.FORM = 0b00; //saves output of ADC as an int
        
    AD1CON1bits.SSRC = 0b111; //automatic conversion
    AD1CON1bits.ASAM = 0; //starts sampling when samp = 1
    
    AD1CON2bits.VCFG = 0b000; //selects AVDD, AVSS (supply voltage to PIC) as Vref
    AD1CON2bits.CSCNA = 0; //one input for ADC

    AD1CON2bits.BUFM = 0;
    AD1CON2bits.ALTS = 0; //uses MUX A

    AD1CON3bits.ADRC = 0; //use microcontroller's system clock
    AD1CON3bits.SAMC = 0b11111;
    
    AD1CHSbits.CH0NA = 0;
    
    //Positive input select
    AD1CHSbits.CH0SA = CH0SA_bits;        
    
    AD1PCFGbits.PCFG5 = 0;
    AD1CSSLbits.CSSL5 = 0;    
   
    /* -------------ADC SAMPLING AND CONVERSION ------------------*/
    AD1CON1bits.SAMP=1; //Start Sampling, Conversion starts automatically after SSRC and SAMC settings
    while(AD1CON1bits.DONE==0)
    {}
    ADCvalue= ADC1BUF0; // ADC output is stored in ADC1BUF0 as this point
    AD1CON1bits.SAMP=0; //Stop sampling
    AD1CON1bits.ADON=0; //Turn off ADC, ADC value stored in ADC1BUF0;
    return (ADCvalue); //returns 10 bit ADC output stored in ADC1BIF0 to calling function
    
 }



        