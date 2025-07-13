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

void Img_handle(void);
void car_emergency_stop(void);
#endif