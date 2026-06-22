#ifndef __THD_COMP_H__
#define __THD_COMP_H__

#include "main.h"
#include "ad9833.h"
#include "ADC_samp.h"
#include <math.h>

void THD_Comp_Init(void);
float THD_Calc(void);

#endif
