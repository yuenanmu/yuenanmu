#include "zf_common_headfile.h"
// MT9V03X_W               ( 188 )                                         // 图像宽度     范围 [1-752]
// MT9V03X_H               ( 120 ) 
// mt9v03x_image[MT9V03X_H][MT9V03X_W];120*188=22680
//二值化（大津法阈值获取）
uint8 huidu[256]; //灰度直方图，个数统计
uint8 YUZHI=0;
void Dajinfa(){
    for(int i=0;i<22680;i++){
    huidu[mt9v03x_image[i/MT9V03X_W][i%MT9V03X_W]]++ ;//统计灰度直方图
    }
    uint16 H1=0;//第一次高峰值
    uint16 D1=0;//第一次高峰值
    for(int i=0;i<256;i++){     //遍历灰度直方图
        if(huidu[i]>H1){        //寻找最高峰
            H1=huidu[i];        
            D1=i;
        }
    }
    #define KUANDU 20 //宽度
    uint16 H2=0;//第二次高峰值
    uint16 D2=0;//第二次高峰索引
    char OK=0;
            //????
    uint16 H3=0;//第三次高峰值
    uint16 D3=0;//第三次高峰索引    
    if(OK){
        if(D1<D2){
            if(D1<D2){
                for(int i=D1;i<D2;i++){
                    if(huidu[i]<H3){    //寻找低谷       
                        H3=huidu[i];
                        D3=i;
                    }
                }
            }
        }
    }

    YUZHI=D3;
}

void binaryzation(){
    for(int i=0;i<MT9V03X_W*MT9V03X_H;i++){
        if(mt9v03x_image[i/MT9V03X_W][i%MT9V03X_W]>YUZHI){ //大于阈值
            mt9v03x_image[i/MT9V03X_W][i%MT9V03X_W]=0; //黑色
    }else{
        mt9v03x_image[i/MT9V03X_W][i%MT9V03X_W]=1; //小于阈值]
    }
	}
}

//边缘提取
/***********中线*****************/
char center_line[MT9V03X_H][MT9V03X_W]; //中心线数组
void extract_center_line() {
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
/***********图像腐蚀*****************/
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

//特征识别



/***********元素处理*****************/
//补线
char ax,ay,bx,by,cx,cy,dx,dy; //补线起点和终点
void cross(){
    for(int i=0;i<MT9V03X_H;i++){
        for(int j=0;j<MT9V03X_W;j++){
            if(mt9v03x_image[i][j]==1){ //如果是白色
                if(mt9v03x_image[i+1][j]==0 && mt9v03x_image[i-1][j]==0){ //上下都是黑色
                    ax=j; ay=i; //记录起点
                    while(mt9v03x_image[i][j+1]==1){ //向右走
                        j++;
                    }
                    bx=j; by=i; //记录终点
                    //补线
                    for(int k=ax;k<=bx;k++){
                        mt9v03x_image[ay][k]=1;
                    }
                }
            }
        }
    }
}
void all_module_prodeal(){
    
}


//中心寻线



//中心寻找



//偏差曲率计算



//待优化：记录时间，图像切片，函数优化（结构体？）