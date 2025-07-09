#include "zf_common_headfile.h"
#include "key.h"
//
int16 Threshold_multiple;
int16 Threshold;   
void Duct_start(){}	
//ï¿½ï¿½ï¿½ï¿½Ò»ï¿½ï¿½switch2×ªï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ðµï¿½KEY5,KEY6
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
uint8 Model=1;          //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ¶ï¿½ï¿½ï¿½Ö¾Î»  Model:1~20
uint8 last_model;
uint8 grade_flag=0;     //È·ï¿½Ï±ï¿½Ö¾Î»
uint8 ips200_show_flag=0; //ï¿½ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½ï¿½Ö¾
uint8 lose=0;           //ï¿½ï¿½
uint8 plus=0;           //ï¿½ï¿½
uint8 return_flag=0;    //ï¿½ï¿½ï¿½ï¿½

uint8 Angle_flag=0; //ï¿½ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½ï¿½Ö¾

uint8 key_flag=0;
uint32 data_buff[50]={0};

uint8 ips200_show_element_flag=0;
uint8 ips200_show_ring_flag=0;


void key(void)
{
		//ï¿½ï¿½ï¿½ï¿½×´Ì¬ï¿½ï¿½ï¿½ï¿½
	static uint8 key1_status = 1;
	static uint8 key2_status = 1;
	static uint8 key3_status = 1;
	static uint8 key4_status = 1;
	//ÉÏÒ»´Î¿ª¹Ø×´Ì¬±äÁ¿
	static uint8 key1_last_status;
	static uint8 key2_last_status;
	static uint8 key3_last_status;
	static uint8 key4_last_status;
	//Ê¹ÓÃ´Ë·½·¨ÓÅµãÔÚÓÚ£¬²»ÐèÒªÊ¹ÓÃwhile(1) µÈ´ý£¬±ÜÃâ´¦ÀíÆ÷×ÊÔ´ÀË·Ñ
	//±£´æ°´¼ü×´Ì¬
	key1_last_status = key1_status;
	key2_last_status = key2_status;
	key3_last_status = key3_status;
	key4_last_status = key4_status;
	//¶ÁÈ¡µ±Ç°°´¼ü×´Ì¬
	key1_status = gpio_get_level(KEY1);
	key2_status = gpio_get_level(KEY2);
	key3_status = gpio_get_level(KEY3);
	key4_status = gpio_get_level(KEY4);
	//¼ì²âµ½°´¼ü°´ÏÂÖ®ºó  ²¢·Å¿ªÖÃÎ»±êÖ¾Î»
	if(key1_status && !key1_last_status)    key1_flag = 1;//·µ»Ø
	if(key2_status && !key2_last_status)    key2_flag = 1;//ÉÏ
	if(key3_status && !key3_last_status){   
		if(!gpio_get_level(SWITCH2))
		key3_flag = 1;//ï¿½ï¿½
		else
		key5_flag = 1;	
	}
	if(key4_status && !key4_last_status){   
		if(!gpio_get_level(SWITCH2))
		key4_flag = 1;//ï¿½ï¿½
		else
		key6_flag = 1;	
	}
	//±êÖ¾Î»ÖÃÎ»Ö®ºó£¬¿ÉÒÔÊ¹ÓÃ±êÖ¾Î»Ö´ÐÐ×Ô¼ºÏëÒª×öµÄÊÂ¼þ¡£Íâ²¿ÏÔÊ¾±êÖ¾Î»»áÎª0£¬ÕâÀïÖÐ¼ä±äÁ¿Ö±½Ó¡±ÓÅ»¯¡°
	if(key1_flag==1)   
	{
		key1_flag = 0;//Ê¹ï¿½Ã°ï¿½ï¿½ï¿½Ö®ï¿½ï¿½Ó¦ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î»
		return_flag=1;//ï¿½ï¿½ï¿½ï¿½	
	}
	if(key2_flag==1)   
	{
		key2_flag = 0;//Ê¹ÓÃ°´¼üÖ®ºó£¬Ó¦¸ÃÇå³ý±êÖ¾Î»
		//grade_flag++;	//È·¶¨
		grade_flag=grade_flag%3+1;
	}
	if(key3_flag==1)   
	{
		key3_flag = 0;//Ê¹ï¿½Ã°ï¿½ï¿½ï¿½Ö®ï¿½ï¿½Ó¦ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î»

		Model--;  //¹â±ê--£¨ÉÏÒÆ£©
		Model = (Model < 1) ? menu_item : (Model > menu_item) ? 1 : Model;
	}
	if(key4_flag==1)   
	{
		key4_flag = 0;//Ê¹ÓÃ°´¼üÖ®ºó£¬Ó¦¸ÃÇå³ý±êÖ¾Î»
		Model++;  //¹â±ê++(ÏÂÒÆ)
		Model = (Model < 1) ? menu_item : (Model > menu_item) ? 1 : Model;
	}
	if(key5_flag==1)   
	{
		key5_flag = 0;//Ê¹ï¿½Ã°ï¿½ï¿½ï¿½Ö®ï¿½ï¿½Ó¦ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î»
		plus++;	  //ï¿½ï¿½
	}
	if(key6_flag==1)   
	{
		key6_flag = 0;//Ê¹ï¿½Ã°ï¿½ï¿½ï¿½Ö®ï¿½ï¿½Ó¦ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î»
		lose++;  //ï¿½ï¿½
	}
}
void ParameterExchange(void){
	//ï¿½ï¿½ï¿½
//	if (offsety > 0) {
//    ips200_show_string(0, (last_model - 1) * offsety, " ");  // ï¿½ï¿½ï¿½ï¿½É¹ï¿½ï¿½
//    ips200_show_string(0, (Model - 1) * offsety, ">");       // ï¿½ï¿½Ê¾ï¿½Â¹ï¿½ï¿½
//    last_model = Model;  // ï¿½ï¿½ï¿½Â¼ï¿½Â¼
//	}
	ips200_show_string(0,((Model-1+menu_item)%menu_item)*offsety,">");//1
	ips200_show_string(0,((Model-2+menu_item)%menu_item)*offsety," ");//
	ips200_show_string(0,((Model+menu_item)%menu_item)*offsety," ");//
	
//ï¿½ï¿½ï¿½ä£ºmodelï¿½ï¿½Î§ï¿½Þ¶ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ò½«·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½ï¿½Assert error
	//Model
	ips200_show_string(0,(Model=Model == 20 ? 1 : Model)*offsety," ");
	if(grade_flag==1)                  
	{
		switch(Model)
		{
			case 1: ips200_show_flag=100;grade_flag=2; Model=1; cls_flag=0;Duct_start();    break; //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î» ;
			case 2: ips200_show_flag=1; grade_flag=2; Model=1; cls_flag=0;   break;								//PID
			case 3: ips200_show_flag=2; grade_flag=2; Model=1; cls_flag=0;   break;								//ï¿½ï¿½Öµï¿½ï¿½ï¿½ï¿½
			case 4: ips200_show_flag=3; grade_flag=2; Model=1; cls_flag=0;   break;								//Ôªï¿½ï¿½Ñ¡ï¿½ï¿½
			default:Model=1;break;
		}
	}
/*------------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½---------------------------------------*/
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
/*------------------------pidï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ß¼ï¿½Êµï¿½ï¿½)---------------------------------------*/
	if(ips200_show_flag==1){
		if(plus){
			switch (Model) 
			{
			// case 1: int16 Pid_Kp+=10; break;//Ô¤ï¿½ï¿½×ªï¿½ï¿½ï¿½pid
			// case 2: int16 Pid_Kd+=100; break;	
				case 1:Motor_Pid_speed_Z+=5;
//					if(Motor_Pid_speed_Z==180){
//						Motor_Pid_Dif_P=4;
//					}else if(Motor_Pid_speed_Z==200){
//						Motor_Pid_Dif_P=5;
//					}
					break;
				case 2: Motor_Pid_Dif_P       -=1; break;
				case 3:Linear_speed+=5;break;
				case 4:Curve_speed+=5;break;

				case 5:Motor_Pid_Z_L_Ki+=10;break;
				case 6:Motor_Pid_Z_L_Kp+=10;break;
				case 7:Motor_Pid_Z_R_Ki+=10;break;
				case 8:Motor_Pid_Z_R_Kp+=10;break;
			default:
				break;
			}
			/*---------------flash±£´æ--------------------*/
			EepromWrite();
			Motor_PID_subsection();
			plus=0;
		}
		if(lose){
			switch (Model) 
			{
			case 1:Motor_Pid_speed_Z-=5;
//					if(Motor_Pid_speed_Z==180){
//						Motor_Pid_Dif_P=4;
//					}else if(Motor_Pid_speed_Z==200){
//						Motor_Pid_Dif_P=5;
//					}
					break;
			case 2: Motor_Pid_Dif_P       -=1; break;
			case 3:Linear_speed-=5;break;
			case 4:Curve_speed-=5;break;

			case 5:Motor_Pid_Z_L_Ki-=10;break;
			case 6:Motor_Pid_Z_L_Kp-=10;break;

			case 7:Motor_Pid_Z_R_Ki-=10;break;
			case 8:Motor_Pid_Z_R_Kp-=10;break;
			
			default:
				break;
			}
			/*---------------flash±£´æ--------------------*/
			EepromWrite();
			Motor_PID_subsection();
			lose=0;
		}
		if(return_flag==1){
			return_flag=0;
			Model=1;	//ï¿½ï¿½ï¿½
			cls_flag=0;//ï¿½ï¿½ï¿½ï¿½
			ips200_show_flag=0;	// ï¿½ï¿½ï¿½Øµï¿½ï¿½ï¿½ï¿½Ëµï¿½
			grade_flag=0;		// ï¿½ï¿½ï¿½Ã²Ëµï¿½ï¿½ã¼¶
		}
	}
/*------------------------ï¿½ï¿½Öµï¿½ï¿½ï¿½ï¿½---------------------------------------*/
	if(ips200_show_flag==2){
		if(plus){
			switch (Model) 
			{
			case 1:Threshold_multiple+=5;break;
			case 2:Threshold+=5;   	break;
			default:
				break;
			}
			/*---------------flash±£´æ--------------------*/
			EepromWrite();
			plus=0;
		}
		if(lose){
			switch (Model) 
			{
			case 1:Threshold_multiple=5;break;
			case 2:Threshold-=5;   	break;
			default:
				break;
			}
			/*---------------flash±£´æ--------------------*/
			EepromWrite();
			lose=0;
		}
		if(return_flag==1){
			return_flag=0;
			Model=1;	//ï¿½ï¿½ï¿½
			cls_flag=0;//ï¿½ï¿½ï¿½ï¿½
			ips200_show_flag=0;	// ï¿½ï¿½ï¿½Øµï¿½ï¿½ï¿½ï¿½Ëµï¿½
			grade_flag=0;		// ï¿½ï¿½ï¿½Ã²Ëµï¿½ï¿½ã¼¶
		}
	}
	
}

/*---------------flashï¿½ï¿½ï¿½ï¿½--------------------*/
void EepromWrite(void){
	flash_erase_page (0,0);
	data_buff[0]=Motor_Pid_speed_Z;
	data_buff[1]=Motor_Pid_Dif_P;
	data_buff[2]=Motor_Pid_Z_L_Ki;
	data_buff[3]=Motor_Pid_Z_L_Kp;
	data_buff[4]=Motor_Pid_Z_R_Ki;
	data_buff[5]=Motor_Pid_Z_R_Kp;

	data_buff[6]=Motor_Pid_Dif_P;

	data_buff[7] = Threshold_multiple;
	data_buff[8] = Threshold;
	flash_write_page(0,0,data_buff,50);
	//ÔªËØui
}

void EepromRead(void){
	flash_read_page(0, 0, data_buff, 50);
	Motor_Pid_speed_Z=data_buff[0];
	Motor_Pid_Dif_P=data_buff[1];
	Motor_Pid_Z_L_Ki=data_buff[2];
	Motor_Pid_Z_L_Kp=data_buff[3];
	Motor_Pid_Z_R_Ki=data_buff[4];
	Motor_Pid_Z_R_Kp=data_buff[5];

	Motor_Pid_Dif_P=data_buff[6];

	Threshold_multiple=data_buff[7];
	Threshold=data_buff[8];
}