#include "zf_common_headfile.h"
// ips200_x_max            = 240;
// ips200_y_max            = 320;
void menu_Init(){

}
void menu()
{
	
}

void show_image()
{
    //ips200_init(IPS200_TYPE_SPI);
    //ips200_clear();
    ips200_show_string(5, 2, "Img_Parameter");
}






















































/*
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
