#ifndef _PID_H____
#define _PID_H____
typedef struct 
{
    float Kp,Ki;
    float target;
    float last_error;;
    float output;
}IncrementalPI;//增量式PI控制器

typedef struct 
{
    //比例项*3，积分项，微分项
    float Kp, Ki, Kd;
    float integral; // 积分限幅，记录自身状态
    float prev_error; // 上次误差，记录上次误差
    float integral_max; // 积分最大值
} PositionPID; // 位置PID控制器

void IncrementalPI_Init(IncrementalPI *pi);
float IncrementalPI_Contrl(IncrementalPI *pi, float current_speed);
#endif 