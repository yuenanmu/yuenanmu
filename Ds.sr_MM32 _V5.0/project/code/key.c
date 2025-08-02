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

uint8 key_flag=1;
uint8 close_menu=0;
uint32 data_buff[85]={0};

uint8 ips200_show_element_flag=0;
uint8 ips200_show_ring_flag=0;


void key(void)
{
		//����״̬����
	static uint8 key1_status = 1;
	static uint8 key2_status = 1;
	static uint8 key3_status = 1;
	static uint8 key4_status = 1;
	//��һ�ο���״̬����
	static uint8 key1_last_status;
	static uint8 key2_last_status;
	static uint8 key3_last_status;
	static uint8 key4_last_status;
	//ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
	//���水��״̬
	key1_last_status = key1_status;
	key2_last_status = key2_status;
	key3_last_status = key3_status;
	key4_last_status = key4_status;
	//��ȡ��ǰ����״̬
	key1_status = gpio_get_level(KEY1);
	key2_status = gpio_get_level(KEY2);
	key3_status = gpio_get_level(KEY3);
	key4_status = gpio_get_level(KEY4);
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
	//��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼����ⲿ��ʾ��־λ��Ϊ0�������м����ֱ�ӡ��Ż���
	if(key1_flag==1)   
	{
		key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
		return_flag=1;//����	
	}
	if(key2_flag==1)   
	{
		key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
		//grade_flag++;	//ȷ��
		grade_flag=grade_flag%3+1;
	}
	if(key3_flag==1)   
	{
		key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ

		Model--;  //���--�����ƣ�
		Model = (Model < 1) ? menu_item : (Model > menu_item) ? 1 : Model;
	}
	if(key4_flag==1)   
	{
		key4_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
		Model++;  //���++(����)
		Model = (Model < 1) ? menu_item : (Model > menu_item) ? 1 : Model;
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
			start_go+=1;
			start_go=start_go%2;
			Angle_flag=1;
			key_flag=key_flag;
			key_flag=0;  //�����رղ˵�
			close_menu=1;
			grade_flag=2;			//�������õ���һ����ִ��һ�ξͺã����ֵ�ڷ����˵��в�����ʾ3
			Motor_Pid.Dif_Speed=0;
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
		if(plus){
			switch (Model) 
			{
				case 1:Motor_Pid_speed_Z+=50;break;
				case 2:Motor_Pid.Dif_P +=0.1; break;//+=0.1  Motor_Pid_Dif_P�������������Ч��
				
				case 3:Linear_speed+=5;break;
				case 4:Curve_speed+=5;break;

				case 5:Motor_Pid.Kp+=0.1;break;
				case 6:Motor_Pid.Ki+=1;break;
				case 7:Motor_Pid.Kd+=0.1;break;

				case 8:Motor_Pid.Dir_Kp+=10;break;
				case 9:Motor_Pid.Dir_Kd+=100;break;

				case 10:Turn_PPDD_Loc.kp+=0.1;break;//ת����Ӧϵ��
				case 11:Turn_PPDD_Loc.kd+=0.1;break;//���ƶ���+Ԥ��
				case 12:Turn_PPDD_Loc.kp2+=1;break;//������ϵ��,����ת������,��
				case 13:Turn_PPDD_Loc.kd2+=1;break;//������ת��ϵ��,���ƶ���

			default:
				break;
			}
			/*---------------flash����--------------------*/
			EepromWrite();
			Motor_PID_subsection();
			plus=0;
		}
		if(lose){
			switch (Model) 
			{
						case 1:Motor_Pid_speed_Z-=50;break;
            case 2:Motor_Pid.Dif_P -=0.1; break;
            
            case 3:Linear_speed-=5;break;
            case 4:Curve_speed-=5;break;

            case 5:Motor_Pid.Kp-=0.1;break;
            case 6:Motor_Pid.Ki-=1;break;
            case 7:Motor_Pid.Kd-=0.1;break;

            case 8:Motor_Pid.Dir_Kp-=10;break;
            case 9:Motor_Pid.Dir_Kd-=100;break;

            case 10:Turn_PPDD_Loc.kp-=0.1;break;//ת����Ӧϵ��
            case 11:Turn_PPDD_Loc.kd-=0.1;break;//���ƶ���+Ԥ��
            case 12:Turn_PPDD_Loc.kp2-=1;break;//������ϵ��,����ת������,��
            case 13:Turn_PPDD_Loc.kd2-=1;break;//������ת��ϵ��,���ƶ���
			default:
				break;
			}
			/*---------------flash����--------------------*/
			EepromWrite();
			Motor_PID_subsection();
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
		if(plus){
			switch (Model) 
			{
			case 1:foresight_line+=5;break;
			case 2:Threshold+=5;   	break;
			default:
				break;
			}
			/*---------------flash����--------------------*/
			EepromWrite();
			plus=0;
		}
		if(lose){
			switch (Model) 
			{
			case 1:foresight_line-=5;break;
			case 2:Threshold-=5;   	break;
			default:
				break;
			}
			/*---------------flash����--------------------*/
			EepromWrite();
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

void EepromWrite(void){
	if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
    {
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // ������һҳ
    }
	flash_buffer_clear();  
	flash_union_buffer[0].int16_type  = Motor_Pid_speed_Z;
	flash_union_buffer[1].float_type  = Motor_Pid.Dif_P;           // case 2
    flash_union_buffer[2].int16_type  = Linear_speed;              // case 3
    flash_union_buffer[3].int16_type  = Curve_speed;               // case 4

    flash_union_buffer[4].float_type  = Motor_Pid.Kp;              // case 5
    flash_union_buffer[5].float_type  = Motor_Pid.Ki;              // case 6
    flash_union_buffer[6].float_type  = Motor_Pid.Kd;              // case 7

    flash_union_buffer[7].float_type  = Motor_Pid.Dir_Kp;          // case 8
    flash_union_buffer[8].float_type  = Motor_Pid.Dir_Kd;          // case 9

    flash_union_buffer[9].float_type  = Turn_PPDD_Loc.kp;          // case 10
    flash_union_buffer[10].float_type = Turn_PPDD_Loc.kd;          // case 11
    flash_union_buffer[11].float_type = Turn_PPDD_Loc.kp2;         // case 12
    flash_union_buffer[12].float_type = Turn_PPDD_Loc.kd2;         // case 13
	//Ԫ��ui
	printf("OK12");
  //���
	flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
}

void EepromRead(void){
	flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
	Motor_Pid_speed_Z  =flash_union_buffer[0].int16_type;
	Motor_Pid.Dif_P    = flash_union_buffer[1].float_type;    // case 2
    Linear_speed       = flash_union_buffer[2].int16_type;    // case 3
    Curve_speed        = flash_union_buffer[3].int16_type;    // case 4

    Motor_Pid.Kp       = flash_union_buffer[4].float_type;    // case 5
    Motor_Pid.Ki       = flash_union_buffer[5].float_type;    // case 6
    Motor_Pid.Kd       = flash_union_buffer[6].float_type;    // case 7

    Motor_Pid.Dir_Kp   = flash_union_buffer[7].float_type;    // case 8
    Motor_Pid.Dir_Kd   = flash_union_buffer[8].float_type;    // case 9

    Turn_PPDD_Loc.kp   = flash_union_buffer[9].float_type;    // case 10
    Turn_PPDD_Loc.kd   = flash_union_buffer[10].float_type;   // case 11
    Turn_PPDD_Loc.kp2  = flash_union_buffer[11].float_type;   // case 12
    Turn_PPDD_Loc.kd2  = flash_union_buffer[12].float_type; 
	

	//���
	Motor_PID_subsection();
	printf("OK123\n");
}
