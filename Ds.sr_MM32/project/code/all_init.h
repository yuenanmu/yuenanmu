#ifndef _all_init_h
#define _all_init_h

#include "zf_common_headfile.h"
extern int16 encoder_L;
extern int16 encoder_R;
extern float encoder_integral;
extern float encoder;
extern float encoder_integra2;

void all_init(void);
void  ds_encoderCount(void);
#endif