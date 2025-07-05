#include "zf_common_headfile.h"
#include "all_init.h"
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
	
	
	
	//片上资源初始化
	pit_ms_init(TIM2_PIT, 1); 
	pit_ms_init(TIM5_PIT, 100);
	interrupt_set_priority(TIM2_IRQn, 0);
	interrupt_set_priority(TIM5_IRQn, 1);
	
}