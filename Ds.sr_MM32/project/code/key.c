#include "zf_common_headfile.h"
#include "key.h"
//
uint8 cls_flag=0;//菜单栏
Duct_start(){}	
uint8 start=0;//电机

//多了一个switch2转换，替代理想中的KEY5,KEY6
#define menu_item 20
uint32 key1_count;
uint32 key2_count;
uint32 key3_count;
uint32 key4_count;
uint32 key5_count;
uint32 key6_count;
uint8  key1_flag;
uint8  key2_flag;
uint8  key3_flag;
uint8  key4_flag;
uint8  switch_flag;
uint8  key5_flag;
uint8  key6_flag;
uint32 count_time=1000;

uint8 offsetx=8;
uint8 offsety=16;
uint8 Model=1;          //光标上下移动标志位
uint8 grade_flag=0;     //确认标志位
uint8 ips200_show_flag=0; //二级菜单标志
uint8 lose=0;           //减
uint8 plus=0;           //加
uint8 return_flag=0;    //返回

uint8 Angle_flag=0; //二级菜单标志

uint8 key_flag=0;
uint8 data_buff[85]={0};

uint8 ips200_show_element_flag=0;
uint8 ips200_show_ring_flag=0;


void key(void)
{
		//开关状态变量
	static uint8 key1_status = 1;
	static uint8 key2_status = 1;
	static uint8 key3_status = 1;
	static uint8 key4_status = 1;
//	static uint8 key5_status = 1;
//	static uint8 key6_status = 1;
	//上一次开关状态变量
	static uint8 key1_last_status;
	static uint8 key2_last_status;
	static uint8 key3_last_status;
	static uint8 key4_last_status;
//	static uint8 key5_last_status;
//	static uint8 key6_last_status;
	//使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
	//保存按键状态
	key1_last_status = key1_status;
	key2_last_status = key2_status;
	key3_last_status = key3_status;
	key4_last_status = key4_status;
//	key5_last_status = key5_status;
//	key6_last_status = key6_status;
	//读取当前按键状态
	key1_status = gpio_get_level(KEY1);
	key2_status = gpio_get_level(KEY2);
	key3_status = gpio_get_level(KEY3);
	key4_status = gpio_get_level(KEY4);
//	key5_status = KEY5;
//	key6_status = KEY6;
	//检测到按键按下之后  并放开置位标志位
	if(key1_status && !key1_last_status)    key1_flag = 1;//返回
	if(key2_status && !key2_last_status)    key2_flag = 1;//上
	if(key3_status && !key3_last_status){   
		if(!gpio_get_level(SWITCH2))
		key3_flag = 1;//左
		else
		key5_flag = 1;	
	}
	if(key4_status && !key4_last_status){   
		if(!gpio_get_level(SWITCH2))
		key4_flag = 1;//左
		else
		key6_flag = 1;	
	}
//	if(key5_status && !key5_last_status)    key5_flag = 1;//右
//	if(key6_status && !key6_last_status)    key6_flag = 1;//确认
	//标志位置位之后，可以使用标志位执行自己想要做的事件。外部显示标志位会为0，这里中间变量直接”优化“
	if(key1_flag==1)   
	{
		key1_flag = 0;//使用按键之后，应该清除标志位
		return_flag=1;//返回	
	}
	if(key2_flag==1)   
	{
		key2_flag = 0;//使用按键之后，应该清除标志位
		grade_flag++;	//确定
	}
	if(key3_flag==1)   
	{
		key3_flag = 0;//使用按键之后，应该清除标志位

		Model++;  //光标++
	}
	if(key4_flag==1)   
	{
		key4_flag = 0;//使用按键之后，应该清除标志位
		Model--;  //光标--
		
	}
	if(key5_flag==1)   
	{
		key5_flag = 0;//使用按键之后，应该清除标志位
		plus++;	  //加
	}
	if(key6_flag==1)   
	{
		key6_flag = 0;//使用按键之后，应该清除标志位
		lose++;  //减
	}
}
void ParameterExchange(void){
	//光标
	ips200_show_string(0,((Model-1+menu_item)%menu_item)*offsety,">");
	ips200_show_string(0,((Model-2+menu_item)%menu_item)*offsety," ");
	ips200_show_string(0,((Model+menu_item)%menu_item)*offsety," ");
	//补充：model范围限定，否则将发生断言错误：Assert error
	//Model
	//ips200_show_string(0,Model*offsety == 16*offsety ? 1*offsety : Model*offsety," ");
	if(grade_flag==1)                  
	{
		switch(Model)
		{
			case 1: ips200_show_flag=100;grade_flag=2; Model=0; cls_flag=0;Duct_start();    break; //发车标志位 ;
			case 2: ips200_show_flag=1; grade_flag=2; Model=0; cls_flag=0;   break;								//PID
			case 3: ips200_show_flag=2; grade_flag=2; Model=0; cls_flag=0;   break;								//阈值调整
			case 4: ips200_show_flag=3; grade_flag=2; Model=0; cls_flag=0;   break;								//元素选择
			default:Model=1;break;
		}
	}
/*------------------------发车二级---------------------------------------*/
	if(ips200_show_flag==100){  
		if(grade_flag==3){
			start=1;
			Angle_flag=1;
			key_flag=key_flag;
		}
		if(return_flag==1){
			return_flag=0;
			Model=1;
			cls_flag=0;
			ips200_show_flag=0;
			grade_flag=0;
		}
	}
/*------------------------pid二级(包含三级调参逻辑实现)---------------------------------------*/
	if(ips200_show_flag==1){
		if(plus==1){
			switch (Model) 
			{
			case 1:
				
				break;
			
			default:
				break;
			}
			/*---------------flash保存--------------------*/
			plus=0;
		}
		if(lose==1){
			switch (Model) 
			{
			case 1:
				
				break;
			
			default:
				break;
			}
			/*---------------flash保存--------------------*/
			lose=0;
		}
		if(return_flag==1){
			return_flag=0;
			Model=1;	//光标
			cls_flag=0;//清屏
			ips200_show_flag=0;	// 返回到主菜单
			grade_flag=0;		// 重置菜单层级
		}
	}
/*------------------------阈值二级---------------------------------------*/
	if(ips200_show_flag==2){
		if(plus==1){
			switch (Model) 
			{
			case 1:
				
				break;
			
			default:
				break;
			}
			/*---------------flash保存--------------------*/
			plus=0;
		}
		if(lose==1){
			switch (Model) 
			{
			case 1:
				
				break;
			
			default:
				break;
			}
			/*---------------flash保存--------------------*/
			lose=0;
		}
		if(return_flag==1){
			return_flag=0;
			Model=1;	//光标
			cls_flag=0;//清屏
			ips200_show_flag=0;	// 返回到主菜单
			grade_flag=0;		// 重置菜单层级
		}
	}
	
}