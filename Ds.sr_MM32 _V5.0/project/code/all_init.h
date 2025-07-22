#ifndef _all_init_h
#define _all_init_h

#include "zf_common_headfile.h"
#define BEEP                (D7 ) 
#define MAX_EXPECTED_ENCODER  24
extern int16 encoder_L;
extern int16 encoder_R;
extern float encoder_integra1;
extern float encoder;
extern float encoder_integra2;
extern float encoder_integra;

void all_init(void);
void ds_encoderCount(void);
void BB();
#endif