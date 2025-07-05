#include "zf_common_headfile.h"
#include "all_init.h"
void ds_key_init(void){
	gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);                               // ��ʼ�� KEY1 ���� Ĭ�ϸߵ�ƽ ��������
	gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);                               // ��ʼ�� KEY2 ���� Ĭ�ϸߵ�ƽ ��������
	gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);                               // ��ʼ�� KEY3 ���� Ĭ�ϸߵ�ƽ ��������
	gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP); 
}
void all_init(void){
	
	//ϵͳ��ʼ��
	clock_init(SYSTEM_CLOCK_120M);                                              // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 120MHz
  	debug_init();   
	system_delay_ms(300);
	
	//�����ʼ��
	ds_ips200_init();
	ds_key_init();
	
	
	
	//Ƭ����Դ��ʼ��
	pit_ms_init(TIM2_PIT, 1); 
	pit_ms_init(TIM5_PIT, 100);
	interrupt_set_priority(TIM2_IRQn, 0);
	interrupt_set_priority(TIM5_IRQn, 1);
	
}