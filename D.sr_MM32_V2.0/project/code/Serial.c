#include "zf_common_headfile.h"
#include "Serial.h"
void ds_serial(void){
	//printf("good\n");
	//printf("Target_L:%d,encoder_L:%d,PWM_L:%d,Motor_Pid_speed_Z:%d\n",1500,encoder_L,PWM_L,Motor_Pid_speed_Z);
	//printf("Target_L:%d,encoder_L:%d,PWM_L:%d,Motor_Pid_speed_Z:%d\n",1500,encoder_L,PWM_L,Motor_Pid_speed_Z);
	printf("start_left = %d,start_right = %d\n",start_left,start_right);
	printf("longest_line_row = %d, col = %d, len = %d\n", longest_line_row, longest_line_col, longest_line_len);
//  for (int r = MT9V03X_H - 1; r > longest_line_row; r--) {
//    printf("Row %d: left = %d, right = %d\n", r, left_sideline[r], right_sideline[r]);
//  }
//	for(int i=0;i<120;i++){
//		printf("left_sideline:%d,right_sideline:%d\n",left_sideline[i],right_sideline[i]);
//	}
}
void ds_wireless_uart(void){
	//wireless_uart_send_string("OK");
}