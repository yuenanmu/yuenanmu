#ifndef _motor_h
#define _motor_h

#include "zf_common_headfile.h"
//#define  LPWM TIM5_PWM_CH2_A1    // 定义左电机 PWM 通道
//#define  LDIR (A0)

//#define  RPWM TIM5_PWM_CH4_A3    // 定义右电机 PWM 通道
//#define  RDIR (A2)
#define PWM_2 TIM5_PWM_CH4_A3
#define DIR_2 (A2)

#define PWM_1 TIM5_PWM_CH2_A1
#define DIR_1 (A0)
#define SCALE 4//倍率放大器
typedef struct
{
	float speed;
	float speed_out;
	
  	float Ki;    
	float Kp; 
	float Kd;
	
	float Dir_Kd;  
	float Dir_Kp;
	
	float Dif_P;  //Differential Proportional
	float Dif_Speed;  
}MOTOR_PID;
typedef struct
{
  float kp;
  float kd;
  float kp2;
  float kd2;
  float ek;                      
  float ek1;                      
  float location_sum;            
  float out;
  float PID_I_LIMIT_MAX;
  float PID_OUT_LIMIT_MAX;	
}Turn_PPDD_LocTypeDef;
extern MOTOR_PID Motor_Pid;
extern Turn_PPDD_LocTypeDef Turn_PPDD_Loc;
extern float Desire_Speed;
extern int16 Motor_Pid_speed_Z; //ui调节的速
extern int16 Motor_Pid_Z_Ki;
extern int16 Motor_Pid_Z_Kp;
extern int16 Motor_Pid_Z_Kd;

extern int16 Motor_Pid_Dif_P;
extern int16 Motor_Pid_Z_Dir_Kd; 
extern int16 Motor_Pid_Z_Dir_Kp;


extern int16 Linear_speed;//直道速度
extern int16 Curve_speed; //弯道速度

extern int16 Up_Ramp_speed;//上坡速度
extern int16 Out_Ramp_speed;//下坡速度

//环岛速度
extern uint8 Ring_Speed;

extern int16 Ready_In_50_Ring_speed;
extern int16 In_50_Ring_speed;
extern int16 Ready_Out_50_Ring_speed;

extern int16 Ready_In_60_Ring_speed;
extern int16 In_60_Ring_speed;
extern int16 Ready_Out_60_Ring_speed;

extern int16 Ready_In_90_Ring_speed;
extern int16 In_90_Ring_speed;
extern int16 Ready_Out_90_Ring_speed;
/***************Motor_Control************/
extern uint8 stop_flag;
extern uint8 block_time; //堵转时间记录
extern uint8 stop_time;

/***************Motor_Control************/
extern uint8 start_go;
extern int16 PWM_L,PWM_R;

/*************Incremental_PI************/
extern int16 Motor_error_L,Motor_last_error_L;
extern int16 Motor_error_R,Motor_last_error_R;
extern float dir_out;
void Motor_All_Pid_init();
void Motor_PID_subsection();
void ds_motor_init(void);

void Motor_Control();
void Motor_Control_PwmOut(int16 OUT_L_SPEED, int16 OUT_R_SPEED);

int16 Position_PID(int encoder, int Target);//速度环
int16 Position_PI(int encoder, int Target);

float Position_PD (float err,int Target);//转向环
float turn_pid_location(void);
#endif