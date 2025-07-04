#ifndef __MENU_H__
#define __MENU_H__

typedef enum{
    fl=1,
    sl=2,
    tl=3,
} line_num; //定义菜单行号   
enum Module {
    Start=1,
    Image,
    Pid,
    Element,
};
// typedef enum {
//     //flag
//    int8  Start_Flag,Stop_Flag;


//    //show



//    //.parameter_show




//    //parameter_fix

// } Car_Status;
extern int8 cursor;
extern uint8_t cls_flag;
void show_start();
void show_image();
void show_pid();
void show_element();
void main_menu();
void menu_template();
void display_menu();
int menu_act(int MAX_ITEMS);
int menu2_act(int MAX_ITEMS);
#endif 