#include "Translate.h"

bool jpg_to_bmp(const char* jpg_path, const char* bmp_path)
{
	//This struct contains the JPEG decompression parameters and pointers to working space
	struct jpeg_decompress_struct jpg_decompress;

	//We use our private extension JPEG error handler
	struct jpeg_error_mgr jpg_err;

	//Output row buffer
	JSAMPARRAY jpg_buffer;

	struct bmp_file_ st_bmp_file;
	struct bmp_info_ st_bmp_info;

	unsigned long width = 0;
	unsigned long height = 0;
	unsigned short depth = 0;

	unsigned char* data_buffer = 0;
	unsigned char* data_point = 0;

	int row_stride = 0;

	int headersize = 0;
	int sumsize = 0;

	unsigned char* platte = 0;

	unsigned char* line_buffer = 0;

	int i = 0, j = 0;

	if (!strlen(jpg_path) || !strlen(bmp_path))
	{
		printf("path fail\n");
		return false;
	}

	FILE* f_jpg = fopen(jpg_path, "rb+");
	if (!f_jpg)
	{
		printf("file fail\n");
		return false;
	}

	FILE* f_bmp = fopen(bmp_path, "wb+");
	if (!f_bmp)
	{
		printf("file fail\n");
		fclose(f_jpg);
		return false;
	}

	//We set up the normal JPEG error routines, then override error_exit.
	jpg_decompress.err = jpeg_std_error(&jpg_err);

	//Now we can initialize the JPEG decompression object.
	jpeg_create_decompress(&jpg_decompress);

	//specify data source
	jpeg_stdio_src(&jpg_decompress, f_jpg);

	//read file parameters
	(void)jpeg_read_header(&jpg_decompress, true);

	//Start decompressor
	(void)jpeg_start_decompress(&jpg_decompress);

	width = jpg_decompress.output_width;
	height = jpg_decompress.output_height;
	depth = jpg_decompress.output_components;

	data_buffer = (unsigned char*)malloc(width*height*depth);
	if (!data_buffer)
	{
		printf("allocate fail\n");
		jpeg_finish_decompress(&jpg_decompress);
		jpeg_destroy_decompress(&jpg_decompress);
		free(data_buffer);
		fclose(f_jpg);
		fclose(f_bmp);
		return false;
	}
	memset(data_buffer, 0, sizeof(unsigned char)*width*height*depth);

	//JSAMPLEs per row in output buffer
	row_stride = width * depth;

	//Make a one-row-high sample array that will go away when done with image
	jpg_buffer = (*jpg_decompress.mem->alloc_sarray)((j_common_ptr)&jpg_decompress, JPOOL_IMAGE, row_stride, 1);

	data_point = data_buffer;

	while (jpg_decompress.output_scanline < height)
	{
		jpeg_read_scanlines(&jpg_decompress, jpg_buffer, 1);
		memcpy(data_point, *jpg_buffer, row_stride);
		data_point += row_stride;
	}

	if (depth == 1)
	{
		headersize = 14 + 40 + 256 * 4;
		sumsize = headersize + width * height;
	}
	else if (depth == 3)
	{
		headersize = 14 + 40;
		sumsize = headersize + width * height*depth;
	}
	else
	{
		printf("depth fail\n");
		jpeg_finish_decompress(&jpg_decompress);
		jpeg_destroy_decompress(&jpg_decompress);
		free(data_buffer);
		fclose(f_jpg);
		fclose(f_bmp);
		return false;
	}

	memset(&st_bmp_file, 0, sizeof(st_bmp_file));
	memset(&st_bmp_info, 0, sizeof(st_bmp_info));

	st_bmp_file.bfType = 0x4d42;
	st_bmp_file.bfSize = sumsize;
	st_bmp_file.bfOffBits = headersize;

	st_bmp_info.biSize = 40;
	st_bmp_info.biWidth = width;
	st_bmp_info.biHeight = height;
	st_bmp_info.biPlanes = 1;
	st_bmp_info.biBitCount = depth * 8;
	st_bmp_info.biSizeImage = width * height*depth;

	fwrite(&st_bmp_file, sizeof(st_bmp_file), 1, f_bmp);
	fwrite(&st_bmp_info, sizeof(st_bmp_info), 1, f_bmp);

	if (depth == 1)
	{
		platte = (unsigned char*)malloc(sizeof(unsigned char) * 1024);
		if (!platte)
		{
			printf("allocate fail\n");
			jpeg_finish_decompress(&jpg_decompress);
			jpeg_destroy_decompress(&jpg_decompress);
			free(data_buffer);
			fclose(f_jpg);
			fclose(f_bmp);
			return false;
		}

		for (i = 0, j = 0; i < 1024; i += 4, j++)
		{
			platte[i] = j;
			platte[i + 1] = j;
			platte[i + 2] = j;
			platte[i + 3] = 0;
		}
		fwrite(platte, sizeof(unsigned char) * 1024, 1, f_bmp);
		free(platte);
	}

	line_buffer = (unsigned char*)malloc(sizeof(unsigned char)*width*depth);
	if (!line_buffer)
	{
		printf("allocate fail\n");
		jpeg_finish_decompress(&jpg_decompress);
		jpeg_destroy_decompress(&jpg_decompress);
		free(data_buffer);
		fclose(f_jpg);
		fclose(f_bmp);
		return false;
	}

	data_point = data_buffer + width * depth*(height - 1);
	//data_point = data_buffer;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < row_stride; j += depth)
		{
			if (depth == 1)
				line_buffer[j] = data_point[j];
			else if (depth == 3)
			{
				line_buffer[j + 2] = data_point[j + 0];
				line_buffer[j + 1] = data_point[j + 1];
				line_buffer[j + 0] = data_point[j + 2];
			}
		}
		data_point -= row_stride;
		//data_point += row_stride;
		fwrite(line_buffer, sizeof(unsigned char)*row_stride, 1, f_bmp);
	}

	jpeg_finish_decompress(&jpg_decompress);
	jpeg_destroy_decompress(&jpg_decompress);
	free(data_buffer);
	fclose(f_jpg);
	fclose(f_bmp);
	return true;
}

int main(int argc, char* argv[])
{
	if (jpg_to_bmp("h://56.jpg", "h://55.bmp"))
		printf("jpg to bmp translate finish\n");
	getchar();
	return 0;
}