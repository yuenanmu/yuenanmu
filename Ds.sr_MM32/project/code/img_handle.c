#include "zf_common_headfile.h"
#include "img_handle.h"
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
ImgInformation imginformation;
TrackInformation Trk;
// MT9V03X_W               ( 188 )     
// MT9V03X_H               ( 120 ) 
uint8  image_twovalue[MT9V03X_H][MT9V03X_W];
//uint8  center_line[row][center_col]={0};
uint8  center_line[MT9V03X_H][MT9V03X_W];
uint8 left_flag=0;
uint8 right_flag=0;

uint8 black_write=0;

int calculate_threshold(uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W]){
    signed short i, j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelshortegralBack = 0;
    unsigned long Pixelshortegral = 0;
    signed long PixelshortegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    signed short MinValue, MaxValue;
    signed short Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j <MT9V03X_H; j++)
    {
        for (i = 0; i < MT9V03X_W; i++)
        {
            HistoGram[mt9v03x_image[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++);        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--); //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j];        //  像素总数

    Pixelshortegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        Pixelshortegral += HistoGram[j] * j;        //灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];     //前景像素点数
        PixelFore = Amount - PixelBack;           //背景像素点数
        OmegaBack = (float) PixelBack / Amount;   //前景像素百分比
        OmegaFore = (float) PixelFore / Amount;   //背景像素百分比
        PixelshortegralBack += HistoGram[j] * j;  //前景灰度值
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //背景灰度值
        MicroBack = (float) PixelshortegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float) PixelshortegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);   //计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;     

}
void binarize_image(int threshold){
    for(int nr=0;nr<MT9V03X_H;nr++){
        for(int nc=0;nc<MT9V03X_W;nc++){
            if(mt9v03x_image[nr][nc]>threshold)
                image_twovalue[nr][nc]=1;
            else 
                image_twovalue[nr][nc]=0;
        }
    }
    // for(int i=0;i<MT9V03X_W*MT9V03X_H;i++){
    //     if(mt9v03x_image[i/MT9V03X_W][i%MT9V03X_W]>YUZHI){ //大于阈值
    //         mt9v03x_image[i/MT9V03X_W][i%MT9V03X_W]=0; //黑色
    // }else{
    //     mt9v03x_image[i/MT9V03X_W][i%MT9V03X_W]=1; //小于阈值]
    // }
}

void Get_sideline(){

}
void Get_midline(){
    for (int row = 0; row < MT9V03X_H; row++) {
        int left = -1, right = -1;
        for (int col = 0; col < MT9V03X_W; col++) {
            if (mt9v03x_image[row][col] == 0) { // 黑色像素
                if (left == -1) {
                    left = col; // 记录左边界
                }
                right = col; // 更新右边界,在白列跳变的那一下
            }
        }
        if (left != -1 && right != -1) {
            int center_col = (left + right) / 2; // 计算中心列
            center_line[row][center_col] = 1; // 标记中心线
        }
    }
}
void Bin_Image_Filter(void){
    for(int nr=1;nr<MT9V03X_H-1;nr++){
        for(int nc=1;nc<MT9V03X_W-1;nc++){
            if(mt9v03x_image[nr][nc]==0 && (mt9v03x_image[nr+1][nc]+mt9v03x_image[nr-1][nc]+mt9v03x_image[nr][nc+1]+mt9v03x_image[nr][nc-1])>2){
                mt9v03x_image[nr][nc]=1;
            }else if(mt9v03x_image[nr][nc]==1 && (mt9v03x_image[nr+1][nc]+mt9v03x_image[nr-1][nc]+mt9v03x_image[nr][nc+1]+mt9v03x_image[nr][nc-1])<2){
            mt9v03x_image[nr][nc]=0;
         }
			}
		}
}
void Img_handle(void){
//	  imginformation.threshold=calculate_threshold(mt9v03x_image);
//    binarize_image(imginformation.threshold);
    if(mt9v03x_finish_flag)
		{
			memcpy(image_copy, mt9v03x_image, MT9V03X_H*MT9V03X_W);
			ips200_show_gray_image((1-1)*offsetx,(10-1)*offsety, (const uint8 *)image_copy, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
			//ips200_show_binary_image((1-1)*offsetx,(10-1)*offsety, (const uint8 *)image_twovalue, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H);
      mt9v03x_finish_flag=0;
      //
		}
}