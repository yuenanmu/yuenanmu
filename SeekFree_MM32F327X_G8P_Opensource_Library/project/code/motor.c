#include "zf_common_headfile.h"
#define max_speed 90  // 定义最大速度，保护电机
uint8_t start_flag=1;
//电机初始化，驱动
void motor_init (void)  // 电机初始化
{
  IncrementalPI *L_PI = (IncrementalPI *)malloc(sizeof(IncrementalPI));  // 分配内存给左电机增量式PI控制器
  IncrementalPI *R_PI = (IncrementalPI *)malloc(sizeof(IncrementalPI));  // 分配内存给右电机增量式PI控制器
	IncrementalPI_Init(L_PI);
	IncrementalPI_Init(R_PI);
	  
	gpio_init(LDIR,GPO,GPIO_HIGH, GPI_PULL_UP);
    pwm_init        (LPWM, 17000, 0);  // 初始化左电机 TIM5_CH3_A2  // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
	
	gpio_init(LDIR,GPO,GPIO_HIGH, GPI_PULL_UP);
    pwm_init        (RPWM, 17000, 0);  // 初始化右电机 TIM5_CH3_A3
    Set_motor_speed(0, 0);                        // 设置初始速度为0
}
void Set_motor_speed(int32 L_Speed,int32 R_Speed)  // 设置电机速度
{
	
    //限幅以及方向处理
    if(L_Speed>=0)   //左电机
    {	
        if(L_Speed > max_speed) L_Speed = max_speed;  // 限制左电机速度最大值为10000
		gpio_set_level              (LDIR,GPIO_HIGH);
        pwm_set_duty    (LPWM, L_Speed*100);  // 设置左电机速度
    }
    else if(L_Speed<0)
    {
        if(L_Speed < -max_speed) L_Speed = -max_speed;  // 限制左电机速度最小值为-10000
		gpio_set_level              (LDIR,GPIO_LOW*100);
        pwm_set_duty    (LPWM, -L_Speed*100);  // 设置左电机速度
    }
    
    if(R_Speed>=0)   //右电机
    {
        if(R_Speed > max_speed) R_Speed = max_speed;  // 限制右电机速度最大值为10000
		gpio_set_level              (RDIR,GPIO_HIGH);
        pwm_set_duty    (RPWM, R_Speed*100);  // 设置右电机速度
    }
    else if(R_Speed<0)
    {
        if(R_Speed < -max_speed) R_Speed = -max_speed;  // 限制右电机速度最小值为-10000
		gpio_set_level              (RDIR,GPIO_LOW);
        pwm_set_duty    (RPWM, -R_Speed*100);  // 设置右电机速度
    }
    
}
//速度决策




//漂移算法
void drift_algorithm(void){
    // 这里可以实现漂移算法的逻辑
    // 根据传感器数据和控制算法调整电机速度
    // 例如：根据当前速度和目标速度计算偏差，调整电机输出
    // 可以使用增量式PI控制器来实现速度控制
    // L_PI->output = L_PI->Kp * (L_PI->target - L_PI->last_error) + L_PI->Ki * L_PI->target;
    // R_PI->output = R_PI->Kp * (R_PI->target - R_PI->last_error) + R_PI->Ki * R_PI->target;
}
