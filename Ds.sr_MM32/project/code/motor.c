#include "zf_common_headfile.h"
#include "motor.h"
//
uint8_t black_area=0;
uint8_t encoder_L=0,encoder_R=0,encoder=0;
uint8_t Straight,Ramp_flag,Cross_flag,Ring_state,Ring_left,Ring_right,curve,Angle;
/************************电机保护*********************/
uint8_t stop_flag=0;
uint8_t block_time=0;
uint8_t stop_time=0;
/************************发车控制*********************/
uint8 start=0;
int16 PWM_L=0,PWM_R=0;
/*************Incremental_PI************/
int16 Motor_error_L=0,Motor_last_error_L=0;
int16 Motor_error_R=0,Motor_last_error_R=0;
MOTOR_PID Motor_Pid;

/************************UI界面调节的速度环参数(按元素划分)*********************/
float Desire_Speed=0;
int16 Motor_Pid_speed_Z=0; //ui调节的速

int16 Motor_Pid_Z_L_Ki= 0;
int16 Motor_Pid_Z_L_Kp= 0;
int16 Motor_Pid_Z_R_Ki= 0;
int16 Motor_Pid_Z_R_Kp= 0;
//差速1
int16 Motor_Pid_Dif_P=0;
//直道弯道速度（十字元素）
int16 Linear_speed=0;//直道速度
int16 Curve_speed=0; //弯道速度
//坡道速度
int16 Up_Ramp_speed=0;//上坡速度
int16 Out_Ramp_speed=0;//下坡速度
//环岛速度
uint8 Ring_Speed=0;

int16 Ready_In_50_Ring_speed=0;
int16 In_50_Ring_speed=0;
int16 Ready_Out_50_Ring_speed=0;

int16 Ready_In_60_Ring_speed=0;
int16 In_60_Ring_speed=0;
int16 Ready_Out_60_Ring_speed=0;

int16 Ready_In_90_Ring_speed=0; //进圈
int16 In_90_Ring_speed=0;       //在圈
int16 Ready_Out_90_Ring_speed=0;//出圈

void Motor_PID_subsection()
{
	Motor_Pid.L_Ki=0.01*Motor_Pid_Z_L_Ki; 	
	Motor_Pid.L_Kp=0.1*Motor_Pid_Z_L_Kp; 	
	Motor_Pid.R_Ki=0.01*Motor_Pid_Z_R_Ki; 
	Motor_Pid.R_Kp=0.1*Motor_Pid_Z_R_Kp; 

	Motor_Pid.speed=1.0*Desire_Speed;
	
	Motor_Pid.Dif_P=0.00001*Motor_Pid_Dif_P;
}
void Motor_Pid_init()
{
	Motor_Pid.speed=0.0;

	Motor_Pid.speed_L=0.0;
	Motor_Pid.speed_R=0.0;

	Motor_Pid.L_Ki=0.0;    
	Motor_Pid.L_Kp=0.0;  
	Motor_Pid.R_Ki=0.0;  
	Motor_Pid.R_Kp=0.0;  
	
	Motor_Pid.Dif_Speed=0.0;
}
void Motor_Control()
{
/************************电机保护*********************/
    //原先没想到，学长提醒之下加入了出赛道保护
    if(black_area==1){
        stop_time++;
    }else{stop_time=0;}

    //电机不转但pid疯狂加电流，防止堵转
    if((encoder_L<=5&&PWM_L>9300)||(encoder_R<=5&&PWM_R>9300)&&start==1)
    {
        block_time++;
    }else{ block_time=0; }

    if(block_time>=4||stop_time>=8)
	{
		stop_flag=1;
	}
	if(stop_flag==1){
		start=0;
	}

	/************************速度决策，要综合考虑不同元素情况，flag最麻烦的一集*********************/
    Desire_Speed=Motor_Pid_speed_Z;
	if((encoder>Desire_Speed)&&start==1) //缓减
	{
		Motor_Pid.speed-=20;
		if(Motor_Pid.speed-Desire_Speed<20)
		{
			Motor_Pid.speed=Desire_Speed;
		}
	}
	if(Straight==1 && Ramp_flag==0 && Cross_flag==0 && Ring_state==0 && Ring_left==0 && Ring_right==0 && Ring_Speed==0)
	{
		Desire_Speed=Motor_Pid_speed_Z+Linear_speed;
	}
	if(curve==1 && Ramp_flag==0 && Cross_flag==0 && Ring_state==0 && Ring_left==0 && Ring_right==0 && Ring_Speed==0)
	{
		Desire_Speed=Motor_Pid_speed_Z-Curve_speed;
	}
	if(Ramp_flag==1)Desire_Speed=Up_Ramp_speed; 
	if(Ramp_flag==2)Desire_Speed=Out_Ramp_speed;
	
	if(Ring_Speed==1)Desire_Speed=Ready_In_50_Ring_speed;
	if(Ring_Speed==11)Desire_Speed=In_50_Ring_speed;
	if(Ring_Speed==12)Desire_Speed=Ready_Out_50_Ring_speed;
	
	if(Ring_Speed==2)Desire_Speed=Ready_In_60_Ring_speed;
	if(Ring_Speed==21)Desire_Speed=In_60_Ring_speed;
	if(Ring_Speed==22)Desire_Speed=Ready_Out_60_Ring_speed;
	
	if(Ring_Speed==3)Desire_Speed=Ready_In_90_Ring_speed;
	if(Ring_Speed==31)Desire_Speed=In_90_Ring_speed;
	if(Ring_Speed==32)Desire_Speed=Ready_Out_90_Ring_speed;

	Motor_PID_subsection();
	Motor_Pid.Dif_Speed=0.47*Motor_Pid.speed;
	//方向控制不是很有头绪？？？
	if(Angle>=0)  //右转
	{
		Motor_Pid.speed_L=Motor_Pid.speed;
		Motor_Pid.speed_R=Motor_Pid.speed-Motor_Pid.Dif_Speed*Motor_Pid.Dif_P;
	}
	if(Angle<0)
	{
		Motor_Pid.speed_L=Motor_Pid.speed+Motor_Pid.Dif_Speed*Motor_Pid.Dif_P;
		Motor_Pid.speed_R=Motor_Pid.speed;
	}
	Motor_Control_L(Motor_Pid.speed_L);
	Motor_Control_R(Motor_Pid.speed_R);
}
void Motor_Control_L(int16 OUT_L_SPEED){
	if(start==1)
	{
		Incremental_PI_L(encoder_L,Example_L);
	}
	else
	{
		PWM_L=0;
	}
	if(0<=PWM_L) //电机1   正转 设置占空比为 百分之 (9900/GTM_ATOM0_PWM_DUTY_MAX*100)
	{
		if(PWM_L>9900)
		{
			PWM_L=9900;
		}
		DIR_2 = 0;
		pwm_duty(PWM_2, PWM_L);
	}
	else                //电机1   反转
	{
		if(PWM_L<-9900)
		{
			PWM_L=-9900;
		}
		DIR_2 = 1;
		pwm_duty(PWM_2, -PWM_L);
	}
}


void Motor_Control_R(int16 OUT_L_SPEED);


void Incremental_PI_L (int encoder_L,int Target_L)  //速度环
{		
	Motor_error_L=Target_L-encoder_L;                //求出速度偏差，由测量值减去目标值
	PWM_L+=Motor_Pid.L_Kp*(Motor_error_L-Motor_last_error_L)+Motor_Pid.L_Ki*Motor_error_L;   //使用增量 PI 控制器求出电机 PWM。
	Motor_last_error_L=Motor_error_L;                       //保存上一次偏差
}

void Incremental_PI_R (int encoder_R,int Target_R)
{
	Motor_error_R=Target_R-encoder_R;                //求出速度偏差，由测量值减去目标值
	PWM_R+=Motor_Pid.R_Kp*(Motor_error_R-Motor_last_error_R)+Motor_Pid.R_Ki*Motor_error_R;   //使用增量 PI 控制器求出电机 PWM。
	Motor_last_error_R=Motor_error_R;                       //保存上一次偏差
}
