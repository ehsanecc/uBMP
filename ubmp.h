#ifndef _UBMP_
#define _UBMP_

/* OPTIMIZED FOR EMBEDDED APPLICATION, (C) Ehsan Varasteh 2015 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifndef _STDLIB
/* standard declarations, in case of not defined */
typedef unsigned long long int uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#endif

/* Bitmap header */
typedef struct
{
	uint16_t	Magic;				/* Magic identifier: "BM" */
	uint32_t	FileSize;			/* Size of the BMP file in bytes */
	uint16_t	Reserved1;			/* Reserved */
	uint16_t	Reserved2;			/* Reserved */
	uint32_t	DataOffset;			/* Offset of image data relative to the file's start */
	uint32_t	HeaderSize;			/* Size of the header in bytes */
	uint32_t	Width;				/* Bitmap's width */
	uint32_t	Height;				/* Bitmap's height */
	uint16_t	Planes;				/* Number of color planes in the bitmap */
	uint16_t	BitsPerPixel;		/* Number of bits per pixel */
	uint32_t	CompressionType;	/* Compression type */
	uint32_t	ImageDataSize;		/* Size of uncompressed image's data */
	uint32_t	HPixelsPerMeter;	/* Horizontal resolution (pixels per meter) */
	uint32_t	VPixelsPerMeter;	/* Vertical resolution (pixels per meter) */
	uint32_t	ColorsUsed;			/* Number of color indexes in the color table that are actually used by the bitmap */
	uint32_t	ColorsRequired;		/* Number of color indexes that are required for displaying the bitmap */
} BMP_Header;

typedef struct {
	FILE *fHandle;
	BMP_Header bmpHeader;
} uBMP;

typedef struct {
	uint8_t b,g,r;
} RGB_t;

typedef struct {
	uint8_t x,r,g,b;
} XRGB_t;

typedef struct {
        uint32_t width, height;
} SIZE_t;

uBMP *bmp_open(char *file); // open bmp file and load header
uint16_t bmp_getwidth(uBMP *bmp);
uint16_t bmp_getheight(uBMP *bmp);
RGB_t bmp_getpixel(uBMP *bmp, uint16_t x, uint16_t y); // get pixel by pixel
RGB_t* bmp_getcol(uBMP *bmp, uint16_t x);
RGB_t* bmp_getrow(uBMP *bmp, uint16_t y);
void bmp_close(uBMP *bmp);

void bmp_loadsize(char *file, SIZE_t *size); // direct size load

// pixel formats
// 16b RGB565
// 24b RGB888
// 32b XRGB8888

#endif
