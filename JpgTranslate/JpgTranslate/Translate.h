#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jpeglib.h"
#pragma comment(lib,"libjpeg.lib")

#pragma pack(2)
typedef struct bmp_file_
{
	unsigned short		bfType;
	unsigned long		bfSize;
	unsigned short		bfReserved1;
	unsigned short		bfReserved2;
	unsigned long		bfOffBits;
} bmp_file, pbmp_file;

typedef struct bmp_info_
{
	unsigned long		biSize;
	unsigned long       biWidth;
	unsigned long       biHeight;
	unsigned short      biPlanes;
	unsigned short      biBitCount;
	unsigned long       biCompression;
	unsigned long       biSizeImage;
	unsigned long       biXPelsPerMeter;
	unsigned long       biYPelsPerMeter;
	unsigned long       biClrUsed;
	unsigned long       biClrImportant;
} bmp_info, *pbmp_info;

/*
JPG图片转化为BMP图片
@jpg_path:jpg图片路径
@bmp_path:bmp图片路径
成功返回true 否则返回false
*/
bool jpg_to_bmp(const char* jpg_path,const char* bmp_path);





