#ifndef __HEM7200_H
#define __HEM7200_H 
#include "stm32f10x.h"  


void fetch_bp(void);
void disable_INT8(void);
void EXTI8_Init(void);
void EXTI0_Init(void);
void disable_INT0(void);
#endif
