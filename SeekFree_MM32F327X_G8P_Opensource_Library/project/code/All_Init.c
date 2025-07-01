#include "zf_common_headfile.h"
int32 encoder1;
int32 encoder2;
int8 offset;

uint32 key1_count;
uint32 key2_count;
uint32 key3_count;
uint32 key4_count;
uint8  key1_flag;
uint8  key2_flag;
uint8  key3_flag;
uint8  key4_flag;
uint32 count_time;
void Key_init(void)
{
    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY1 输入 默认高电平 上拉输入   按下时为0，!gpio_get_level(KEY1)
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY2 输入 默认高电平 上拉输入
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY3 输入 默认高电平 上拉输入
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);                               // 初始化 KEY4 输入 默认高电平 上拉输入
	
	  gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
    gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED2 输出 默认高电平 推挽输出模式
}
void all_init(void)
{
	 count_time=0;
	 offset=0;
    //系统初始化
	clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART
	system_delay_ms(300);
	//外设初始化
	ips200_init(IPS200_TYPE_SPI);                   //ips200 初始化 SPI 驱动
	Key_init();                                                         // 初始化按键 GPIO
// 	 while(1)
//    {
//        if(mt9v03x_init())
//        {
//            ips200_show_string(0, 16, "mt9v03x reinit.");
//        }
//        else
//        {
//            break;
//        }
//        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
//    }
//    ips200_show_string(0, 16, "init success.");
	system_delay_ms(1000);  
	
	
	encoder_quad_init(TIM3_ENCODER, TIM3_ENCODER_CH1_B4, TIM3_ENCODER_CH2_B5);
	//encoder_quad_init(TIM4_ENCODER, TIM4_ENCODER_CH1_B6, TIM4_ENCODER_CH2_B7);
	encoder_dir_init(TIM4_ENCODER, TIM4_ENCODER_CH1_B6, TIM4_ENCODER_CH2_B7);
	ips200_clear();
	
  //  motor_init();                                                        // 初始化电机驱动芯片
    //片上资源初始化
	pit_ms_init(TIM2_PIT, 1);                    //按键扫描                    // 初始化 PIT 定时器 TIM2_PIT 1ms 定时器
	pit_ms_init(TIM5_PIT, 100);
	interrupt_set_priority(TIM2_IRQn, 0);
	interrupt_set_priority(TIM5_IRQn, 1);
}
