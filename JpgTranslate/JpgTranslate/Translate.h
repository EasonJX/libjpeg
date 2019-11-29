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
JPGͼƬת��ΪBMPͼƬ
@jpg_path:jpgͼƬ·��
@bmp_path:bmpͼƬ·��
�ɹ�����true ���򷵻�false
*/
bool jpg_to_bmp(const char* jpg_path,const char* bmp_path);





