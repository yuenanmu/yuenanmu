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

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
//void img_handle(){}//图象处理
//void ips200_show(){}
// 本例程是开源库移植用空工程

// **************************** 代码区域 ****************************
int main(void)
{
                                                                // 初始化默认 Debug UART
	  all_init();
	  //gpio_set_level(DIR_2, GPIO_LOW);
		//gpio_set_level(DIR_2,GPIO_HIGH);
		//pwm_set_duty(PWM_1, 1500);
	  debug_assert_disable();
    while(1)
    {
//			if(key_flag==0)//按键ui屏幕显示
//		{
//			key();
//			ParameterExchange();
//			ds_main_menu();
//		}
		//Motor_Control_L(5);
		//Motor_Control_R(-1000);
		//Motor_Control_L(Motor_Pid_speed_Z);
		//Motor_Control_R(int16 OUT_R_SPEED);
			ips200_show_int((2-1)*offsetx,(1-1)*offsety,mt9v03x_finish_flag,3);
			system_delay_ms(500);
			if(mt9v03x_finish_flag)
		{
			//Img_handle();//图象处理
			//mt9v03x_send_data(UART_1, mt9v03x_data_ch1);
		  ips200_show_Img();
			mt9v03x_finish_flag = 0;	
		}
//			key();
//		  system_delay_ms(100);
        // 此处编写需要循环执行的代码
//      ips200_show_int(0,0,gpio_get_level(KEY1),2);
//			ips200_show_int(0,16,key2_flag,2);
//			ips200_show_int(0,32,key3_flag,2);
//			ips200_show_int(0,48,key4_flag,2);
//			ips200_show_int(0,64,key5_flag,2);
//			ips200_show_int(0,80,key6_flag,2);
        // 此处编写需要循环执行的代码
    }
}
// **************************** 代码区域 ****************************
