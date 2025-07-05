#ifndef _motor_h
#define _motor_h

#include "zf_common_headfile.h"

typedef struct
{
	float speed;
	float speed_L;
	float speed_R;
	
    float L_Ki;    
	float L_Kp;  
	float R_Ki;  
	float R_Kp;

	float Dif_P;  //Differential Proportional
	float Dif_Speed;  
}MOTOR_PID;

#endif