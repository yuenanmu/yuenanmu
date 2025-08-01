#include "zf_common_headfile.h"
#include "Serial.h"
void ds_serial(void){
	//printf("good\n");
	//printf("%d,%d,%d\n",imu660ra_gyro_x,imu660ra_gyro_y,imu660ra_gyro_z);
	//printf("Target_L:%d,encoder_L:%d,PWM_L:%d,Motor_Pid_speed_Z:%d\n",1500,encoder_L,PWM_L,Motor_Pid_speed_Z);
//	printf("Track.Err:%.2f   Dif:%.2f  Dif_Sd:%.2f\n",Track.Err,Motor_Pid.Dif_Speed*Motor_Pid.Dif_P,Motor_Pid.Dif_Speed);
//	printf("Target_L:%d,encoder_L:%d,PWM_L:%d,Motor_Pid_speed_Z:%d,Motor_Pid.speed_L:%.2f,Motor_error_L:%d\n",50,encoder_L,PWM_L,Motor_Pid_speed_Z,Motor_Pid.speed_L,Motor_error_L);
//	printf("Target_R:%d,encoder_R:%d,PWM_R:%d,Motor_Pid_speed_Z:%d,Motor_Pid.speed_R:%.2f,Motor_error_R:%d\n",50,encoder_R,PWM_R,Motor_Pid_speed_Z,Motor_Pid.speed_R,Motor_error_R);
//	printf("start_left = %d,start_right = %d\n",start_left,start_right);
//	printf("longest_line_row = %d, col = %d, len = %d\n", longest_line_row, longest_line_col, longest_line_len);
//  for (int r = MT9V03X_H - 1; r > longest_line_row; r--) {
//    printf("Row %d: left = %d, right = %d\n", r, left_sideline[r], right_sideline[r]);
//  }
//	for(int i=0;i<120;i++){
//		printf("left_sideline:%d,right_sideline:%d\n",left_sideline[i],right_sideline[i]);
//	}
}

// 辅助函数：将格式化字符串转换为缓冲区
static void wireless_printf(const char *format, ...) {
    char buffer[256]; // 根据实际需要调整缓冲区大小
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    wireless_uart_send_string(buffer);
}

void send_to_vofa(void) {
   // wireless_uart_send_string("good\n");
//    wireless_printf("%.2f,%.2f,%.2f\r\n",Track.Err, Motor_Pid.Dif_Speed*Motor_Pid.Dif_P, Motor_Pid.Dif_Speed);
    //wireless_printf("%.2f,%d,%d,%d\r\n",Motor_Pid.speed, encoder_L, PWM_L,Motor_error_L);
		wireless_printf("%.2f,%d,%d,%d\r\n",Motor_Pid.speed, encoder_R, PWM_R,Motor_error_R);
//    wireless_printf("%d,%d,%d,%d,%.2f,%d\r\n",50, encoder_R,PWM_R,Motor_Pid_speed_Z,Motor_Pid.speed_R, Motor_error_R);
    //wireless_printf("%d,%d,%d,%.2f,%.2f,%d,%d\r\n",110, encoder_L,PWM_L,Motor_Pid.L_Ki,Motor_Pid.L_Kp,Motor_Pid_Z_L_Ki,Motor_Pid_Z_L_Kp);
  	//wireless_printf("%d,%d,%d,%.2f,%.2f,%d,%d\r\n",0, encoder_R,PWM_R,Motor_Pid.R_Ki,Motor_Pid.R_Kp,Motor_Pid_Z_R_Ki,Motor_Pid_Z_R_Kp);
	   //wireless_printf("%.2f,%.2f,%2.f,%2.f,%2.f,%2.f\n",Track.Err,Motor_Pid.speed_L,Motor_Pid.speed_R,Motor_Pid.Dif_Speed*Motor_Pid.Dif_P,Motor_Pid.Dif_Speed,Motor_Pid.Dif_P);
	   //wireless_printf("%.2f,%.2f,%d,%.2f,%d,%.2f\n",Track.Err,Motor_Pid.speed_L,encoder_L,Motor_Pid.speed_R,encoder_R,Motor_Pid.Dif_Speed);
	 //wireless_printf("%.2f,%d,%d,%d,%d,%.2f\n",Track.Err,PWM_L,encoder_L,PWM_R,encoder_R,Motor_Pid.Dif_Speed);
}

// 替换后的调试输出
void ds_wireless_uart(void){
    // 第一行
    wireless_uart_send_string("good\r\n");
    
    // 第二行 - 轨道误差和转向信息
    wireless_printf("Track.Err:%.2f   Dif:%.2f  Dif_Sd:%.2f\r\n",
                  Track.Err, 
                  Motor_Pid.Dif_Speed*Motor_Pid.Dif_P, 
                  Motor_Pid.Dif_Speed);
    
    // 第三行 - 左侧电机信息
    wireless_printf("Target_L:%d,encoder_L:%d,PWM_L:%d,Motor_Pid_speed_Z:%d,Motor_Pid.speed_L:%.2f,Motor_error_L:%d\r\n",
                  50, encoder_L, PWM_L, 
                  Motor_Pid_speed_Z, 
                  Motor_Pid.speed_L, 
                  Motor_error_L);
    
    // 第四行 - 右侧电机信息
    wireless_printf("Target_R:%d,encoder_R:%d,PWM_R:%d,Motor_Pid_speed_Z:%d,Motor_Pid.speed_R:%.2f,Motor_error_R:%d\r\n",
                  50, encoder_R, PWM_R, 
                  Motor_Pid_speed_Z, 
                  Motor_Pid.speed_R, 
                  Motor_error_R);
}