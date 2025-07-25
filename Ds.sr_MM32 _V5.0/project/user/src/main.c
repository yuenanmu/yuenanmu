/*********************************************************************************************************************
* MM32F327X-G8P Opensourec Library 即（MM32F327X-G8P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MM32F327X-G8P 开源库的一部分
* 
* MM32F327X-G8P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.37
* 适用平台          MM32F327X_G8P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#define DEG_TO_RAD(deg)  ((deg)*3.1415/180.0)

// **************************** 代码区域 ****************************
int main(void)
{
	all_init();
	debug_assert_disable();
	BB();
    while(1)
    {
//			double angle_deg = TIM2_count_time;       // 角度
//			double angle_rad = DEG_TO_RAD(angle_deg); // 转换为弧度
//			result = sin(angle_rad); 
			if(key_flag==1)//按键ui屏幕显示
		{
	  	ParameterExchange();
			ds_main_menu();
		}
		if(close_menu==1){
			ips200_clear();
			close_menu=0;
		}
			if(mt9v03x_finish_flag)
		{
			Img_Processing();//图象处理
			//mt9v03x_send_data(UART_1, mt9v03x_data_ch1);
			//if(ips200_show_flag==2||ips200_show_flag==1||key_flag==0){}  else ips200_show_Img();
			mt9v03x_finish_flag = 0;	
		}
    }
}
// **************************** 代码区域 ****************************
//变量显示在频幕上，标志位清除太快就像没有一样。有两块黑色区域就说明进入了某个函数。下次查看变量的时候应该写一个if函数，而不是直接打印变量
//ips200_show_binary_image显示二值化图像就是狗屎。。。
//原来数组越界真的会导致函数卡死，，，和上次的闪存一样莫名其妙的
//debug调试断点，print查看运行时的变量
//屏幕：割裂：拔掉所有电源。只有线：摄像头线松了
//差分速度有点大。差分速度的pid（加菜单）为什么左电机不动了。

//电机测试代码
//		if(start_go==1){
//			Motor_Control_L(30);
//			Motor_Control_R(50);
////		 pwm_set_duty(PWM_1, 1500);
////		 pwm_set_duty(PWM_2, 1500);
//		}
//		else{
////		 pwm_set_duty(PWM_1, 0);//重置目标速度
////		 pwm_set_duty(PWM_2, 0);		
//			Motor_Control_L(0);
//			Motor_Control_R(0);
//		}
			//gpio_set_level(DIR_2,GPIO_HIGH);
			//gpio_set_level(DIR_2,GPIO_LOW);
		
//		 pwm_set_duty(PWM_1, 1500);
//		 pwm_set_duty(PWM_2, 1500);

//电机测试代码
//		if(start_go==1){
//			// Motor_Control_L(30);
//			// Motor_Control_R(50);
//		 pwm_set_duty(PWM_1,2000);
//		 pwm_set_duty(PWM_2,2000);
//		}
//		else{
////		 pwm_set_duty(PWM_1, 0);//重置目标速度
////		 pwm_set_duty(PWM_2, 0);		
//			Motor_Control_L(0);
//			Motor_Control_R(0);
//		}
//		if(gpio_get_level(SWITCH1)==0){
//			encoder_integra1=0;
//			encoder_integra2=0;
//		}