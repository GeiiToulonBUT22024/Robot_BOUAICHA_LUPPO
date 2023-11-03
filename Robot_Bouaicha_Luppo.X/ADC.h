/* 
 * File:   ADC.h
 * Author: TP_EO_6
 *
 * Created on 2 novembre 2023, 09:51
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

void InitADC1(void);
void ADC1StartConversionSequence();
void ADCClearConversionFinishedFlag(void);
unsigned int * ADCGetResult(void);
unsigned char ADCIsConversionFinished(void);


#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

