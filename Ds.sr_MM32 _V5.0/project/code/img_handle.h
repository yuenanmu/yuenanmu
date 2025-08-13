#ifndef _Img_h
#define _Img_h
#include "zf_common_headfile.h"
typedef struct {
    uint8_t left[MT9V03X_H];            // 左边线每一行的位置
    uint8_t right[MT9V03X_H];           // 右边线每一行的位置
    uint8_t width[MT9V03X_H];           // 每一行赛道宽度
    uint8_t center[MT9V03X_H];          // 中线位置（左+右）/2
    uint8_t left_lost_flag[MT9V03X_H];  // 左边是否丢线
    uint8_t right_lost_flag[MT9V03X_H]; // 右边是否丢线
    uint8_t both_lost_time;             // 同时丢线的行数
    uint8_t left_lost_time;             // 左边丢线行数
    uint8_t right_lost_time;            // 右边丢线行数
    uint8_t left_start_row;             // 左边第一个未丢线行
    uint8_t right_start_row;            // 右边第一个未丢线行
    uint8_t longest_white_column;       // 白列最长的列编号
    uint8_t longest_white_length;       // 该列长度
		float Err;
}ds_Track_Boundary;
extern ds_Track_Boundary Track;
extern uint8 threshold;
extern uint8 image_two_value[MT9V03X_H][MT9V03X_W];
extern uint8 Zebra_Flag;
extern volatile uint8 Curve_Flag;
extern volatile uint8 Straight_Flag;
extern int16 foresight_line;
extern volatile int Search_Stop_Line;
extern uint8 Std_Line;
extern double result;

int  my_adapt_threshold(uint8*image,uint16 col, uint16 row);

void Longest_White_Column(void);
void Image_Binarization(int threshold);

void Cross_Detect(void);

void Show_Boundry(void);
void Find_Up_Point(int start,int end);
void Find_Down_Point(int start,int end);
void Lengthen_Left_Boundry(int start,int end);
void Lengthen_Right_Boundry(int start,int end);
void Left_Add_Line(int x1,int y1,int x2,int y2);
void Right_Add_Line(int x1,int y1,int x2,int y2);
void Draw_Line(int startX, int startY, int endX, int endY);

void Img_Processing(void);
void Get_UseImg(void);
void car_emergency_stop(void);
void Img_draw(void);
void Img_draw_clear(void);
#endif
