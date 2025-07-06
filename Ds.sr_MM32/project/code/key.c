#include "zf_common_headfile.h"
#include "key.h"
//
int16 Threshold_multiple;
int16 Threshold;   
void Duct_start(){}	
//����һ��switch2ת������������е�KEY5,KEY6
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
uint8 Model=1;          //��������ƶ���־λ  Model:1~20
uint8 last_model;
uint8 grade_flag=0;     //ȷ�ϱ�־λ
uint8 ips200_show_flag=0; //�����˵���־
uint8 lose=0;           //��
uint8 plus=0;           //��
uint8 return_flag=0;    //����

uint8 Angle_flag=0; //�����˵���־

uint8 key_flag=0;
uint32 data_buff[50]={0};

uint8 ips200_show_element_flag=0;
uint8 ips200_show_ring_flag=0;


void key(void)
{
		//����״̬����
	static uint8 key1_status = 1;
	static uint8 key2_status = 1;
	static uint8 key3_status = 1;
	static uint8 key4_status = 1;
//	static uint8 key5_status = 1;
//	static uint8 key6_status = 1;
	//��һ�ο���״̬����
	static uint8 key1_last_status;
	static uint8 key2_last_status;
	static uint8 key3_last_status;
	static uint8 key4_last_status;
//	static uint8 key5_last_status;
//	static uint8 key6_last_status;
	//ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
	//���水��״̬
	key1_last_status = key1_status;
	key2_last_status = key2_status;
	key3_last_status = key3_status;
	key4_last_status = key4_status;
//	key5_last_status = key5_status;
//	key6_last_status = key6_status;
	//��ȡ��ǰ����״̬
	key1_status = gpio_get_level(KEY1);
	key2_status = gpio_get_level(KEY2);
	key3_status = gpio_get_level(KEY3);
	key4_status = gpio_get_level(KEY4);
//	key5_status = KEY5;
//	key6_status = KEY6;
	//��⵽��������֮��  ���ſ���λ��־λ
	if(key1_status && !key1_last_status)    key1_flag = 1;//����
	if(key2_status && !key2_last_status)    key2_flag = 1;//��
	if(key3_status && !key3_last_status){   
		if(!gpio_get_level(SWITCH2))
		key3_flag = 1;//��
		else
		key5_flag = 1;	
	}
	if(key4_status && !key4_last_status){   
		if(!gpio_get_level(SWITCH2))
		key4_flag = 1;//��
		else
		key6_flag = 1;	
	}
//	if(key5_status && !key5_last_status)    key5_flag = 1;//��
//	if(key6_status && !key6_last_status)    key6_flag = 1;//ȷ��
	//��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼����ⲿ��ʾ��־λ��Ϊ0�������м����ֱ�ӡ��Ż���
	if(key1_flag==1)   
	{
		key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
		return_flag=1;//����	
	}
	if(key2_flag==1)   
	{
		key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
		grade_flag++;	//ȷ��
	}
	if(key3_flag==1)   
	{
		key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ

		Model--;  //���--�����ƣ�
		//Model=(Model+menu_item)%menu_item+1;
		//Model == 16 ? 1 : Model;
		//Model = (Model == 1) ? menu_item : Model - 1;  // ���δ���
	}
	if(key4_flag==1)   
	{
		key4_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
		Model++;  //���++(����)
		//Model=Model%menu_item+1;
		//Model = (Model == menu_item) ? 1 : Model + 1;  // ���δ���
	}
	if(key5_flag==1)   
	{
		key5_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
		plus++;	  //��
	}
	if(key6_flag==1)   
	{
		key6_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
		lose++;  //��
	}
}
void ParameterExchange(void){
	//���
//	if (offsety > 0) {
//    ips200_show_string(0, (last_model - 1) * offsety, " ");  // ����ɹ��
//    ips200_show_string(0, (Model - 1) * offsety, ">");       // ��ʾ�¹��
//    last_model = Model;  // ���¼�¼
//	}
	ips200_show_string(0,((Model-1+menu_item)%menu_item)*offsety,">");//1
	ips200_show_string(0,((Model-2+menu_item)%menu_item)*offsety," ");//
	ips200_show_string(0,((Model+menu_item)%menu_item)*offsety," ");//
	
//���䣺model��Χ�޶������򽫷������Դ���Assert error
	//Model
	ips200_show_string(0,(Model=Model == 20 ? 1 : Model)*offsety," ");
	if(grade_flag==1)                  
	{
		switch(Model)
		{
			case 1: ips200_show_flag=100;grade_flag=2; Model=1; cls_flag=0;Duct_start();    break; //������־λ ;
			case 2: ips200_show_flag=1; grade_flag=2; Model=1; cls_flag=0;   break;								//PID
			case 3: ips200_show_flag=2; grade_flag=2; Model=1; cls_flag=0;   break;								//��ֵ����
			case 4: ips200_show_flag=3; grade_flag=2; Model=1; cls_flag=0;   break;								//Ԫ��ѡ��
			default:Model=1;break;
		}
	}
/*------------------------��������---------------------------------------*/
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
/*------------------------pid����(�������������߼�ʵ��)---------------------------------------*/
	if(ips200_show_flag==1){
		if(plus==1){
			switch (Model) 
			{
			// case 1: int16 Pid_Kp+=10; break;//Ԥ��ת���pid
			// case 2: int16 Pid_Kd+=100; break;	
				case 1:Motor_Pid_speed_Z+=5;
					if(Motor_Pid_speed_Z==180){
						Motor_Pid_Dif_P=4;
					}else if(Motor_Pid_speed_Z==200){
						Motor_Pid_Dif_P=5;
					}
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
			/*---------------flash����--------------------*/
			plus=0;
		}
		if(lose==1){
			switch (Model) 
			{
			case 1:Motor_Pid_speed_Z-=5;
					if(Motor_Pid_speed_Z==180){
						Motor_Pid_Dif_P=4;
					}else if(Motor_Pid_speed_Z==200){
						Motor_Pid_Dif_P=5;
					}
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
			/*---------------flash����--------------------*/
			lose=0;
		}
		if(return_flag==1){
			return_flag=0;
			Model=1;	//���
			cls_flag=0;//����
			ips200_show_flag=0;	// ���ص����˵�
			grade_flag=0;		// ���ò˵��㼶
		}
	}
/*------------------------��ֵ����---------------------------------------*/
	if(ips200_show_flag==2){
		if(plus==1){
			switch (Model) 
			{
			case 1:Threshold_multiple+=5;break;
			case 2:Threshold+=5;   	break;
			default:
				break;
			}
			/*---------------flash����--------------------*/
			plus=0;
		}
		if(lose==1){
			switch (Model) 
			{
			case 1:Threshold_multiple=5;break;
			case 2:Threshold-=5;   	break;
			default:
				break;
			}
			/*---------------flash����--------------------*/
			lose=0;
		}
		if(return_flag==1){
			return_flag=0;
			Model=1;	//���
			cls_flag=0;//����
			ips200_show_flag=0;	// ���ص����˵�
			grade_flag=0;		// ���ò˵��㼶
		}
	}
	
}

/*---------------flash����--------------------*/
void EepromWrite(void){
	flash_erase_page(0, 0);
	data_buff[0]=Motor_Pid_speed_Z;
	data_buff[1]=Motor_Pid_Dif_P;
	data_buff[2]=Motor_Pid_Z_L_Ki;
	data_buff[3]=Motor_Pid_Z_L_Kp;
	data_buff[4]=Motor_Pid_Z_R_Ki;
	data_buff[5]=Motor_Pid_Z_R_Kp;

	data_buff[6]=Motor_Pid_Dif_P;

	data_buff[7] = Threshold_multiple;
	data_buff[8] = Threshold;

	//Ԫ��ui
	data_buff[9] = Linear_speed;
	data_buff[10] = Curve_speed;
	flash_write_page(0, 0,data_buff, 50);
}

void EepromRead(void){
	 flash_read_page (0, 0,data_buff, 30);
	Motor_Pid_speed_Z=data_buff[0];
	Motor_Pid_Dif_P=data_buff[1];
	Motor_Pid_Z_L_Ki=data_buff[2];
	Motor_Pid_Z_L_Kp=data_buff[3];
	Motor_Pid_Z_R_Ki=data_buff[4];
	Motor_Pid_Z_R_Kp=data_buff[5];

	Motor_Pid_Dif_P=data_buff[6];

	Threshold_multiple=data_buff[7] ;
	Threshold=data_buff[8] ;

	//Ԫ��ui
	Linear_speed=data_buff[9];
	Curve_speed=data_buff[10];
	flash_write_page(0, 0,data_buff, 50);
}