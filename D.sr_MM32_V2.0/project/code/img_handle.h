#ifndef _Img_h
#define _Img_h

#include "zf_common_headfile.h"

typedef struct 
{
    uint16 max;
    uint16 min;
    uint16 threshold;
    uint16 aver;
    uint16 bin_thrd;
}ImgInformation;
typedef struct{
    uint8_t left_sideline;
    uint8_t right_sideline;
    
    uint16_t midline;
    uint16_t Width;
    uint16_t left_qulu;
    uint16_t right_qulu;

}TrackInformation;
extern ImgInformation imginformation;
extern TrackInformation Trk;
extern uint8 left_sideline[120];
extern uint8 right_sideline[120];
extern uint8 longest_line_row;
extern uint8 longest_line_col;
extern uint8 longest_line_len;
extern uint8 start_left,start_right; 
void Img_handle(void);
void car_emergency_stop(void);
void Img_draw(void);
void Img_draw_clear(void);
#endif