#include "math.h"

#include "xc.h"
#include "ADC.h"
#include "IOs.h"
#include "UART2.h"
#include "ChangeClk.h"

uint16_t multimeter_mode = -1;
float vRef = 3.2;

void DisplayVoltage(void){
    uint16_t adc_output = Do_ADC(0b0101);
    float inv_Vres = 1023/vRef; 
    float voltage_reading = adc_output/inv_Vres; //converts adc output to voltage
    
    //Displays voltage reading
    NewClk(8);
    XmitUART2('\r', 1);
    Disp2String("VOLTMETER Voltage = ");
    Disp2Dec(voltage_reading);
    XmitUART2('V', 1);
    XmitUART2(' ', 5);
    NewClk(defaultClk);  
}

void DisplayResistance(void){
    uint16_t adc_output = Do_ADC(0b1011);
    float inv_Vres = 1023/vRef;
    float voltage_reading = adc_output/inv_Vres; //converts adc output to voltage
    float resistance_reading = voltage_reading / (vRef - voltage_reading) *  1000; //voltage divider formula
    
    //Displays resistance reading
    NewClk(8);
    XmitUART2('\r', 1);
    Disp2String("OHMMETER Resistance = ");
    Disp2Dec(resistance_reading);
    XmitUART2(234, 1);
    XmitUART2(' ', 5);
    NewClk(defaultClk);  
}

void DisplayPulse(void) {
    uint16_t amplitude = Get_Amplitude();
    float Vres = vRef / 1023;
    float voltage_reading = amplitude * Vres;

    NewClk(8);
    XmitUART2('\r', 1);
    Disp2String("Amplitude = ");
    Disp2Dec(voltage_reading);
    XmitUART2('V', 1);
    XmitUART2(' ', 5);
    NewClk(defaultClk);
}

void Multimeter(){
    if(multimeter_mode == 1){ //voltage mode
        DisplayVoltage();
    }
    else if(multimeter_mode == 2) { //resistance mode
        DisplayResistance();       
    }
    else if (multimeter_mode == 3) {
        DisplayPulse();
    }
    else{ //idle mode
        //Clears terminal
        NewClk(8);
        XmitUART2('\r', 1);
        XmitUART2(' ', 40);
        NewClk(32);  
        
        Idle();
    }
}
