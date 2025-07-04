#ifndef _motor_H____
#define _motor_H____
#define  LPWM TIM5_PWM_CH2_A1    // 定义左电机 PWM 通道
#define  LDIR (A0)

#define  RPWM TIM5_PWM_CH4_A3    // 定义右电机 PWM 通道
#define  RDIR (A2)

void motor_init();
void Set_motor_speed(int32 L_Speed,int32 R_Speed);

#endif 