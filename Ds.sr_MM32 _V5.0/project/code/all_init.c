#include "zf_common_headfile.h"
#include "all_init.h"
int16 encoder_L,encoder_L_last;
int16 encoder_R,encoder_R_last;
float encoder_integra1=0,encoder=0,encoder_integra2=0,encoder_integra=0;
void ds_encoder_init(void){
	encoder_quad_init(TIM3_ENCODER,TIM3_ENCODER_CH1_B4,TIM3_ENCODER_CH2_B5);
	encoder_quad_init(TIM4_ENCODER,TIM4_ENCODER_CH1_B6,TIM4_ENCODER_CH2_B7);
}
void  ds_encoderCount(void)
{	encoder_L=encoder_get_count(TIM3_ENCODER);
	encoder_clear_count(TIM3_ENCODER);
	encoder_R=-encoder_get_count(TIM4_ENCODER);
	encoder_clear_count(TIM4_ENCODER);
//	if(abs(encoder_L - encoder_L_last) > MAX_EXPECTED_ENCODER) {
//    encoder_L = encoder_L_last; // 或者直接跳过当前帧
//}
//	if(abs(encoder_R - encoder_R_last) > MAX_EXPECTED_ENCODER) {
//    encoder_R = encoder_R_last; // 或者直接跳过当前帧
//}
	encoder=(encoder_L+encoder_R)*0.5;
	encoder_integra1+=encoder_L*0.02;
	encoder_integra2+=encoder_R*0.02;
	
	encoder_integra+=encoder*0.02;

//	encoder_L_last=encoder_L;
//	encoder_R_last=encoder_R;
}
void ds_key_init(void){
	gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY1 输入 默认高电平 上拉输入
	gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY2 输入 默认高电平 上拉输入
	gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY3 输入 默认高电平 上拉输入
	gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP); 
	
	gpio_init(BEEP,GPO,GPIO_LOW,GPO_PUSH_PULL);																	//蜂鸣器
}
void beep_on(){
	gpio_set_level(BEEP,GPIO_HIGH);
}
void beep_off(){
	gpio_set_level(BEEP,GPIO_LOW);
}
void BB(){
	beep_on();
	system_delay_ms(500);
	beep_off();
}
void all_init(void){
	
	//系统初始化
	clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
  debug_init();   
	system_delay_ms(100);
	
	//外设初始化
	while(1)
  {
        if(mt9v03x_init())
        {
            ips200_show_string(0, 16, "mt9v03x reinit.");
        }
        else
        {
            break;
        }
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
  }
	wireless_uart_init();
  ds_ips200_init();
	ds_key_init();
	ds_encoder_init();
	ds_motor_init();
	Motor_Pid_init();
	//初始化之后读取flash并复制
	EepromRead();
	ips200_show_string(0, 16, "init success.");
	system_delay_ms(100); 
	ips200_clear();
	//片上资源初始化
	pit_ms_init(TIM2_PIT, 1); 
	pit_ms_init(TIM7_PIT, 100);
	interrupt_set_priority(TIM2_IRQn, 0);
	interrupt_set_priority(TIM7_IRQn, 1);
	
}