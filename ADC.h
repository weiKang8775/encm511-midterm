#ifndef ADC_H
#define ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */
void ADC_Init(void);
uint16_t Do_ADC(uint16_t CH0SA_bits);
uint16_t Get_Amplitude(void);
#endif	/* ADC_H */

