#include "zf_common_headfile.h"
//8X16
// ips200_x_max            = 240;
// ips200_y_max            = 320;
//一级菜单：0/1 ：确认，返回  3/4：上，下
//二级菜单：0/1 ：确认，返回  3/4：加，减
uint8_t cls_flag;
char Start_Flag=0,Stop_Flag=0; //结构体
int8 cursor=1,delta;
int menu_act(int MAX_ITEMS) {
		int result=-1;
    if (key3_flag) {
        cursor =(cursor%MAX_ITEMS)+1;
        key3_flag = 0;
    } else if (key4_flag) {
        cursor = ((cursor - 2 + MAX_ITEMS) % MAX_ITEMS) + 1;  // 反向循环选择
        key4_flag = 0;
    }
    // 边界检查
//    cursor = (cursor < 1) ? 1 : cursor;           // 不能小于 1
//    cursor = (cursor > MAX_ITEMS) ? MAX_ITEMS : cursor; // 不能超过最大值
    // 处理确认键和返回键
    if (key1_flag) {
        key1_flag = 0;
        return cursor;  // 返回当前选项
    }
    if (key2_flag) {
        key2_flag = 0;
        cursor = 1;     // 重置光标
        return 0;      // 返回退出信号
    }

    return -1;  // 无操作
}
int menu2_act(int MAX_ITEMS) {
    static int cursor = 1;  // 静态变量，初始化并保持值

	if (key3_flag) {	//delta还需要一个倍率放大器
        delta=1;
        key3_flag = 0;
    } else if (key4_flag) {
        delta=-1;
        key3_flag = 0;
    }

    // 边界检查
    cursor = (cursor < 1) ? 1 : cursor;           // 不能小于 1
    cursor = (cursor > MAX_ITEMS) ? MAX_ITEMS : cursor; // 不能超过最大值
		
    // 处理确认键和返回键
    if (key1_flag) {
        key1_flag = 0;
        return cursor;  // 返回当前选项
    }
    if (key2_flag) {
        key2_flag = 0;
        cursor = 1;     // 重置光标
        return 0;      // 返回退出信号
    }

    return -1;  // 无操作
}

void menu_Init(){

}
void ips200_show_cursor(int8 cursor)
{

}
void display_menu()
{
	//在这个函数上面一行（函数外），加上ips200_clear();
    ips200_show_string(0, 2, "Main_Menu");
    ips200_show_string(16, 16, "Start");
	  ips200_show_string(0, 16, "->");
	
    ips200_show_string(16, 32, "Image");
    ips200_show_string(16, 48, "Pid");
    ips200_show_string(16, 64, "Element");
	  ips200_show_string(0, 300, "E2/E3:IN/OUT  E4:UP  E5:DOWN");
    ips200_show_cursor(cursor);
}
/*--------------------------------------------------------------一级菜单列表--------------------------------------------------------------------------------------------------------------*/
void main_menu()
{
	display_menu();
    while(1){
    menu_act(4);
	switch (cursor)
    {
        case Start:
        show_start();
        break;
    case Image:
        show_image();
        break;
    case Pid:
        show_pid();
        break;
    case Element:
        show_element();
        break;  
    default:
        break;
    }
    }
}

/*--------------------------------------------------------------二级菜单---------------------------------------------------------------------------------------------------*/
void show_start()
{
    ips200_clear();
    ips200_show_string(0, 0, "Start_Menu");
    ips200_show_string(16, 16, "Start_Flag: ");
    ips200_show_int(0,0,Start_Flag,1);
    ips200_show_string(0, 16, "Stop_Flag: ");
    ips200_show_int(0, 32,Stop_Flag, 1);
    while(1){
        menu_act(2);
        switch (cursor)
        {
        case 1:
            ips200_show_string(0, 80, "Image_Parameter");
            ips200_show_cursor(delta);
            break;
        case 2:
            ips200_show_string(0, 80, "Pid_Parameter");
            ips200_show_cursor(delta);
            break;
        case 3:
            ips200_show_string(0, 80, "Element_Parameter");
            ips200_show_cursor(delta);
            break;
        case 4:
            menu_template();
            break;
        default:
            break;
        }
    }

}
void show_image()
{
    ips200_clear();
    ips200_show_string(5, 2, "Img_Parameter");
        while(1){
        if(mt9v03x_init())
        {
            ips200_show_string(0, 16, "mt9v03x reinit.");
        }
        else
        {
            break;
        }
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }
        ips200_show_string(0, 16, "init success.");
    while(1){
        menu_act(4);
        	if(mt9v03x_finish_flag)
		{
			memcpy(image_copy, mt9v03x_image, MT9V03X_H*MT9V03X_W);
			//ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 1);
			ips200_show_gray_image(0, 0, (const uint8 *)image_copy, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
			mt9v03x_finish_flag=0;
		}
        
    }
}
void show_pid(){
	
}
void show_element(){
	
}


/*--------------------------------------------------------------二级电机菜单---------------------------------------------------------------------------------------------------*/

void menu_template(){
    ips200_clear();
    ips200_show_string(5, 2, "Motor_Parameter");
    ips200_show_string(0, 16, "1.Motor_Speed");
    ips200_show_string(0, 32, "2.Motor_Direction");
    ips200_show_string(0, 48, "3.Motor_Acceleration");
    ips200_show_string(0, 64, "4.Motor_Deceleration");
    while(1){
        menu2_act(4);
        switch (cursor)
        {
        case 1:
            ips200_show_string(0, 80, "Motor_Speed");
            ips200_show_cursor(delta);
            break;
        case 2:
            ips200_show_string(0, 80, "Motor_Direction");
            ips200_show_cursor(delta);
            break;
        case 3:
            ips200_show_string(0, 80, "Motor_Acceleration");
            ips200_show_cursor(delta);
            break;
        case 4:
            ips200_show_string(0, 80, "Motor_Deceleration");
            ips200_show_cursor(delta);
            break;
        default:
            break;
        }
    }

}




































/*
int  menu2_act(){
    if(key1_flag){
        return cursor;
        key1_flag=0;
    }
    if(key2_flag){
        cursor=1;
        return 0;
        key2_flag=0;
    }
    if(key3_flag){
        delta=1;
        key3_flag=0;
    }
    if(key4_flag){
        delta=-1;
        key4_flag=0;
    }
}
可以return  -1；超出三界之外，不在五行之中
*/











/*
typedef struct line {
    struct line* prior; // 前驱指针
    struct line* up;    // 上一级菜单指针
    int line_num;       // 菜单行号
    int data;           //显示的数据值
    int Serial;         //向串口发送的数据标识
    struct line* next;  //后继指针
} line;//采用双向链表实现高效访问

line* line_Init(){              //创建节点并初始化
    line *li =(line*)malloc(sizeof(line));
    if (li == NULL) {
        return NULL; // 分配失败
    }
    li->prior=li;
    li->next=li;    //前驱和后继都初始化为li自身，构成环形结构你
    li->up=NULL;
    return li;
}
line * menu_Init(){              //创建菜单
    line *li =(line*)malloc(sizeof(line));
    li->line_num=0; //数据字段初始化
    li->data=0;     
    li->prior=li->next=li->up; //同样形成构成环形结构
    return li;
}
void line_tail(line *li,int x){  //添加新节点到链表尾部
   line*ps=(line*)malloc(sizeof(line)); //创建新节点
    if (ps == NULL) {
         return; // 分配失败
    }
    ps->line_num=x; //设置行号
    ps->up=NULL;    
    ps->prior=li->prior; //将新节点的前驱指针指向链表的最后一个节点
    ps->next=(li->prior)->next; //将新节点的后继指针指向链表的第一个节点
    (li->prior)->next=ps; //将链表的最后一个节点的后继指针指向新节点
    li->prior=(li->prior)->next; //将链表的最后一个节点指向新节点
}
void menu_tail(line *pr,line *li,int x,int y,int z){ //添加新菜单到链表尾部
   line*ps=(line*)malloc(sizeof(line)); //创建新节点
   if(ps==NULL){
    return;    //分配失败
   }
   ps->line_num=x; //设置行号
   ps->data=y; //设置显示的数据值
   ps->Serial=z; //设置串口发送标识
   ps->up=li;
   li->up=ps; //将新节点的上一级菜单指针指向当前菜单

   ps->prior=pr->prior; //将新节点的前驱指针指向链表的最后一个节点
   ps->next=(pr->prior)->next; //将新节点的后继指针指向链表的第一个节点
   (pr->prior)->next=ps; //将链表的最后一个节点的后继指针指向新节点  
   ps->prior=(pr->prior)->next; //将链表的最后一个节点指向新节点
}
void menu_tail_t(line *pr,line *li,int x,int y,int z){ //添加新菜单到链表尾部
   line*ps=(line*)malloc(sizeof(line)); //创建新节点
   if(ps==NULL){
    return;    //分配失败
   }
   ps->line_num=x; //设置行号
   ps->data=y; //设置显示的数据值
   ps->Serial=z; //设置串口发送标识
   ps->up=li;
   //li->up=ps; //将新节点的上一级菜单指针指向当前菜单  这一步取消

   ps->prior=pr->prior; //将新节点的前驱指针指向链表的最后一个节点
   ps->next=(pr->prior)->next; //将新节点的后继指针指向链表的第一个节点
   (pr->prior)->next=ps; //将链表的最后一个节点的后继指针指向新节点  
   ps->prior=(pr->prior)->next; //将链表的最后一个节点指向新节点
}
line *list_Init(void){          //菜单初始化
   line* l1=line_Init(); //创建链表头节点
   line_tail(l1,fl);
   line_tail(l1,sl);
   line_tail(l1,tl);
   //menu_tail(l1,l1,1,0,0); //创建菜单
   line*pz=l1->next; //指向第一个菜单节点


   line*l2=line_Init(); //创建子菜单
   menu_tail(l2,pz,fl,0,0);//创建连接功能
   menu_tail_t(l2,pz,sl,1,2);//创建正真的功能位，且对应的菜单位不与他的产生联系
   pz=pz->next;

   l2=line_Init(); //创建子菜单
   menu_tail(l2,pz,fl,0,0);//创建连接功能
   menu_tail_t(l2,pz,sl,1,3);//创建正真的功能位，且对应的菜单位不与他的产生联系
   pz=pz->next;

    l2=line_Init(); //创建子菜单 
    menu_tail(l2,pz,fl,0,0);//创建连接功能  
    menu_tail_t(l2,pz,sl,1,4);//创建正真的功能位，且对应的菜单位不与他的产生联系

    return l1; //返回链表头节点
}
void list_menu(void){       //一级菜单
    ips200_clear(); 
    ips200_show_char(1,1,"*");
    ips200_show_char(1,2,"L:Min or Max");
    ips200_show_string(1,3,"A:Min or Max");
    ips200_show_string(1,3,"B:Min or Max");
}
void list_menu2(line*pr){      //二级菜单
    ips200_clear();
    ips200_show_char(1,1,"*");
    ips200_show_string(1,2,"Max");
    ips200_show_string(1,3,"Min");
    ips200_show_num(1,4,pr->data,5); //显示当前值
    ips200_show_num(1,5,pr->next->data,5); //显示当前值
}
*/  
