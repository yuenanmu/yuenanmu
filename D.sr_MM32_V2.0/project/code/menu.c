#include "zf_common_headfile.h"
#include "menu.h"

uint8 cls_flag=0;
void display_menu_status(void){
//			ips200_show_string((2-1)*offsetx,(10-1)*offsety,"return_flag:");
//			  ips200_show_int((15-1)*offsetx,(10-1)*offsety,return_flag,4);
//			
//			ips200_show_string((2-1)*offsetx,(11-1)*offsety,"grade_flag:");
//			  ips200_show_int((15-1)*offsetx,(11-1)*offsety,grade_flag,4);
//			
//			ips200_show_string((2-1)*offsetx,(12-1)*offsety,"lose:");
//			  ips200_show_int((15-1)*offsetx,(12-1)*offsety,lose,4);
//				
//			ips200_show_string((2-1)*offsetx,(13-1)*offsety,"plus");
//			  ips200_show_int((15-1)*offsetx,(13-1)*offsety,plus,4);
//				
//				ips200_show_string((2-1)*offsetx,(14-1)*offsety,"Model:");
//				ips200_show_int((12-1)*offsetx,(14-1)*offsety,Model,4);
//				
//				ips200_show_string((2-1)*offsetx,(15-1)*offsety,"ips200_show_flag:");
//				ips200_show_int((24-1)*offsetx,(15-1)*offsety,ips200_show_flag,4);
	
	
			ips200_show_string((2-1)*offsetx,(20-1)*offsety,"re:");
			  ips200_show_int((5-1)*offsetx,(20-1)*offsety,return_flag,1);
			
			ips200_show_string((7-1)*offsetx,(20-1)*offsety,"gr:");
			  ips200_show_int((10-1)*offsetx,(20-1)*offsety,grade_flag,1);
			
			ips200_show_string((12-1)*offsetx,(20-1)*offsety,"pl:");
			  ips200_show_int((15-1)*offsetx,(20-1)*offsety,plus,1);
				
				ips200_show_string((17-1)*offsetx,(20-1)*offsety,"lo:");
			  ips200_show_int((20-1)*offsetx,(20-1)*offsety,lose,1);
				
				ips200_show_string((22-1)*offsetx,(20-1)*offsety,"Mo:");
				ips200_show_int((25-1)*offsetx,(20-1)*offsety,Model,2);
				
				ips200_show_string((27-1)*offsetx,(20-1)*offsety,"ip:");
				ips200_show_int((30-1)*offsetx,(20-1)*offsety,ips200_show_flag,1);

}
void ds_ips200_init(void){
    ips200_init(IPS200_TYPE_SPI);
    ips200_clear();
	system_delay_ms(100);
}
void ds_main_menu(void){
    if(cls_flag==0){
        ips200_clear();
        cls_flag=1;
    }
    if(ips200_show_flag==0){
        //
        ips200_show_string(1*offsetx,(1-1)*offsety,"CarGo");
        ips200_show_string(1*offsetx,(2-1)*offsety,"Pid");
        ips200_show_string(1*offsetx,(3-1)*offsety,"Img");
        ips200_show_string(1*offsetx,(4-1)*offsety,"Element");
        //状态参数显示
        //ips200_show_float((1-1)*offsetx,(1-1)*offsety,data,int_part_len,2);
        //ips200_show_int((1-1)*offsetx,(1-1)*offsety,data,len);
			
        ips200_show_string(1*offsetx,(5-1)*offsety,"Status");
        ips200_show_string((2-1)*offsetx,(6-1)*offsety,"encoder_L:");
        ips200_show_int((12-1)*offsetx,(6-1)*offsety,encoder_L,5);
				
			  display_menu_status();
//        ips200_show_string((16-1)*offsetx,(6-1)*offsety,"encoder_R:");
//        ips200_show_int((26-1)*offsetx,(6-1)*offsety,encoder_R,4);

//        ips200_show_string((1-1)*offsetx,(7-1)*offsety,"Desire_Speed:");
//        ips200_show_float((10-1)*offsetx,(7-1)*offsety,Desire_Speed,2,1);

//        ips200_show_string((1-1)*offsetx,(8-1)*offsety,"Motor_Pid.speed_L");
//        ips200_show_float((10-1)*offsetx,(8-1)*offsety,Motor_Pid.speed_L,2,1);
//        ips200_show_string((15-1)*offsetx,(8-1)*offsety,"Motor_Pid.speed_R");
//        ips200_show_float((25-1)*offsetx,(8-1)*offsety,Motor_Pid.speed_R,2,1);
    }
    if(ips200_show_flag==100)ips200_show_CarGo();
    if(ips200_show_flag==1)ips200_show_Pid();
    if(ips200_show_flag==2)ips200_show_Img();
    if(ips200_show_flag==3)ips200_show_Element();

    if(ips200_show_element_flag==1)ips200_show_Ring();
    if(ips200_show_element_flag==2)ips200_show_Ramp();
    if(ips200_show_element_flag==3)ips200_show_Cross();
    
    if(ips200_show_flag==100)ips200_show_CarGo();
}
void ips200_show_CarGo(void){
    ips200_show_string(1*offsetx,(1-1)*offsety,"CarGo");
    ips200_show_int((10-1)*offsetx,(1-1)*offsety,start,1);
	  display_menu_status();
}
void ips200_show_Pid(void){
    ips200_show_string(1*offsetx,(1-1)*offsety,"Sd");
    ips200_show_string(1*offsetx,(2-1)*offsety,"Cs");
    
    ips200_show_string(1*offsetx,(3-1)*offsety,"Line_SP");
    ips200_show_string(1*offsetx,(4-1)*offsety,"Curve_SP");

    ips200_show_string(1*offsetx,(5-1)*offsety,"M_LI");
    ips200_show_string(1*offsetx,(6-1)*offsety,"M_LP");
    ips200_show_string(1*offsetx,(7-1)*offsety,"M_RI");
    ips200_show_string(1*offsetx,(8-1)*offsety,"M_RP");
    //ips200_show_float((1-1)*offsetx,(1-1)*offsety,data,int_part_len,2);
    ips200_show_float((10-1)*offsetx,(1-1)*offsety,Motor_Pid_speed_Z,3,2);
    ips200_show_float((10-1)*offsetx,(2-1)*offsety,Motor_Pid.Dif_P,3,2);

    ips200_show_float((10-1)*offsetx,(3-1)*offsety,Linear_speed,3,1);
		ips200_show_float((10-1)*offsetx,(4-1)*offsety,Curve_speed,3,1);
		
		ips200_show_float((10-1)*offsetx,(5-1)*offsety,Motor_Pid.L_Ki,2,3);
		ips200_show_float((10-1)*offsetx,(6-1)*offsety,Motor_Pid.L_Kp,3,3);
		ips200_show_float((10-1)*offsetx,(7-1)*offsety,Motor_Pid.R_Ki,2,3);
		ips200_show_float((10-1)*offsetx,(8-1)*offsety,Motor_Pid.R_Kp,3,3);
		
		display_menu_status();
}
void ips200_show_Img(void){
    ips200_show_int((5-1)*offsetx,(1-1)*offsety,imginformation.threshold,3);
		ips200_show_gray_image((2-1)*offsetx,(2-1)*offsety, (const uint8 *)image_copy, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
		//ips200_show_gray_image((2-1)*offsetx,(10-1)*offsety, (const uint8 *)image_twovalue, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);
		//Img_draw();
		display_menu_status();
}
void ips200_show_Element(void){
    ips200_show_string(1*offsetx,(1-1)*offsety,"Ring");
    ips200_show_string(1*offsetx,(2-1)*offsety,"Ramp");
    ips200_show_string(1*offsetx,(3-1)*offsety,"Cross");

    //ips200_show_float((1-1)*offsetx,(1-1)*offsety,data,int_part_len,2);
	  display_menu_status();
}
void ips200_show_Ring(void){
    display_menu_status();
}
void ips200_show_Ramp(void){
    display_menu_status();
}
void ips200_show_Cross(void){
    display_menu_status();
}