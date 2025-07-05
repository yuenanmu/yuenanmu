#include "zf_common_headfile.h"
#include "menu.h"

uint8 cls_flag=0;

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
}
void ips200_show_Pid(void){
    ips200_show_string(1*offsetx,(1-1)*offsety,"Kp");
    ips200_show_string(1*offsetx,(2-1)*offsety,"Kd");
    ips200_show_string(1*offsetx,(3-1)*offsety,"Sd");
    ips200_show_string(1*offsetx,(4-1)*offsety,"Cs");

    ips200_show_string(1*offsetx,(5-1)*offsety,"Duty");
    ips200_show_string(1*offsetx,(6-1)*offsety,"Duer");
    
    ips200_show_string(1*offsetx,(7-1)*offsety,"Line_SP");
    ips200_show_string(1*offsetx,(8-1)*offsety,"Curve_SP");

    ips200_show_string(1*offsetx,(9-1)*offsety,"M_LP");
    ips200_show_string(1*offsetx,(10-1)*offsety,"M_LI");
    ips200_show_string(1*offsetx,(11-1)*offsety,"M_RP");
    ips200_show_string(1*offsetx,(12-1)*offsety,"M_RI");
    //ips200_show_float((1-1)*offsetx,(1-1)*offsety,data,int_part_len,2);
}
void ips200_show_Img(void){
    
}
void ips200_show_Element(void){
    ips200_show_string(1*offsetx,(1-1)*offsety,"Ring");
    ips200_show_string(1*offsetx,(2-1)*offsety,"Ramp");
    ips200_show_string(1*offsetx,(3-1)*offsety,"Cross");

    //ips200_show_float((1-1)*offsetx,(1-1)*offsety,data,int_part_len,2);
}
void ips200_show_Ring(void){
    
}
void ips200_show_Ramp(void){
    
}
void ips200_show_Cross(void){
    
}