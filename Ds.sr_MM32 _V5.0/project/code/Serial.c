#include "zf_common_headfile.h"
#include "Serial.h"
void ds_serial(void){
	//printf("good\n");
	printf("%d,%d,%d\n",gx,gy,gz);
	//printf("%.2f,%d,%d\r\n",Motor_Pid.speed,(encoder_R+encoder_L)>>1,(PWM_L+PWM_R)>>1);
	//printf("Target_L:%d,encoder_L:%d,PWM_L:%d,Motor_Pid_speed_Z:%d\n",1500,encoder_L,PWM_L,Motor_Pid_speed_Z);
//	printf("Track.Err:%.2f   Dif:%.2f  Dif_Sd:%.2f\n",Track.Err,Motor_Pid.Dif_Speed*Motor_Pid.Dif_P,Motor_Pid.Dif_Speed);
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
	//	wireless_printf("%.2f,%d,%d,%d\r\n",Motor_Pid.speed, encoder_R, PWM_R,Motor_error_R);
//    wireless_printf("%d,%d,%d,%d,%.2f,%d\r\n",50, encoder_R,PWM_R,Motor_Pid_speed_Z,Motor_Pid.speed_R, Motor_error_R);
    //wireless_printf("%d,%d,%d,%.2f,%.2f,%d,%d\r\n",110, encoder_L,PWM_L,Motor_Pid.L_Ki,Motor_Pid.L_Kp,Motor_Pid_Z_L_Ki,Motor_Pid_Z_L_Kp);
  	//wireless_printf("%d,%d,%d,%.2f,%.2f,%d,%d\r\n",0, encoder_R,PWM_R,Motor_Pid.R_Ki,Motor_Pid.R_Kp,Motor_Pid_Z_R_Ki,Motor_Pid_Z_R_Kp);
	   //wireless_printf("%.2f,%.2f,%2.f,%2.f,%2.f,%2.f\n",Track.Err,Motor_Pid.speed_L,Motor_Pid.speed_R,Motor_Pid.Dif_Speed*Motor_Pid.Dif_P,Motor_Pid.Dif_Speed,Motor_Pid.Dif_P);
	   //wireless_printf("%.2f,%.2f,%d,%.2f,%d,%.2f\n",Track.Err,Motor_Pid.speed_L,encoder_L,Motor_Pid.speed_R,encoder_R,Motor_Pid.Dif_Speed);
	 //wireless_printf("%.2f,%d,%d,%d,%d,%.2f\n",Track.Err,PWM_L,encoder_L,PWM_R,encoder_R,Motor_Pid.Dif_Speed);
}
