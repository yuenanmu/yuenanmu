#include "zf_common_headfile.h"
#include "img_handle.h"
#define IMG_BLACK 0x00
#define IMG_WHITE 0xff 
#define Prediction_Confidence  0.55
#define MID_W 87
//
ds_Track_Boundary Track;
// MT9V03X_W               ( 188 )     
// MT9V03X_H               ( 120 ) 
uint8 image_two_value[MT9V03X_H][MT9V03X_W];//二值化后的原数组
volatile int Left_Line[MT9V03X_H]; //左边线数组
volatile int Right_Line[MT9V03X_H];//右边线数组
volatile int Mid_Line[MT9V03X_H];  //中线数组
volatile int Road_Wide[MT9V03X_H]; //赛宽数组
//volatile int White_Column[MT9V03X_W];    //每列白列长度
volatile int Search_Stop_Line;     //搜索截止行,只记录长度，想要坐标需要用视野高度减去该值
volatile int Boundry_Start_Left;   //左右边界起始点
volatile int Boundry_Start_Right;  //第一个非丢线点,常规边界起始点
volatile int Left_Lost_Time;       //边界丢线数
volatile int Right_Lost_Time;
volatile int Both_Lost_Time;//两边同时丢线数
int Longest_White_Column_Left[2]; //最长白列,[0]是最长白列的长度，也就是Search_Stop_Line搜索截止行，[1】是第某列
int Longest_White_Column_Right[2];//最长白列,[0]是最长白列的长度，也就是Search_Stop_Line搜索截止行，[1】是第某列
int Left_Lost_Flag[MT9V03X_H] ; //左丢线数组，丢线置1，没丢线置0
int Right_Lost_Flag[MT9V03X_H]; //右丢线数组，丢线置1，没丢线置0

float final_mid_line = MID_W;   // 最终输出的中线值
float last_mid_line = MID_W;    // 上次中线值
//---元素类
uint8 threshold;
//环岛
 volatile int Island_State;     //环岛状态标志
 volatile int Left_Island_Flag; //左右环岛标志
 volatile int Right_Island_Flag;//左右环岛标志

//坡道
volatile int Ramp_Flag=0;//坡道标志

//十字
volatile int Cross_Flag=0;
uint8 Zebra_Flag;
uint8 Zebra_Count=0;
volatile int Left_Down_Find=0; //十字使用，找到被置行数，没找到就是0
volatile int Left_Up_Find=0;   //四个拐点标志
volatile int Right_Down_Find=0;
volatile int Right_Up_Find=0;

//斑马线
volatile int Zebra_Stripes_Flag=0;//斑马线

//控制标
volatile uint8 Obstacle_Dir=0; //0右拐，1左拐
volatile uint8 Island_Switch=1;//环岛识别开启标志位
volatile uint8 Straight_Flag=0;//长直道识别标
volatile uint8 Ramp_Switch=0;  //坡道识别标志位
//const uint8 Weight[MT9V03X_H]=
//{
//        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端00 ——09 行权重
//        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端10 ——19 行权重
//        1, 1, 1, 1, 1, 1, 1, 3, 4, 5,              //图像最远端20 ——29 行权重
//        6, 7, 9,11,13,15,17,19,20,20,              //图像最远端30 ——39 行权重
//       19,17,15,13,11, 9, 7, 5, 3, 1,              //图像最远端40 ——49 行权重
//        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端50 ——59 行权重
//        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端60 ——69 行权重
//};
const uint8 Weight[MT9V03X_H]=
{
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端00 ——09 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端10 ——19 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端20 ——29 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端30 ——39 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端40 ——49 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端50 ——59 行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              //图像最远端60 ——69 行权重
};
uint8 mid_weight_list[120] = 
{
    0,0,0,0,0,0,0,0,0,0,        //十个
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,6,6,
    6,6,6,6,8,8,8,9,9,9,
    9,10,10,10,11,12,13,14,15,16,
    17,18,20,24,20,20,19,19,18,17,
    16,15,14,13,12,11,10,9,8,7,
    6,6,6,6,6,6,6,6,6,6,
    6,5,4,3,2,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
};
/*----------------------------------------------图像处理部分------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------
  @brief     计算阈值
  @param     起始行，终止行
  @return    返回角点所在的行数，找不到返回0
  Sample     calculate_threshold(*image_copy,MT9V03X_H,MT9V03X_W);
  @note      用ips200_show_gray_image();函数显示图像
-------------------------------------------------------------------------------------------------------------------*/
int my_adapt_threshold(uint8 *image, uint16 col, uint16 row)   //注意计算阈值的一定要是原图像
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j;
    int pixelSum = width * height/4;
    int threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }
    //计算每个像素值的点在整幅图像中的比例
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }
    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值
        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;
        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }
    }
    return threshold;
}
void Image_Binarization(int threshold)//图像二值化
{
    uint16 i,j;
    for(i=0;i<MT9V03X_H;i++)
    {
        for(j=0;j<MT9V03X_W;j++)
        {
            if(mt9v03x_image[i][j]>=threshold)
                image_two_value[i][j]=IMG_WHITE;//白
            else
                image_two_value[i][j]=IMG_BLACK;//黑
        }
    }
}
/*------------------------------------边线提取----------------------------------------------------*/
void Longest_White_Column()//最长白列巡线
{
    int i, j;
    int start_column=20;//最长白列的搜索区间
    int end_column=MT9V03X_W-20;
    int left_border = 0, right_border = 0;//临时存储赛道位置
	  uint8	White_Column[MT9V03X_W]={0};
    Longest_White_Column_Left[0] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Left[1] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Right[0] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Right[1] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Right_Lost_Time = 0;    //边界丢线数
    Left_Lost_Time  = 0;
    Boundry_Start_Left  = 0;//第一个非丢线点,常规边界起始点
    Boundry_Start_Right = 0;
    Both_Lost_Time = 0;//两边同时丢线数

//环岛需要对最长白列范围进行限定，环岛3状态找不到上角点，可以修改下述参数
    //环岛3状态需要改变最长白列寻找范围
    if(Right_Island_Flag==1)//右环
    {
        if(Island_State==3)
        {
            start_column=40;
            end_column=MT9V03X_W-20;
        }
    }
    else if(Left_Island_Flag==1)//左环
    {
        if(Island_State==3)
        {
            start_column=20;
            end_column=MT9V03X_W-40;
        }
    }

    //从左到右，从下往上，遍历全图记录范围内的每一列白点数量
    for (j =start_column; j<=end_column; j++)
    {
        for (i = MT9V03X_H - 1; i >= 0; i--)
        {
            if(image_two_value[i][j] == IMG_BLACK)
                break;
            else
                White_Column[j]++;
        }
    }

    //从左到右找左边最长白列
    Longest_White_Column_Left[0] =0;
    for(i=start_column;i<=end_column;i++)
    {
        if (Longest_White_Column_Left[0] < White_Column[i])//找最长的那一列
        {
            Longest_White_Column_Left[0] = White_Column[i];//【0】是白列长度
            Longest_White_Column_Left[1] = i;              //【1】是下标，第j列
        }
    }
    //从右到左找右左边最长白列
    Longest_White_Column_Right[0] = 0;//【0】是白列长度
    for(i=end_column;i>=start_column;i--)//从右往左，注意条件，找到左边最长白列位置就可以停了
    {
        if (Longest_White_Column_Right[0] < White_Column[i])//找最长的那一列
        {
            Longest_White_Column_Right[0] = White_Column[i];//【0】是白列长度
            Longest_White_Column_Right[1] = i;              //【1】是下标，第j列
        }
    }

    Search_Stop_Line = Longest_White_Column_Left[0];//搜索截止行选取左或者右区别不大，他们两个理论上是一样的
    for (i = MT9V03X_H - 1; i >=MT9V03X_H-Search_Stop_Line; i--)//常规巡线
    {
        for (j = Longest_White_Column_Right[1]; j <= MT9V03X_W - 1 - 2; j++)
        {
            if (image_two_value[i][j] ==IMG_WHITE && image_two_value[i][j + 1] == IMG_BLACK && image_two_value[i][j + 2] == IMG_BLACK)//白黑黑，找到右边界
            {
                right_border = j;
								//BB();
                Right_Lost_Flag[i] = 0; //右丢线数组，丢线置1，不丢线置0
                break;
            }
            else if(j>=MT9V03X_W-1-2)//没找到右边界，把屏幕最右赋值给右边界
            {
                right_border = j;
							  //BB();
                Right_Lost_Flag[i] = 1; //右丢线数组，丢线置1，不丢线置0
                break;
            }
        }
        for (j = Longest_White_Column_Left[1]; j >= 0 + 2; j--)//往左边扫描
        {
            if (image_two_value[i][j] ==IMG_WHITE && image_two_value[i][j - 1] == IMG_BLACK && image_two_value[i][j - 2] == IMG_BLACK)//黑黑白认为到达左边界
            {
                left_border = j;
								//BB();
                Left_Lost_Flag[i] = 0; //左丢线数组，丢线置1，不丢线置0
                break;
            }
            else if(j<=0+2)
            {
                left_border = j;//找到头都没找到边，就把屏幕最左右当做边界
                Left_Lost_Flag[i] = 1; //左丢线数组，丢线置1，不丢线置0
                break;
            }
        }
        Left_Line [i] = left_border;       //左边线线数组
        Right_Line[i] = right_border;      //右边线线数组
    }

    for (i = MT9V03X_H - 1; i >= 0; i--)//赛道数据初步分析
    {
        if (Left_Lost_Flag[i]  == 1)//单边丢线数
            Left_Lost_Time++;
        if (Right_Lost_Flag[i] == 1)
            Right_Lost_Time++;
        if (Left_Lost_Flag[i] == 1 && Right_Lost_Flag[i] == 1)//双边丢线数
            Both_Lost_Time++;
        if (Boundry_Start_Left ==  0 && Left_Lost_Flag[i]  != 1)//记录第一个非丢线点，边界起始点
            Boundry_Start_Left = i;
        if (Boundry_Start_Right == 0 && Right_Lost_Flag[i] != 1)
            Boundry_Start_Right = i;
        Road_Wide[i]=Right_Line[i]-Left_Line[i];
    }
}
void Show_Boundry(void)
{
    int16 i;
    for(i=MT9V03X_H-1;i>=MT9V03X_H-Search_Stop_Line;i--)//从最底下往上扫描
    {
        image_two_value[i][Left_Line[i]+1]=IMG_BLACK;
        image_two_value[i][(Left_Line[i]+Right_Line[i])>>1]=IMG_BLACK;
        image_two_value[i][Right_Line[i]-1]=IMG_BLACK;
				//BB();
    }

    //在屏幕理论中线处显示红线，用于调整摄像头
    //ips200_draw_line ( MT9V03X_W/2, MT9V03X_H-10, MT9V03X_W/2, MT9V03X_H, RGB565_RED);
}
/*-------------------------------------找拐点------------------------------------------*/
void Find_Down_Point(int start,int end)
{
    int i,t;
    Right_Down_Find=0;
    Left_Down_Find=0;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_H-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=MT9V03X_H-1-5;
    if(end<=MT9V03X_H-Search_Stop_Line)
        end=MT9V03X_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(Left_Down_Find==0&&//只找第一个符合条件的点
           abs(Left_Line[i]-Left_Line[i+1])<=5&&//角点的阈值可以更改
           abs(Left_Line[i+1]-Left_Line[i+2])<=5&&
           abs(Left_Line[i+2]-Left_Line[i+3])<=5&&
              (Left_Line[i]-Left_Line[i-2])>=8&&
              (Left_Line[i]-Left_Line[i-3])>=15&&
              (Left_Line[i]-Left_Line[i-4])>=15)
        {
            Left_Down_Find=i;//获取行数即可
        }
        if(Right_Down_Find==0&&//只找第一个符合条件的点
           abs(Right_Line[i]-Right_Line[i+1])<=5&&//角点的阈值可以更改
           abs(Right_Line[i+1]-Right_Line[i+2])<=5&&
           abs(Right_Line[i+2]-Right_Line[i+3])<=5&&
              (Right_Line[i]-Right_Line[i-2])<=-8&&
              (Right_Line[i]-Right_Line[i-3])<=-15&&
              (Right_Line[i]-Right_Line[i-4])<=-15)
        {
            Right_Down_Find=i;
        }
        if(Left_Down_Find!=0&&Right_Down_Find!=0)//两个找到就退出
        {
            break;
        }
    }
}
void Find_Up_Point(int start,int end)
{
    int i,t;
    Left_Up_Find=0;
    Right_Up_Find=0;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(end<=MT9V03X_H-Search_Stop_Line)
        end=MT9V03X_H-Search_Stop_Line;
    if(end<=5)//及时最长白列非常长，也要舍弃部分点，防止数组越界
        end=5;
    if(start>=MT9V03X_H-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=MT9V03X_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(Left_Up_Find==0&&//只找第一个符合条件的点
           abs(Left_Line[i]-Left_Line[i-1])<=5&&
           abs(Left_Line[i-1]-Left_Line[i-2])<=5&&
           abs(Left_Line[i-2]-Left_Line[i-3])<=5&&
              (Left_Line[i]-Left_Line[i+2])>=8&&
              (Left_Line[i]-Left_Line[i+3])>=15&&
              (Left_Line[i]-Left_Line[i+4])>=15)
        {
            Left_Up_Find=i;//获取行数即可
        }
        if(Right_Up_Find==0&&//只找第一个符合条件的点
           abs(Right_Line[i]-Right_Line[i-1])<=5&&//下面两行位置差不多
           abs(Right_Line[i-1]-Right_Line[i-2])<=5&&
           abs(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-8&&
              (Right_Line[i]-Right_Line[i+3])<=-15&&
              (Right_Line[i]-Right_Line[i+4])<=-15)
        {
            Right_Up_Find=i;//获取行数即可
        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)//下面两个找到就出去
        {
            break;
        }
    }
    if(abs(Right_Up_Find-Left_Up_Find)>=30)//纵向撕裂过大，视为误判
    {
        Right_Up_Find=0;
        Left_Up_Find=0;
    }
}
/*-------------------------------------补线------------------------------------------*/
void Left_Add_Line(int x1,int y1,int x2,int y2)//左补线,补的是边界
{
    int i,max,a1,a2;
    int hx;
    if(x1>=MT9V03X_W-1)//起始点位置校正，排除数组越界的可能
       x1=MT9V03X_W-1;
    else if(x1<=0)
        x1=0;
     if(y1>=MT9V03X_H-1)
        y1=MT9V03X_H-1;
     else if(y1<=0)
        y1=0;
     if(x2>=MT9V03X_W-1)
        x2=MT9V03X_W-1;
     else if(x2<=0)
             x2=0;
     if(y2>=MT9V03X_H-1)
        y2=MT9V03X_H-1;
     else if(y2<=0)
             y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//坐标互换
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    for(i=a1;i<=a2;i++)//根据斜率补线即可
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=MT9V03X_W)
            hx=MT9V03X_W;
        else if(hx<=0)
            hx=0;
        Left_Line[i]=hx;
    }
}
void Right_Add_Line(int x1,int y1,int x2,int y2)//右补线,补的是边界
{
    int i,max,a1,a2;
    int hx;
    if(x1>=MT9V03X_W-1)//起始点位置校正，排除数组越界的可能
       x1=MT9V03X_W-1;
    else if(x1<=0)
        x1=0;
    if(y1>=MT9V03X_H-1)
        y1=MT9V03X_H-1;
    else if(y1<=0)
        y1=0;
    if(x2>=MT9V03X_W-1)
        x2=MT9V03X_W-1;
    else if(x2<=0)
        x2=0;
    if(y2>=MT9V03X_H-1)
        y2=MT9V03X_H-1;
    else if(y2<=0)
         y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//坐标互换
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    for(i=a1;i<=a2;i++)//根据斜率补线即可
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=MT9V03X_W)
            hx=MT9V03X_W;
        else if(hx<=0)
            hx=0;
        Right_Line[i]=hx;
    }
}
void Lengthen_Left_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=MT9V03X_H-1)//起始点位置校正，排除数组越界的可能
        start=MT9V03X_H-1;
    else if(start<=0)
        start=0;
    if(end>=MT9V03X_H-1)
        end=MT9V03X_H-1;
    else if(end<=0)
        end=0;
    if(end<start)//++访问，坐标互换
    {
        t=end;
        end=start;
        start=t;
    }

    if(start<=5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
         Left_Add_Line(Left_Line[start],start,Left_Line[end],end);
    }

    else
    {
        k=(float)(Left_Line[start]-Left_Line[start-4])/5.0;//这里的k是1/斜率
        for(i=start;i<=end;i++)
        {
            Left_Line[i]=(int)(i-start)*k+Left_Line[start];//(x=(y-y1)*k+x1),点斜式变形
            if(Left_Line[i]>=MT9V03X_W-1)
            {
                Left_Line[i]=MT9V03X_W-1;
            }
            else if(Left_Line[i]<=0)
            {
                Left_Line[i]=0;
            }
        }
    }
}
void Lengthen_Right_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=MT9V03X_H-1)//起始点位置校正，排除数组越界的可能
        start=MT9V03X_H-1;
    else if(start<=0)
        start=0;
    if(end>=MT9V03X_H-1)
        end=MT9V03X_H-1;
    else if(end<=0)
        end=0;
    if(end<start)//++访问，坐标互换
    {
        t=end;
        end=start;
        start=t;
    }

    if(start<=5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Right_Add_Line(Right_Line[start],start,Right_Line[end],end);
    }
    else
    {
        k=(float)(Right_Line[start]-Right_Line[start-4])/5.0;//这里的k是1/斜率
        for(i=start;i<=end;i++)
        {
            Right_Line[i]=(int)(i-start)*k+Right_Line[start];//(x=(y-y1)*k+x1),点斜式变形
            if(Right_Line[i]>=MT9V03X_W-1)
            {
                Right_Line[i]=MT9V03X_W-1;
            }
            else if(Right_Line[i]<=0)
            {
                Right_Line[i]=0;
            }
        }
    }
}
void Draw_Line(int startX, int startY, int endX, int endY)
{
    int i,x,y;
    int start=0,end=0;
    if(startX>=MT9V03X_W-1)//限幅处理
        startX=MT9V03X_W-1;
    else if(startX<=0)
        startX=0;
    if(startY>=MT9V03X_H-1)
        startY=MT9V03X_H-1;
    else if(startY<=0)
        startY=0;
    if(endX>=MT9V03X_W-1)
        endX=MT9V03X_W-1;
    else if(endX<=0)
        endX=0;
    if(endY>=MT9V03X_H-1)
        endY=MT9V03X_H-1;
    else if(endY<=0)
        endY=0;
    if(startX==endX)//一条竖线
    {
        if (startY > endY)//互换
        {
            start=endY;
            end=startY;
        }
        for (i = start; i <= end; i++)
        {
            if(i<=1)
                i=1;
            image_two_value[i][startX]=IMG_BLACK;
            image_two_value[i-1][startX]=IMG_BLACK;
        }
    }
    else if(startY == endY)//补一条横线
    {
        if (startX > endX)//互换
        {
            start=endX;
            end=startX;
        }
        for (i = start; i <= end; i++)
        {
            if(startY<=1)
                startY=1;
            image_two_value[startY][i]=IMG_BLACK;
            image_two_value[startY-1][i]=IMG_BLACK;
        }
    }
    else //上面两个是水平，竖直特殊情况，下面是常见情况
    {
        if(startY>endY)//起始点矫正
        {
            start=endY;
            end=startY;
        }
        else
        {
            start=startY;
            end=endY;
        }
        for (i = start; i <= end; i++)//纵向补线，保证每一行都有黑点
        {
            x =(int)(startX+(endX-startX)*(i-startY)/(endY-startY));//两点式变形
            if(x>=MT9V03X_W-1)
                x=MT9V03X_W-1;
            else if (x<=1)
                x=1;
            image_two_value[i][x] = IMG_BLACK;
            image_two_value[i][x-1] = IMG_BLACK;
        }
        if(startX>endX)
        {
            start=endX;
            end=startX;
        }
        else
        {
            start=startX;
            end=endX;
        }
        for (i = start; i <= end; i++)//横向补线，保证每一列都有黑点
        {

            y =(int)(startY+(endY-startY)*(i-startX)/(endX-startX));//两点式变形
            if(y>=MT9V03X_H-1)
                y=MT9V03X_H-1;
            else if (y<=0)
                y=0;
            image_two_value[y][i] = IMG_BLACK;
        }
    }
}
/*------------------------------------特征识别----------------------*/
void Cross_Detect()
{
		//BB();
    int down_search_start=0;//下点搜索开始行
    Cross_Flag=0;
    if(Island_State==0&&Ramp_Flag==0)//与环岛互斥开
    {
        Left_Up_Find=0;
        Right_Up_Find=0;
        if(Both_Lost_Time>=10)//十字必定有双边丢线，在有双边丢线的情况下再开始找角点
        {
            Find_Up_Point( MT9V03X_H-1, 0 );
            if(Left_Up_Find==0&&Right_Up_Find==0)//只要没有同时找到两个上点，直接结束
            {
                return;
            }
        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)//找到两个上点，就找到十字了
        {
            Cross_Flag=1;//对应标志位，便于各元素互斥掉
            down_search_start=Left_Up_Find>Right_Up_Find?Left_Up_Find:Right_Up_Find;//用两个上拐点坐标靠下者作为下点的搜索上限
            Find_Down_Point(MT9V03X_H-5,down_search_start+2);//在上拐点下2行作为下点的截止行
            if(Left_Down_Find<=Left_Up_Find)
            {
                Left_Down_Find=0;//下点不可能比上点还靠上
            }
            if(Right_Down_Find<=Right_Up_Find)
            {
                Right_Down_Find=0;//下点不可能比上点还靠上
            }
            if(Left_Down_Find!=0&&Right_Down_Find!=0)
            {//四个点都在，无脑连线，这种情况显然很少
                Left_Add_Line (Left_Line [Left_Up_Find ],Left_Up_Find ,Left_Line [Left_Down_Find ] ,Left_Down_Find);
                Right_Add_Line(Right_Line[Right_Up_Find],Right_Up_Find,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else if(Left_Down_Find==0&&Right_Down_Find!=0)//11//这里使用的都是斜率补线
            {//三个点                                     //01
                Lengthen_Left_Boundry(Left_Up_Find-1,MT9V03X_H-1);
                Right_Add_Line(Right_Line[Right_Up_Find],Right_Up_Find,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else if(Left_Down_Find!=0&&Right_Down_Find==0)//11
            {//三个点                                     //10
                Left_Add_Line (Left_Line [Left_Up_Find ],Left_Up_Find ,Left_Line [Left_Down_Find ] ,Left_Down_Find);
                Lengthen_Right_Boundry(Right_Up_Find-1,MT9V03X_H-1);
            }
            else if(Left_Down_Find==0&&Right_Down_Find==0)//11
            {//就俩上点                                   //00
                Lengthen_Left_Boundry (Left_Up_Find-1,MT9V03X_H-1);
                Lengthen_Right_Boundry(Right_Up_Find-1,MT9V03X_H-1);
            }
        }
        else
        {
            Cross_Flag=0;
        }
    }
    //角点相关变量，debug使用
    //ips200_showuint8(0,12,Cross_Flag);
//    ips200_showuint8(0,13,Island_State);
//    ips200_showuint8(50,12,Left_Up_Find);
//    ips200_showuint8(100,12,Right_Up_Find);
//    ips200_showuint8(50,13,Left_Down_Find);
//    ips200_showuint8(100,13,Right_Down_Find);
}
uint8  Zebra_Detected(void)
{
    uint8 zebra_count=0;
    Zebra_Flag=0;//斑马线标志清零
    if(Longest_White_Column_Left[1]>20&&Longest_White_Column_Right[1]<MT9V03X_W-20&&
        Longest_White_Column_Right[1]>20&&Longest_White_Column_Left[1]<MT9V03X_W-20&&
        Search_Stop_Line>=110&&
    Boundry_Start_Left>=MT9V03X_H-20&&
    Boundry_Start_Right>=MT9V03X_H-20)
        {
            for(int i=MT9V03X_H-1;i>=MT9V03X_H-3;i--)
            {
                for(int j=0;j<=MT9V03X_W-1-3;j++)
                {
                    if(image_two_value[i][j]==IMG_WHITE&&image_two_value[i][j+1]==0&&image_two_value[i][j+2]==0)
                    {
                        zebra_count++;
                    }
									}
                if(zebra_count>=10)
                {
                    Zebra_Flag=1;
                        //BB();
                        return Zebra_Flag ;
                }
            }
        }
				return 0;
}
/*-------------------------------------图像处理------------------------------------------*/
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
	if(black_pixel>=4*MT9V03X_W*0.7||Track.Err>150)
	{
		start_go=0;
		Motor_Control_L(0);
		Motor_Control_R(0);
	}
}
void Bin_Image_Filter(uint8 *image,uint16 H,uint16 W){
    for(int nr=1;nr<H-1;nr++){
        for(int nc=1;nc<W-1;nc++){
					int index=nr*W+nc;
					int sum=image[(nr+1)*W+nc]+image[(nr-1)*W+nc]+image[nr*W+(nc+1)]+image[nr*W+(nc-1)];
            if(image[index]==IMG_BLACK && sum>2*IMG_WHITE){
                image[index]=IMG_WHITE;
            }else if(image[index]==IMG_WHITE && sum<2*IMG_WHITE){
                image[index]=IMG_BLACK;
         }
			}
		}
}
uint8 foresight_line=25;
float Get_Err1(void)        //常规误差计算&&前瞻范围画线
{
	float Err1=0,Err2=0,Err=0;
	float weight_count=0;
	//常规误差
	for(int i=MT9V03X_H-foresight_line;i>=MT9V03X_H-foresight_line-20;i--)//常规误差计算
	{
		if(i==MT9V03X_H-foresight_line||i==MT9V03X_H-foresight_line-20){
			for(int j=Left_Line[i];j<Right_Line[i];j++)
			{
				ips200_draw_point(j+(2-1)*offsetx, i+(10-1)*offsety,RGB565_RED);
			}
		}
			Err1+=(MT9V03X_W/2-((Left_Line[i]+Right_Line[i])>>1));//右移1位，等效除2
	}
	// for(int i=Search_Stop_Line+5;i<=Search_Stop_Line+40;i++)//常规误差计算
	// {
	// 	if(i==Search_Stop_Line+5||i==Search_Stop_Line+40){
	// 		for(int j=Left_Line[i];j<Right_Line[i];j++)
	// 		{
	// 			ips200_draw_point(j+(2-1)*offsetx, i+(10-1)*offsety,RGB565_RED);
	// 		}
	// 	}
	// 		Err2+=(MT9V03X_W/2-((Left_Line[i]+Right_Line[i])>>1));//右移1位，等效除2
	// }
	
	Err=Err1/20.0;//+Err2/35.0*(1-Prediction_Confidence)   *Prediction_Confidence
	return Err;
}
float Get_Err2(void)
{
    float err=0;
    uint8 weight_sum;
    for(int i=MT9V03X_H-4;i>MT9V03X_H-Search_Stop_Line+10;i--)
    {
        err+=(MT9V03X_W/2-((Left_Line[i]+Right_Line[i])>>1)*Weight[i]);//位操作等效除以2
        weight_sum+=Weight[i];
    }
    err=err/weight_sum;
    return err;
}

float find_mid_line_weight(void)
{
    float mid_line_value = MID_W;       // 最终中线输出值
    float mid_line = MID_W;             // 本次中线值
    float weight_midline_sum = 0;      // 加权中线累加值
    float weight_sum = 0;              // 权重累加值
		for(uint8 i = MT9V03X_H - 1; i > Search_Stop_Line; i--)
		{
			weight_midline_sum += Mid_Line[i] * mid_weight_list[i];
			weight_sum += mid_weight_list[i];
		}
    mid_line = (float)(weight_midline_sum / weight_sum);
    mid_line_value = last_mid_line * 0.2 + mid_line * 0.8; // 互补滤波
    last_mid_line = mid_line_value;
    return mid_line_value;
}
float Get_Err3(void)
{
    float err;
	uint8 mid_line_data=find_mid_line_weight();
    err=MT9V03X_W-mid_line_data;
    return err;
}
void Get_UseImg(void){
	memcpy(image_copy, mt9v03x_image, MT9V03X_H*MT9V03X_W);
	threshold=my_adapt_threshold(*image_copy,MT9V03X_W, MT9V03X_H);
	Image_Binarization(threshold);
	Bin_Image_Filter(image_two_value[0], MT9V03X_H,MT9V03X_W);
}
void Img_Processing(void){
	Get_UseImg();
	Longest_White_Column();
	
	Cross_Detect();
	
	if(Zebra_Detected()==1){
		BB();
	Zebra_Flag=0;
	Zebra_Count+=1;
	Zebra_Count=Zebra_Count%2+1;
	if(Zebra_Count==2){
		start_go=0;
		Motor_Control_L(0);
		Motor_Control_R(0);
		}
	}
	//Show_Boundry();
	Track.Err=Get_Err1();
}
void Img_draw(void){
	//画的所有的点和线都要偏移
	for(uint8 nr=MT9V03X_H-1;nr>MT9V03X_H-Longest_White_Column_Left[0];nr--){
		ips200_draw_point(Longest_White_Column_Left[1]+(2-1)*offsetx, nr+(10-1)*offsety,RGB565_YELLOW);
	}
	for(uint8 nr=MT9V03X_H-1;nr>MT9V03X_H-Longest_White_Column_Left[0];nr--){
		ips200_draw_point(Left_Line[nr]+(2-1)*offsetx, nr+(10-1)*offsety,RGB565_GREEN);
		ips200_draw_point(Right_Line[nr]+(2-1)*offsetx, nr+(10-1)*offsety,RGB565_BLUE);
		ips200_draw_point(((Right_Line[nr]+Left_Line[nr])>>1)+(2-1)*offsetx, nr+(10-1)*offsety,RGB565_BLACK);
	}
	for(uint8 nr=MT9V03X_H-1;nr>1;nr--){
		ips200_draw_point(MT9V03X_W/2+(2-1)*offsetx, nr+(10-1)*offsety,RGB565_RED);
	}
}
void Img_draw_clear(void){
	memset((void *)Left_Line,0,sizeof(Left_Line));
	memset((void *)Right_Line,MT9V03X_W-1,sizeof(Left_Line));
	memset((void *)Right_Lost_Flag,0,sizeof(Right_Lost_Flag));
	memset((void *)Left_Lost_Flag,0,sizeof(Left_Lost_Flag));
}