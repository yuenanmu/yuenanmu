#include "zf_common_headfile.h"
#include "all_init.h"
int16 encoder_L;
int16 encoder_R;
float encoder_integral=0,encoder=0,encoder_integra2=0;
void ds_encoder_init(void){
	encoder_quad_init(TIM3_ENCODER,TIM3_ENCODER_CH1_B4,TIM3_ENCODER_CH2_B5);
	encoder_quad_init(TIM4_ENCODER,TIM4_ENCODER_CH1_B6,TIM4_ENCODER_CH2_B7);
}
void  ds_encoderCount(void)
{	encoder_L=encoder_get_count(TIM3_ENCODER);
    encoder_clear_count(TIM3_ENCODER);
    encoder_R=encoder_get_count(TIM4_ENCODER);
    encoder_clear_count(TIM4_ENCODER);
	
	encoder=(encoder_L+encoder_R)*0.5;
	encoder_integral+=encoder*0.02;
	encoder_integral+=encoder*0.02;
}
void ds_key_init(void){
	gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY1 输入 默认高电平 上拉输入
	gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY2 输入 默认高电平 上拉输入
	gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY3 输入 默认高电平 上拉输入
	gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP); 
}
void all_init(void){
	
	//系统初始化
	clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
  debug_init();   
	system_delay_ms(300);
	
	//外设初始化
	ds_ips200_init();
	ds_key_init();
	ds_encoder_init();
	ds_motor_init();
	Motor_Pid_init();
	EepromRead();
	
	//片上资源初始化
	pit_ms_init(TIM2_PIT, 1);  //分时复用定时器
	pit_ms_init(TIM1_PIT, 100);
	interrupt_set_priority(TIM2_IRQn, 0);
	interrupt_set_priority(TIM1_UP_IRQn, 1);
	
}