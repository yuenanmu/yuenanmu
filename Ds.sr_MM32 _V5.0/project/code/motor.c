#include "zf_common_headfile.h"
#include "motor.h"

#define MAX_speed 3500
#define MAX_INTEGRAL 600//560//480//360
#define MAX_dif_speed 3000 
#define MAX__DUTY 6000
float Err0,Err1,ErrI,Err2;
//
uint8_t Ring_left=0,Ring_right=0,curve=0;
/************************发车控制*********************/
uint8 start_go=0;
int16 PWM_L=0,PWM_R=0;
/*************速度环&&转项环************/
MOTOR_PID Motor_Pid;
Turn_PPDD_LocTypeDef Turn_PPDD_Loc;

//无陀螺仪转向环
int8 Dir_last_error=0,Dir_error=0;
float Dif_Speed=0;//不赋值，输出的值很诡异。。不能是全局
float dir_out;
/************************UI界面调节的速度环参数(按元素划分)*********************/
float Desire_Speed=0;
int16 Motor_Pid_speed_Z=0; //ui调节的速

int16 Motor_Pid_Z_Ki= 0;
int16 Motor_Pid_Z_Kp= 0;
int16 Motor_Pid_Z_Kd= 0;
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

void Motor_PID_subsection()//flash原本数据导致变量是“-nan”,无法修改。取消这个函数注释
{
//	Motor_Pid_speed_Z=0;
//	Motor_Pid.Ki=0.01*Motor_Pid_Z_Ki; 	
//	Motor_Pid.Kp=0.1*Motor_Pid_Z_Kp; 
//	Motor_Pid.Kd=0.1*Motor_Pid_Z_Kd; 

//	Motor_Pid.speed=1.0*Desire_Speed;
//	
//	Motor_Pid.Dif_P=0.1*Motor_Pid_Dif_P;
//	
//	Motor_Pid.Dir_Kd=0.001*Motor_Pid_Z_Dir_Kd; 
//	Motor_Pid.Dir_Kp=0.001*Motor_Pid_Z_Dir_Kp;
//	Turn_PPDD_Loc.kd=0.0;
//	Turn_PPDD_Loc.kp=0.0;
//	Turn_PPDD_Loc.kd2=0.0;
//	Turn_PPDD_Loc.kp2=0.0;
		//foresight_line=40;
}
void Motor_All_Pid_init()
{
	Motor_Pid_speed_Z=200;
	Motor_Pid.speed=-2;

	Motor_Pid.Ki=10.0;//3.84;//6.639;    
	Motor_Pid.Kp=8.0;//9; 
	Motor_Pid.Kd=16.0;	

	Motor_Pid.Dif_Speed=0.0;

	Turn_PPDD_Loc.kd=8.0f;
	Turn_PPDD_Loc.kp=0.0f;
	Turn_PPDD_Loc.kd2=0.0f;
	Turn_PPDD_Loc.kp2=0.01f;

}
void ds_motor_init(void){
	gpio_init(DIR_1,GPO,GPIO_HIGH, GPO_PUSH_PULL);
  pwm_init        (PWM_1, 17000, 0);  // 初始化左电机
	
	gpio_init(DIR_2,GPO,GPIO_HIGH, GPO_PUSH_PULL);
  pwm_init        (PWM_2, 17000, 0);  // 初始化右电机
}
void Motor_Control()
{
	/************************速度决策*********************/
    Desire_Speed=Motor_Pid_speed_Z;
	if((encoder>Desire_Speed)&&start_go==1) //缓减
	{
		Motor_Pid.speed-=20;
		if(Motor_Pid.speed-Desire_Speed<20)
		{
			Motor_Pid.speed=Desire_Speed;
		}
	}
	if(Straight_Flag==1 && Ramp_flag==0 && Cross_flag==0 && Ring_state==0 && Ring_left==0 && Ring_right==0 && Ring_Speed==0)
	{
		Desire_Speed=Motor_Pid_speed_Z+Linear_speed;
	}
	if(Curve_Flag==1 && Ramp_flag==0 && Cross_flag==0 && Ring_state==0 && Ring_left==0 && Ring_right==0 && Ring_Speed==0)
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

	Motor_Pid.speed=1.0*Desire_Speed;
	if(start_go==1){
	int16 speed_out = Position_PID((encoder_R+encoder_L)>>1, Motor_Pid.speed); // 返回调节量
	dir_out = turn_pid_location(); 
	// 计算差速
	float diff_output = dir_out * Motor_Pid.Dif_P;
	// 限制差速不超过基础速度的80%，防止反转
	float max_diff = speed_out * 0.8;
	if (fabs(diff_output) > max_diff) {
	diff_output = (diff_output > 0) ? max_diff : -max_diff;
	}
//	PWM_L = speed_out;//+diff_output;
//	PWM_R = speed_out;//-diff_output;
	PWM_L = speed_out+diff_output;
	PWM_R = speed_out-diff_output;
	//BB();
	}else
	{
	int16 speed_out = Position_PID((encoder_R+encoder_L)>>1, 0); // 返回调节量
//	dir_out = turn_pid_location(); 
//	// 计算差速
//	float diff_output = dir_out * Motor_Pid.Dif_P;
//	// 限制差速不超过基础速度的80%，防止反转
//	float max_diff = speed_out * 0.8;
//	if (fabs(diff_output) > max_diff) {
//	diff_output = (diff_output > 0) ? max_diff : -max_diff;
//	}
	PWM_L = speed_out;//+diff_output;
	PWM_R = speed_out;//-diff_output;
	}
	Motor_Control_PwmOut(PWM_L, PWM_R); 
}
void Motor_Control_PwmOut(int16 OUT_L_PWM, int16 OUT_R_PWM){
	
	if (OUT_L_PWM >= 0) {
		OUT_L_PWM=OUT_L_PWM > MAX__DUTY? MAX__DUTY:OUT_L_PWM;
		//gpio_set_level(DIR_1, GPIO_HIGH);
		gpio_set_level(DIR_1, GPIO_LOW);
		pwm_set_duty(PWM_1, OUT_L_PWM);
	}
	else if(OUT_L_PWM <0) {
		OUT_L_PWM=OUT_L_PWM < -MAX__DUTY? -MAX__DUTY:OUT_L_PWM;
		gpio_set_level(DIR_1, GPIO_HIGH);
		//gpio_set_level(DIR_1, GPIO_LOW);
			pwm_set_duty(PWM_1, -OUT_L_PWM);
	}
	if (OUT_R_PWM >=0) {
		OUT_R_PWM=OUT_R_PWM > MAX__DUTY? MAX__DUTY:OUT_R_PWM;
		gpio_set_level(DIR_2, GPIO_HIGH);
		pwm_set_duty(PWM_2, OUT_R_PWM);
	}
	else if (OUT_R_PWM <0) {
		OUT_R_PWM=(OUT_R_PWM < -MAX__DUTY)? -MAX__DUTY:OUT_R_PWM;
			gpio_set_level(DIR_2, GPIO_LOW);
			pwm_set_duty(PWM_2, -OUT_R_PWM);
	}
}

/***********************************速度环PID   *************************************************/

int16 Position_PID(int encoder, int Target)
{
    static int16 Err0 = 0;  // 当前误差（左电机）
    static int16 Err1 = 0;  // 上一次误差（左电机）
    static int16 ErrI = 0;  // 积分项（左电机）

    int16 Out_PWM;

    Err0 = Target - encoder;

    if (Motor_Pid.Ki!= 0){
        ErrI += Err0;
			
			if (ErrI > MAX_INTEGRAL)
        ErrI = MAX_INTEGRAL;
    else if (ErrI< -MAX_INTEGRAL)
        ErrI = -MAX_INTEGRAL;
		}
    else
        ErrI = 0;

    Out_PWM = Motor_Pid.Kp * Err0
              + Motor_Pid.Ki * ErrI
              + Motor_Pid.Kd * (Err0 - Err1);  // 若左右微分系数共用，可保留R_Kd；否则改为L_Kd

    if (Out_PWM > MAX_speed)
        Out_PWM = MAX_speed;
    else if (Out_PWM < -MAX_speed)
        Out_PWM = -MAX_speed;
		
		Err1 = Err0;

    return Out_PWM;
}


int16 Position_PI(int encoder, int target)
{
    static int16 err = 0;   // 当前误差
    static int16 errI = 0;  // 积分项

    int16 out_pwm;

    err = target - encoder;

    // 计算积分项（如果 Ki 不为 0 才允许积分）
    if (Motor_Pid.Ki != 0)
        errI += err;
    else
        errI = 0;

    // 限制积分项范围，防止积分饱和
    if (errI > MAX_INTEGRAL)
        errI = MAX_INTEGRAL;
    if (errI < -MAX_INTEGRAL)
        errI = -MAX_INTEGRAL;

    // PI 输出计算（无 D 项）
    out_pwm = Motor_Pid.Kp * err + Motor_Pid.Ki * errI;

    // 输出限幅，防止超过最大PWM
    if (out_pwm > MAX_speed)
        out_pwm = MAX_speed;
    if (out_pwm < -MAX_speed)
        out_pwm = -MAX_speed;

    return out_pwm;
}

/***************************转向环******************************************************/
float Position_PD (float err,int Target)
{
	Dir_error=Target-err;
	Dif_Speed=Motor_Pid.Dir_Kd*(Dir_error-Dir_last_error)+Motor_Pid.Dir_Kp*Dir_error;   //使用位置式 PD 控制器求出电机 PWM
	Dir_last_error=Dir_error;                       //保存上一次偏差

	//限幅
	if(Dif_Speed>=MAX_dif_speed)		Dif_Speed=MAX_dif_speed;
	else if(Dif_Speed<=-MAX_dif_speed)	Dif_Speed=-MAX_dif_speed;
	
	return Dif_Speed;
}
//加上陀螺仪的PPDD控制
float PPDD_location(float setvalue, float actualvalue, float GZ)
{
    Turn_PPDD_Loc.ek = setvalue - actualvalue;
    Turn_PPDD_Loc.location_sum += Turn_PPDD_Loc.ek; // 计算累计误差值                         
    // out计算
    Turn_PPDD_Loc.out = Turn_PPDD_Loc.kp * Turn_PPDD_Loc.ek
                      + Turn_PPDD_Loc.kp2 * fabs(Turn_PPDD_Loc.ek) * Turn_PPDD_Loc.ek
                      + (Turn_PPDD_Loc.ek - Turn_PPDD_Loc.ek1) * Turn_PPDD_Loc.kd
                      + GZ * Turn_PPDD_Loc.kd2; // 极性调整处
    Turn_PPDD_Loc.ek1 = Turn_PPDD_Loc.ek;
    // PID限幅
    if (Turn_PPDD_Loc.out < -MAX_dif_speed) Turn_PPDD_Loc.out = -MAX_dif_speed;
    if (Turn_PPDD_Loc.out > MAX_dif_speed)  Turn_PPDD_Loc.out = MAX_dif_speed;

    return Turn_PPDD_Loc.out;
}
float turn_pid_location(void)
{
	//imu660ra_gyro_x=0; // 没陀螺仪,先为零.陀螺仪数据清零
	dir_out=-PPDD_location(0,Track.Err,gz);//转向环
  return  dir_out;
}