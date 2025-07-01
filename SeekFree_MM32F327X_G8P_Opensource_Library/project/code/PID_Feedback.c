#include"zf_common_headfile.h"
#define max_output 10000 // 定义最大输出值
#define IncrementalPI_Kp 0.1f // 比例系数
#define IncrementalPI_Ki 0.01f // 积分系数    
void IncrementalPI_Init(IncrementalPI *pi) { // 初始化增量式PI控制器
    pi->Kp = 1;     //未调整
    pi->Ki = 1; 
    pi->target = 0; 
    pi->last_error = 0; 
    pi->output = 0; 
}
float IncrementalPI_Contrl(IncrementalPI *pi, float current_speed) {        //电机转速控制
    float error = pi->target - current_speed; // 计算误差
    float delta_output = pi->Kp * (error - pi->last_error) + pi->Ki * error; // 计算增量(Kp*误差之差 + Ki * 误差)
    pi->output += delta_output; // 更新输出,当前的输出=上次输出+增量
    pi->last_error = error; // 更新上次误差
    if (pi->output > max_output) pi->output = max_output; // 限制输出最大值
    if (pi->output < -max_output) pi->output = -max_output; 
    return pi->output; // 返回当前输出
}
//int32 PD_Contrl(){
//    
//}
int32 PositionPID_Contrl(PositionPID*pid ,float target,float current ){     //车身角度控制
    float error = target-current; // 计算误差
    //积分项计算(带限幅)
    pid->integral += error; // 积分
    if(pid->integral>pid->integral_max) pid->integral = pid->integral_max;
    if(pid->integral<-pid->integral_max) pid->integral = -pid->integral_max; 
    //微分项
    float derivative =error -pid->prev_error;
    pid->prev_error = error; // 迭代误差
    //输出计算
    float output =pid->Kp*error +
                  pid->Ki*pid->integral+
                  pid->Kd*derivative; 
     return output; // 返回输出
}