#include "zf_common_headfile.h"
#include "motor.h"

#define MAX_DUTY 4000
//
uint8_t black_area=0;
//uint8_t Straight,Ramp_flag,Cross_flag;
uint8_t Ring_left,Ring_right,curve;
/************************电机保护*********************/
uint8_t stop_flag=0;
uint8_t block_time=0;
uint8_t stop_time=0;
/************************发车控制*********************/
uint8 start_go=0;
int16 PWM_L=0,PWM_R=0;
/*************Incremental_PI************/
int16 Motor_error_L=0,Motor_last_error_L=0;
int16 Motor_error_R=0,Motor_last_error_R=0;
MOTOR_PID Motor_Pid;
int8 Dir_last_error=0,Dir_error=0;
float Dif_Speed=0;//不赋值，输出的值很诡异。。不能是全局
float dir_out;
/************************UI界面调节的速度环参数(按元素划分)*********************/
float Desire_Speed=0;
int16 Motor_Pid_speed_Z=0; //ui调节的速

int16 Motor_Pid_Z_L_Ki= 0;
int16 Motor_Pid_Z_L_Kp= 0;
int16 Motor_Pid_Z_R_Ki= 0;
int16 Motor_Pid_Z_R_Kp= 0;
//差速1
int16 Motor_Pid_Z_Dir_Kd=0;
int16 Motor_Pid_Z_Dir_Kp=0;
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
	//BB();
	
	Motor_Pid.Dif_P=0.1*Motor_Pid_Dif_P;
	
	Motor_Pid.Dir_Kd=0.001*Motor_Pid_Z_Dir_Kd; 
	Motor_Pid.Dir_Kp=0.001*Motor_Pid_Z_Dir_Kp;
	//printf("OK1\n");
}
void Motor_Pid_init()
{

	Motor_Pid.speed=0.0;

	Motor_Pid.speed_L=0.0;
	Motor_Pid.speed_R=0.0;

//	Motor_Pid.L_Ki=1.0;    
//	Motor_Pid.L_Kp=1.0;  
	Motor_Pid.L_Ki=6.6;//3.84;//6.639;    
	Motor_Pid.L_Kp=15.0;//9;  
	Motor_Pid.R_Ki=6.1;  
	Motor_Pid.R_Kp=10.0;  
	//Dir_Kp=19.770
	//kd=1.48
	Motor_Pid.Dif_Speed=0.0;
}
void ds_motor_init(void){
	gpio_init(DIR_1,GPO,GPIO_HIGH, GPO_PUSH_PULL);
  pwm_init        (PWM_1, 17000, 0);  // 初始化左电机
	
	gpio_init(DIR_2,GPO,GPIO_HIGH, GPO_PUSH_PULL);
  pwm_init        (PWM_2, 17000, 0);  // 初始化右电机
}
void Motor_Control()
{
/************************电机保护*********************/
    //原先没想到，学长提醒之下加入了出赛道保护
    if(black_area==1){
        stop_time++;
    }else{stop_time=0;}

    //电机不转但pid疯狂加电流，防止堵转
    if((encoder_L<=5&&PWM_L>7000)||(encoder_R<=5&&PWM_R>7000)&&start_go==1)
    {
        block_time++;
    }else{ block_time=0; }

    if(block_time>=4||stop_time>=8)
	{
		stop_flag=1; 
	}
	if(stop_flag==1){
		start_go=0;
	}

	/************************速度决策，要综合考虑不同元素情况，flag最麻烦的一集*********************/
    Desire_Speed=Motor_Pid_speed_Z;
	if((encoder>Desire_Speed)&&start_go==1) //缓减
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
	if(start_go==1){
	int16 speed_out_L = Incremental_PI_L(encoder_L, Motor_Pid.speed); // 返回调节量
	int16 speed_out_R = Incremental_PI_R(encoder_R, Motor_Pid.speed); // 返回调节量
	
	dir_out = Position_PD(Track.Err, 0);
	
	PWM_L = speed_out_L + dir_out*Motor_Pid.Dif_P;
	PWM_R = speed_out_R - dir_out*Motor_Pid.Dif_P;
	}else
	{
	int16 speed_out_L = Incremental_PI_L(encoder_L, 0); // 返回调节量
	int16 speed_out_R = Incremental_PI_R(encoder_R, 0); // 返回调节量
	
	dir_out = Position_PD(Track.Err, 0);
	
	PWM_L = speed_out_L ;
	PWM_R = speed_out_R ;
	}
//调节速度环
//	if(start_go==1){
//	int16 speed_out_L = Incremental_PI_L(encoder_L, Motor_Pid.speed); // 返回调节量
//	int16 speed_out_R = Incremental_PI_R(encoder_R, Motor_Pid.speed); // 返回调节量
//	
//	dir_out = Position_PD(Track.Err, 0);

//	PWM_L = speed_out_L;
//	PWM_R = speed_out_R;
//	}else
//	{
//	int16 speed_out_L = Incremental_PI_L(encoder_L, 0); // 返回调节量
//	int16 speed_out_R = Incremental_PI_R(encoder_R, 0); // 返回调节量
//	
//	dir_out = Position_PD(Track.Err, 0);
//	
//	PWM_L = speed_out_L ;
//	PWM_R = speed_out_R ;
//	}
//调节方向环
//	if(start_go==1){
//	int16 speed_out_L = Incremental_PI_L(encoder_L, Motor_Pid.speed); // 返回调节量
//	int16 speed_out_R = Incremental_PI_R(encoder_R, Motor_Pid.speed); // 返回调节量
//	
//	dir_out = Position_PD(Track.Err, 0);

//	PWM_L = + dir_out*Motor_Pid.Dif_P;
//	PWM_R = - dir_out*Motor_Pid.Dif_P;
//	}else
//	{
//	int16 speed_out_L = Incremental_PI_L(encoder_L, 0); // 返回调节量
//	int16 speed_out_R = Incremental_PI_R(encoder_R, 0); // 返回调节量
//	
//	dir_out = Position_PD(Track.Err, 0);
//	
//	PWM_L = speed_out_L ;
//	PWM_R = speed_out_R ;
//	}
	Motor_Control_L(PWM_L);   
	Motor_Control_R(PWM_R); 
}
void Motor_Control_L(int16 OUT_L_PWM){
	if (OUT_L_PWM >= 0) {
		gpio_set_level(DIR_1, GPIO_HIGH);
		pwm_set_duty(PWM_1, OUT_L_PWM);
	}
	else if(OUT_L_PWM <0) {
			gpio_set_level(DIR_1, GPIO_LOW);
			pwm_set_duty(PWM_1, -OUT_L_PWM);
	}
}

void Motor_Control_R(int16 OUT_R_PWM){
		if (OUT_R_PWM >=0) {
			gpio_set_level(DIR_2, GPIO_HIGH);
			pwm_set_duty(PWM_2, OUT_R_PWM);
		}
		else if (OUT_R_PWM <0) {
				gpio_set_level(DIR_2, GPIO_LOW);
				pwm_set_duty(PWM_2, -OUT_R_PWM);
		}
}

int16 Incremental_PI_L (int encoder_L,int Target_L)  //速度环
{		
	Motor_error_L=(Target_L-encoder_L);                //求出速度偏差，由测量值减去目标值
	PWM_L+=Motor_Pid.L_Kp*(Motor_error_L-Motor_last_error_L)+Motor_Pid.L_Ki*Motor_error_L;   //使用增量 PI 控制器求出电机 PWM。
	Motor_last_error_L=Motor_error_L;                       //保存上一次偏差
	// 限幅防止PWM过高
	if (PWM_L > MAX_DUTY) PWM_L = MAX_DUTY;
	if (PWM_L < -MAX_DUTY) PWM_L = -MAX_DUTY;
	return PWM_L; // 返回PWM值
}

int16 Incremental_PI_R (int encoder_R,int Target_R)
{
	Motor_error_R=(Target_R-encoder_R);                //求出速度偏差，由测量值减去目标值
	PWM_R+=Motor_Pid.R_Kp*(Motor_error_R-Motor_last_error_R)+Motor_Pid.R_Ki*Motor_error_R;   //使用增量 PI 控制器求出电机 PWM。
	Motor_last_error_R=Motor_error_R;      	//保存上一次偏差
	//限幅
	if (PWM_R > MAX_DUTY) PWM_R = MAX_DUTY;
	if (PWM_R < -MAX_DUTY) PWM_R = -MAX_DUTY;
	return PWM_R; // 返回PWM值
}
float Position_PD (float err,int Target)//懒得改名字
{
	Dir_error=Target-err;
	Dif_Speed=Motor_Pid.Dir_Kd*(Dir_error-Dir_last_error)+Motor_Pid.Dir_Kp*Dir_error;   //使用位置式 PD 控制器求出电机 PWM
	Dir_last_error=Dir_error;                       //保存上一次偏差
	uint16 MAX_dif_speed=1000;
	//限幅
	if(Dif_Speed>=MAX_dif_speed)		Dif_Speed=MAX_dif_speed;
	else if(Dif_Speed<=-MAX_dif_speed)	Dif_Speed=-MAX_dif_speed;
	
	return Dif_Speed;
}
