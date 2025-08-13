#include "img_process.h"
//uint8 Image_Use[LCDH][LCDW]; // 存储使用中的图像数据的数组，大小为LCDH行LCDW列
//uint8 Bin_Image[LCDH][LCDW]; // 存储二值化后的图像数据的数组，大小为LCDH行LCDW列
//uint8 LeftSide[LCDH]; // 存储左侧边缘的数组，长度为LCDH
//uint8 RightSide[LCDH]; // 存储右侧边缘的数组，长度为LCDH
//uint8 BinEro_Image[LCDH][LCDW]; // 存储经过腐蚀处理后的二值化图像数据的数组，大小为LCDH行LCDW列
//uint8 Erosion[LCDH][LCDW]; // 存储腐蚀后的图像数据的数组，大小为LCDH行LCDW列
//int CollectTim = 0; // 记录采集时间的整型变量，初始值为0
//uint16 ThresholdSobel; // Sobel算子阈值的无符号整型变量
//uint16 ThresholdOstu; // Otsu算法阈值的无符号整型变量
//uint16 ExposureTime; // 曝光时间的无符号整型变量
//uint16 ImageGain; // 图像增益的无符号整型变量
