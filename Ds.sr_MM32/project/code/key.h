#ifndef _key_h
#define _key_h

#include "zf_common_headfile.h"

#define KEY1                    (E2 )
#define KEY2                    (E3 )
#define KEY3                    (E4 )
#define KEY4                    (E5 )

#define SWITCH1                 (D3 )
#define SWITCH2                 (D4 )
#define KEY5 SWITCH1
#define KEY6 SWITCH2

#define FLASH_SECTION_INDEX       (127)    // 存储数据用的扇区 倒数第一个扇区
#define FLASH_PAGE_INDEX          (3)      // 存储数据用的页码 倒数第一个页码

extern uint32 key1_count;
extern uint32 key2_count;
extern uint32 key3_count;
extern uint32 key4_count;
extern uint32 key5_count;
extern uint32 key6_count;
extern uint32 count_time;
extern uint8  key1_flag;
extern uint8  key2_flag;
extern uint8  key3_flag;
extern uint8  key4_flag;
extern uint8  switch_flag;
extern uint8  key5_flag;
extern uint8  key6_flag;
extern uint8  key_flag;

//开放接口给菜单显示函数
extern uint8 offsetx;
extern uint8 offsety;
extern uint8 ips200_show_flag; //菜单显示标志
extern uint8 ips200_show_element_flag;
extern uint8 ips200_show_ring_flag;
extern uint8  return_flag;
extern uint8	Model;	//光标
extern uint8	cls_flag;//清屏
extern uint8	ips200_show_flag;	// 返回到主菜单
extern uint8	grade_flag;		// 重置菜单层级
extern uint8	lose;	// 返回到主菜单
extern uint8	plus;		// 重置菜单层级

void key(void);
void ParameterExchange(void);
void EepromWrite(void);
void EepromRead(void);
#endif