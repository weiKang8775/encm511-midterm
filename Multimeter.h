#ifndef MULTIMETER_H
#define	MULTIMETER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

void DiplayResistance(void);
void DisplayVoltage(void);
void DisplayPulse(void);
void Multimeter();

#endif	/* XC_HEADER_TEMPLATE_H */

