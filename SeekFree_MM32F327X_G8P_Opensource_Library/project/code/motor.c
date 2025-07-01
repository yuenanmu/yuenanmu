#include "zf_common_headfile.h"
#define  LPWM TIM5_PWM_CH3_A2    // 定义左电机 PWM 通道
#define  RPWM TIM5_PWM_CH4_A3    // 定义右电机 PWM 通道
void motor_init (void)  // 电机初始化
{
    IncrementalPI *L_PI = (IncrementalPI *)malloc(sizeof(IncrementalPI));  // 分配内存给左电机增量式PI控制器
    IncrementalPI *R_PI = (IncrementalPI *)malloc(sizeof(IncrementalPI));  // 分配内存给右电机增量式PI控制器
    pwm_init        (TIM5_PWM_CH3_A2, 17000, 0);  // 初始化左电机 TIM5_CH3_A2  // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    pwm_init        (TIM5_PWM_CH4_A3, 17000, 0);  // 初始化右电机 TIM5_CH3_A3
    Set_motor_speed(0, 0);                        // 设置初始速度为0
}
void Set_motor_speed(uint32 L_Speed,uint32 R_Speed)  // 设置电机速度
{
    //限幅以及方向处理
    if(L_Speed>=0)   //左电机
    {
        if(L_Speed > 10000) L_Speed = 10000;  // 限制左电机速度最大值为10000
        pwm_set_duty    (LPWM, L_Speed);  // 设置左电机速度
    }
    else if(L_Speed<0)
    {
        if(L_Speed < -10000) L_Speed = -10000;  // 限制左电机速度最小值为-10000
        pwm_set_duty    (LPWM, -L_Speed);  // 设置左电机速度
    }
    
    if(R_Speed>=0)   //右电机
    {
        if(R_Speed > 10000) R_Speed = 10000;  // 限制右电机速度最大值为10000
        pwm_set_duty    (RPWM, R_Speed);  // 设置右电机速度
    }
    else if(R_Speed<0)
    {
        if(R_Speed < -10000) R_Speed = -10000;  // 限制右电机速度最小值为-10000
        pwm_set_duty    (RPWM, -R_Speed);  // 设置右电机速度
    }
    
}