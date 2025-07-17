#ifndef _menu_h
#define _menu_h

#include "zf_common_headfile.h"

extern uint8 cls_flag;

void ds_main_menu(void);
void ds_ips200_init(void);
void ips200_show_CarGo(void);
void ips200_show_Pid(void);
void ips200_show_Img(void);
void ips200_show_Element(void);
void ips200_show_Ring(void);
void ips200_show_Ramp(void);
void ips200_show_Cross(void);

#endif