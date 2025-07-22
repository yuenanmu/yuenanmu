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
//    encoder_L = encoder_L_last; // ����ֱ��������ǰ֡
//}
//	if(abs(encoder_R - encoder_R_last) > MAX_EXPECTED_ENCODER) {
//    encoder_R = encoder_R_last; // ����ֱ��������ǰ֡
//}
	encoder=(encoder_L+encoder_R)*0.5;
	encoder_integra1+=encoder_L*0.02;
	encoder_integra2+=encoder_R*0.02;
	
	encoder_integra+=encoder*0.02;

//	encoder_L_last=encoder_L;
//	encoder_R_last=encoder_R;
}
void ds_key_init(void){
	gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);                               // ��ʼ�� KEY1 ���� Ĭ�ϸߵ�ƽ ��������
	gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);                               // ��ʼ�� KEY2 ���� Ĭ�ϸߵ�ƽ ��������
	gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);                               // ��ʼ�� KEY3 ���� Ĭ�ϸߵ�ƽ ��������
	gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP); 
	
	gpio_init(BEEP,GPO,GPIO_LOW,GPO_PUSH_PULL);																	//������
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
	
	//ϵͳ��ʼ��
	clock_init(SYSTEM_CLOCK_120M);                                              // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 120MHz
  debug_init();   
	system_delay_ms(100);
	
	//�����ʼ��
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
        system_delay_ms(500);                                                   // ����ʱ�������Ʊ�ʾ�쳣
  }
	wireless_uart_init();
  ds_ips200_init();
	ds_key_init();
	ds_encoder_init();
	ds_motor_init();
	Motor_Pid_init();
	//��ʼ��֮���ȡflash������
	EepromRead();
	ips200_show_string(0, 16, "init success.");
	system_delay_ms(100); 
	ips200_clear();
	//Ƭ����Դ��ʼ��
	pit_ms_init(TIM2_PIT, 1); 
	pit_ms_init(TIM7_PIT, 100);
	interrupt_set_priority(TIM2_IRQn, 0);
	interrupt_set_priority(TIM7_IRQn, 1);
	
}