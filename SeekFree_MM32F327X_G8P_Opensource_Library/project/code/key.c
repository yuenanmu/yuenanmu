#include "zf_common_headfile.h"
#include "key.h"
//待增加函数
void Duct_start(){};
	
//获取时间
uint32 Tim1_count_time;                                                // 定时器分时复用
//内部使用判断按键逻辑
uint8_t key1_flag=0;
uint8_t key2_flag=0;
uint8_t key3_flag=0;
uint8_t key4_flag=0;
uint8_t key5_flag=0;
uint8_t key6_flag=0;

//作为外部接口
uint8_t model=1;      //光标
uint8_t grade_flag=0; //确认标志位
uint8_t ips200_show_flag=0;//二级
uint8_t add; //加
uint8_t sub; //subtract
uint8_t return_flag=0; //返回标志位

uint8_t Augle_flag=0; //加法标志位
uint8_t key_flag=0;
uint8_t data_buff[85]={0};

uint8_t ips200_show_element_flag=0; //二级显示元素标志位
uint8_t ips200_show_ring_flag=0; //二级显示元素标志位2

uint8_t offsetx=8;
uint8_t offsety=16;
//void key(void){
//    static uint8_t key_status[4]={1,1,1,1}; //按键状态
//    static uint8_t key_last_status[4]={1,1,1,1}; //按键上次状态
//    static uint8_t key_press_time[4]={0,0,0,0}; //按键计时
//		static uint32_t press_duration;
//    for(int i=0;i<4;i++){
//        key_last_status[i]=key_status[i]; //保存上次状态
//        //key_status[i]=gpio_get_level(KEY1+i); //读取当前按键状态
//        //采用条件运算符
//        key_status[i]=(i==0)?gpio_get_level(KEY1):
//                      (i==1)?gpio_get_level(KEY2):
//                      (i==2)?gpio_get_level(KEY3):
//                      gpio_get_level(KEY4); //读取当前按键状态
//    }
//    
//    for(int i=0;i<4;i++){

//        //按键逻辑判断
//        if(key_status[i]==0&& key_last_status[i]==1){ //按键按下瞬间，跳变
//            key_press_time[i]=Tim1_count_time;//计时清零
//        }else if(key_status[i]==1 && key_last_status[i]==0){ //按键松开瞬间，跳变
//            press_duration =  Tim1_count_time-key_press_time[i];
//				}
//				
//				if(press_duration<1000){ //短按
//						switch(i){
//								case 0: key1_flag=1; break; //KEY1,
//								case 1: key2_flag=1; break; //KEY2
//								case 2: key3_flag=1; break; //KEY3
//								case 3: key4_flag=1; break; //KEY4
//						}
//				}else if(press_duration>=1000&& press_duration<2000){ //长按，maybe:press_duration>=1000 && press_duration<2000
//						switch(i){
//								case 2: key5_flag=1; break; //KEY5
//								case 3: key6_flag=1; break; //KEY6
//						}
//    }
//}
//		if(key1_flag){key1_flag=0;return_flag=1;}
//		if(key2_flag){key2_flag=0;grade_flag++;}
//		if(key3_flag){key2_flag=0;model++;}
//		if(key4_flag){key4_flag=0;model--;}
//		if(key5_flag){key5_flag=0;add++;}
//		if(key6_flag){key6_flag=0;sub++;}
//    
//}
// 定义按键状态枚举
typedef enum {
    KEY_STATE_RELEASED,    // 释放状态
    KEY_STATE_PRESSED,     // 按下状态
    KEY_STATE_SHORT_PRESS, // 短按触发
    KEY_STATE_LONG_PRESS   // 长按触发
} KeyState;

void key(void) {
    static KeyState key_state[4] = {KEY_STATE_RELEASED};
    static uint32_t press_start_time[4] = {0};
    static uint8_t debounce_cnt[4] = {0};
    
    const uint8_t DEBOUNCE_THRESHOLD = 5;  // 消抖阈值
    const uint32_t LONG_PRESS_TIME = 1000; // 长按时间阈值（单位需与Tim1_count_time一致）
    
    for (int i = 0; i < 4; i++) {
        // 读取当前按键电平（低电平表示按下）
        uint8_t key_level = (i == 0) ? gpio_get_level(KEY1) :
                          (i == 1) ? gpio_get_level(KEY2) :
                          (i == 2) ? gpio_get_level(KEY3) :
                                     gpio_get_level(KEY4);
        
        // 软件消抖
        if (key_level == 0) {
            if (++debounce_cnt[i] >= DEBOUNCE_THRESHOLD) {
                // 确认按下
                if (key_state[i] == KEY_STATE_RELEASED) {
                    key_state[i] = KEY_STATE_PRESSED;
                    press_start_time[i] = Tim1_count_time;
                } else if (key_state[i] == KEY_STATE_PRESSED && 
                          (Tim1_count_time - press_start_time[i]) >= LONG_PRESS_TIME) {
                    // 触发长按
                    key_state[i] = KEY_STATE_LONG_PRESS;
                }
            }
        } else {
            debounce_cnt[i] = 0;  // 重置消抖计数
            
            // 处理释放事件
            if (key_state[i] == KEY_STATE_PRESSED) {
                // 触发短按
                key_state[i] = KEY_STATE_SHORT_PRESS;
            } else if (key_state[i] == KEY_STATE_LONG_PRESS) {
                key_state[i] = KEY_STATE_RELEASED;
            }
        }
    }
    
    // 根据按键状态设置标志位（在消抖后处理）
    for (int i = 0; i < 4; i++) {
        if (key_state[i] == KEY_STATE_SHORT_PRESS) {
            switch (i) {
                case 0: key1_flag = 1; break;  // KEY1 短按
                case 1: key2_flag = 1; break;  // KEY2 短按
                case 2: key3_flag = 1; break;  // KEY3 短按
                case 3: key4_flag = 1; break;  // KEY4 短按
            }
            key_state[i] = KEY_STATE_RELEASED;  // 重置状态
        } else if (key_state[i] == KEY_STATE_LONG_PRESS) {
            switch (i) {
                case 2: key5_flag = 1; break;  // KEY3 长按
                case 3: key6_flag = 1; break;  // KEY4 长按
            }
            key_state[i] = KEY_STATE_RELEASED;  // 重置状态
        }
    }
    
    // 处理标志位（与按键扫描分离，避免嵌套过深）
    if (key1_flag) { key1_flag = 0; return_flag = 1; }
    if (key2_flag) { key2_flag = 0; grade_flag++; }
    if (key3_flag) { key3_flag = 0; model++; }
    if (key4_flag) { key4_flag = 0; model--; }
    if (key5_flag) { key5_flag = 0; add++; }
    if (key6_flag) { key6_flag = 0; sub++; }
}
void Parameter_fix(void){
    //光标
   ips200_show_string(0,(model-1)*offsety,"->");
   ips200_show_string(0,((model+14)%16)*offsety,"  ");
   ips200_show_string(0,model==16?offsety:model*offsety," ");
   if(grade_flag==1){
    switch (model)  
    {
    case 1:
        ips200_show_flag=100;grade_flag=2;model=0;cls_flag=0;Duct_start();
        break;
    case 2:
        ips200_show_flag=1;grade_flag=2;model=0;cls_flag=0;
        break;
    case 3:
        ips200_show_flag=2;grade_flag=2;model=0;cls_flag=0;
        break;
    case 4: 
        ips200_show_flag=3;grade_flag=2;model=0;cls_flag=0;
        break;
    default:
        model=1;
        break;
    }
   }
   if(ips200_show_flag==100){
    if(grade_flag==3){
        start_flag=1;
        Augle_flag=1;
        key_flag=key_flag;
    }
    if(return_flag==1){ //返回键
        return_flag=0;
        ips200_show_flag=0;
        model=1;
        cls_flag=0;
        grade_flag=0;
    }
   }
//    if(ips200_show_flag==1){
//     if(add==1){
//         switch(model){
//             case 1:
//                 case:Pid_Kp+=0.1; break;
//             case 2:
//                 case:Pid_Kd+=1; break;

//         }
//     }
//     if(return_flag==1){ //返回键
//         return_flag=0;
//         ips200_show_flag=0;
//         model=1;
//         cls_flag=0;
//         grade_flag=0;
//     }
//    }
}