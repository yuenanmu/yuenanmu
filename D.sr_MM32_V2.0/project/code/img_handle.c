#include "zf_common_headfile.h"
#include "img_handle.h"
#define BLACK 0x00
#define WHITE 0xff
// MT9V03X_W               ( 188 )     
// MT9V03X_H               ( 120 ) 
ImgInformation imginformation;
TrackInformation Trk;
//uint8  center_line[row][center_col]={0};
uint8  center_line[MT9V03X_H][MT9V03X_W];
uint8 left_flag=0;
uint8 right_flag=0;

uint8 black_write=0;
/*----------------------------------------------图像处理部分------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------
  @brief     计算阈值
  @param     起始行，终止行
  @return    返回角点所在的行数，找不到返回0
  Sample     calculate_threshold(*image_copy,MT9V03X_H,MT9V03X_W);
  @note      用ips200_show_gray_image();函数显示图像
-------------------------------------------------------------------------------------------------------------------*/
uint8 calculate_threshold(uint8 *image,uint8 H, uint8 W){
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
		static uint8 last_Threshold;
    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j <H; j++)
    {
        for (i = 0; i < W; i++)
        {
           HistoGram[image[j * W + i]]++;//统计灰度级中每个像素在整幅图像中的个数
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
		//
		if(abs(last_Threshold-Threshold)<=1){
			return last_Threshold;
		}
		last_Threshold=Threshold;
    return Threshold;     
}
uint8 otsuThreshold_less(uint8 *image, uint16 width, uint16 height)
{
    #define GrayScale 256
    int pixel_count[GrayScale] = {0};//每个灰度值所占像素个数
    float pixel_account[GrayScale] = {0};//每个灰度值所占总像素比例
    int i,j;   
    int pixel_sum = width * height;   //总像素点
    uint8 threshold = 0;
    uint8* pixel_data = image;  //指向像素数据的指针


    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixel_count[(int)pixel_data[i * width + j]]++;  //将像素值作为计数数组的下标

        }
    }
    float u = 0;  
    for (i = 0; i < GrayScale; i++)
    {
        pixel_account[i] = (float)pixel_count[i] / pixel_sum;   //计算每个像素在整幅图像中的比例  
        u += i * pixel_account[i];  //总平均灰度
    }

      
    float variance_max=0.0;  //最大类间方差
	float variance;
    float w0 = 0, avgValue  = 0;  //w0 前景比例 ，avgValue 前景平均灰度
    for(int i = 0; i < 256; i++)     //每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
    {  
        w0 += pixel_account[i];  //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
        avgValue  += i * pixel_account[i];        
        if (w0 <= 0 || w0 >= 1)
			continue;
        variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //类间方差   
        if(variance > variance_max) 
        {  
            variance_max = variance;  
            threshold = i;  
        }  
    } 

    return threshold;

}
void binarize_image(int threshold){		
     for(int i=0;i<MT9V03X_W*MT9V03X_H;i++){
         if(image_copy[i/MT9V03X_W][i%MT9V03X_W]>threshold){ //大于阈值
             image_twovalue[i/MT9V03X_W][i%MT9V03X_W]=WHITE;
     }else{
         image_twovalue[i/MT9V03X_W][i%MT9V03X_W]=BLACK; //小于阈值
     }
	 }
}

void Bin_Image_Filter(uint8 *image,uint16 H,uint16 W){
    for(int nr=1;nr<H-1;nr++){
        for(int nc=1;nc<W-1;nc++){
					int index=nr*W+nc;
					int sum=image[(nr+1)*W+nc]+image[(nr-1)*W+nc]+image[nr*W+(nc+1)]+image[nr*W+(nc-1)];
            if(image[index]==0 && sum>2){
                image[index]=1;
            }else if(image[index]==1 && sum<2){
                image[index]=0;
         }
			}
		}
}
void car_emergency_stop(void){
	uint8 nc,nr;
	uint8 black_pixel;
	for(nr=MT9V03X_H-1;nr<MT9V03X_H-5;nr--)
	{
		for(nc=1;nc<MT9V03X_W;nc++)
		{
			black_pixel++;
		}
	}	
	if(black_pixel>=4*MT9V03X_W*0.7)
	{
		start=0;
	}
}
/*------------------------------------边线提取----------------------------------------------------*/
uint8 start_left,start_right;
uint8 longest_line_col,longest_line_row,longest_line_len;//白线定位和定长
void Get_whitest_columns(void){
	uint8 nc,nr;
	longest_line_col=MT9V03X_W/2;
	longest_line_row=MT9V03X_H-2;
	longest_line_len=0;
	for(nc=MT9V03X_W/2; nc>1;nc--)
	{
	if(image_twovalue[MT9V03X_H - 2][nc]==WHITE&&image_twovalue[MT9V03X_H - 2][nc-1]!= WHITE&&image_twovalue[MT9V03X_H - 2][nc-2]!= WHITE||nc==2)
	{start_left=nc;break;}
	}
	for(nc=MT9V03X_W/2; nc<MT9V03X_W-2;nc++)
	{	
		if(image_twovalue[MT9V03X_H - 2][nc]==WHITE&&image_twovalue[MT9V03X_H - 2][nc+1]!= WHITE&&image_twovalue[MT9V03X_H - 2][nc+2]!= WHITE||nc==186)
		{start_right=nc;break;}
	}
	if (start_left >= start_right)
	{
		// 设置安全默认值或添加错误处理
		longest_line_col = MT9V03X_W / 2;
		longest_line_row= 60;
		return;
	}
	for(nc=20;nc<start_right;nc++){
		uint8 tmp_row=0,tmp_maxlen=0;
		//最底下（MT9V03X_H-1）往上搜线
		for(nr=MT9V03X_H-1;nr>0;nr--){
			if(image_twovalue[nr][nc]==WHITE&&
				image_twovalue[nr-1][nc]==BLACK){
					tmp_row=nr;
					tmp_maxlen=MT9V03X_H-tmp_row;
					break;
				}
		}
		//简单边界处理
		if(nr==0&&
			image_twovalue[0][nc]==WHITE)
		{tmp_row=2;tmp_maxlen=MT9V03X_H - 2;}
		if(tmp_maxlen>longest_line_len)
		{
			longest_line_len=tmp_maxlen;
			longest_line_col=nc;
			longest_line_row=tmp_row;//高度定位，y值
		}
	}
}
uint8 left_sideline[120]={0},right_sideline[120]={0};

uint8 left_breakpoints[6][2]={{0}},right_breakpoints[6][2]={{0}};
uint8 left_breakpoint_count,right_breakpoint_count;
uint8 left_breakpoint_detected,right_breakpoint_detected;
uint8 left_lost,right_lost;

void Find_Boundry(void){
	uint8 nc,nr;
//	memset(left_sideline,0,sizeof(left_sideline));
//	memset(right_sideline,0,sizeof(right_sideline));
	for(nr=MT9V03X_H - 1;nr>longest_line_row;nr--)
	{
		for(nc=longest_line_col;nc>1;nc--){
			if((image_twovalue[nr][nc]==WHITE&&image_twovalue[nr][nc-1]==BLACK&&image_twovalue[nr][nc-2]==BLACK)||nc==2)
			{
				left_sideline[nr]=nc;
				if(left_sideline[nr]<=2)left_lost++;	//搜到了最左边
				break;//该行的点找到，下一行
			}
		}
		for(nc=longest_line_col;nc<MT9V03X_W - 2;nc++){
			if((image_twovalue[nr][nc]==WHITE&&image_twovalue[nr][nc+1]==BLACK&&image_twovalue[nr][nc+2]==BLACK)||nc==185)
			{
				right_sideline[nr]=nc;
				if(right_sideline[nr]==185)right_lost++;//搜到了最右边
				break;//该行的点找到，下一行
			}
		}
	}
}
/*-------------------------------------找四个点------------------------------------------*/















/*-------------------------------------补线，画线------------------------------------------*/
void Img_draw(void){
	ips200_show_gray_image((3-1)*offsetx,(10-1)*offsety, (const uint8 *)image_twovalue, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H,0);
	//画的所有的点和线都要偏移
	for(uint8 nr=MT9V03X_H-1;nr>0;nr--){
		ips200_draw_point(longest_line_col, nr+(10-1)*offsety,RGB565_YELLOW);
	}
	for(uint8 nr=MT9V03X_H-1;nr>longest_line_row;nr--){
		ips200_draw_point(left_sideline[nr]+(3-1)*offsetx, nr+(10-1)*offsety,RGB565_GREEN);
		ips200_draw_point(right_sideline[nr]+(3-1)*offsetx, nr+(10-1)*offsety,RGB565_BLUE);
		ips200_draw_point((left_sideline[nr]+right_sideline[nr])/2+(3-1)*offsetx, nr+(10-1)*offsety,RGB565_RED);
	}
	
//	for(uint8 nr=0;nr< MT9V03X_H;nr++){
//		ips200_draw_point(longest_line_col, nr+(10-1)*offsety,RGB565_YELLOW);
//	}
//	for(uint8 nr=longest_line_row;nr<MT9V03X_H-1;nr++){
//		ips200_draw_point(left_sideline[nr]+(2-1)*offsetx, nr+(10-1)*offsety,RGB565_GREEN);
//		ips200_draw_point(right_sideline[nr]+(2-1)*offsetx, nr+(10-1)*offsety,RGB565_BLUE);
//		ips200_draw_point((left_sideline[nr]+right_sideline[nr])/2+(2-1)*offsetx, nr+(10-1)*offsety,RGB565_RED);
//	}
	Img_draw_clear();
}
void Img_draw_clear(void){

}
void Img_handle(void){
    memcpy(image_copy, mt9v03x_image, MT9V03X_H*MT9V03X_W);
    imginformation.threshold=otsuThreshold_less(*image_copy,MT9V03X_H,MT9V03X_W);
    binarize_image(imginformation.threshold);
    Bin_Image_Filter(*image_twovalue,MT9V03X_H,MT9V03X_W);
    Get_whitest_columns();
		Find_Boundry();
		Img_draw();
}

// // 跟踪边线模块结构体 + 主函数封装
// #include <stdint.h>
// #include <string.h>

// #define MT9V03X_W 188
// #define MT9V03X_H 120
// #define IMG_BLACK 0x00
// #define IMG_WHITE 0xFF

