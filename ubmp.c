#include "ubmp.h"

/* OPTIMIZED FOR EMBEDDED APPLICATION, (C) Ehsan Varasteh 2015 */

void bmp_loadsize(char *file, SIZE_t *size) {
  FILE *fi;
  
  fi = fopen(file, "rb");
  if(fi == NULL) { 
    size->height = 0; size->width = 0;
    return;
  }
  
  fseek(fi, 18, SEEK_SET);
  fread(&size->width, 4, 1, fi);
  fread(&size->height, 4, 1, fi);
  fclose(fi);
}

uBMP *bmp_open(char *file) { // open bmp file and load header
  uBMP *bmp;
  
  bmp = (uBMP*) malloc(sizeof(uBMP));
  bmp->fHandle = fopen(file, "rb");
  if(bmp->fHandle == NULL) { free(bmp); return NULL; }
  fread(&bmp->bmpHeader.Magic, 2, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.FileSize, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.Reserved1, 2, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.Reserved2, 2, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.DataOffset, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.HeaderSize, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.Width, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.Height, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.Planes, 2, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.BitsPerPixel, 2, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.CompressionType, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.ImageDataSize, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.HPixelsPerMeter, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.VPixelsPerMeter, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.ColorsUsed, 4, 1, bmp->fHandle); // 
  fread(&bmp->bmpHeader.ColorsRequired,4, 1, bmp->fHandle); // 
  
  if(bmp->bmpHeader.Magic != 0x4D42) { free(bmp); return NULL; } // not BMP!
  
  return bmp;
}

RGB_t bmp_getpixel(uBMP *bmp, uint16_t x, uint16_t y) { // get pixel
  uint32_t offset, rgb;
  RGB_t rt = {0,0,0};
  uint8_t pad;
  
  if(bmp == NULL) return rt;
  
  if(x > bmp->bmpHeader.Width || y > bmp->bmpHeader.Height)
	  return rt;
  
  pad = 4 - ((bmp->bmpHeader.Width * (bmp->bmpHeader.BitsPerPixel/8))%4);
  if(pad == 4) pad = 0;
  offset = bmp->bmpHeader.DataOffset + (y * ((bmp->bmpHeader.Width * (bmp->bmpHeader.BitsPerPixel/8))+pad)) + (x * (bmp->bmpHeader.BitsPerPixel/8)); // offset of it!
  
  fseek(bmp->fHandle, offset, SEEK_SET);
  if(bmp->bmpHeader.BitsPerPixel == 24)
    fread(&rt, bmp->bmpHeader.BitsPerPixel/8, 1, bmp->fHandle);
  else
    fread(&rgb, bmp->bmpHeader.BitsPerPixel/8, 1, bmp->fHandle);
  
  if(bmp->bmpHeader.BitsPerPixel == 16) { // RGB565 to RGB888
     rt.r = ( ((rgb & 0xf800) >> 10) * 527 + 23 ) >> 6;
     rt.g = ( ((rgb & 0x07e0) >> 4) * 259 + 33 ) >> 6;
     rt.b = ( (rgb & 0x001f) * 527 + 23 ) >> 6;
  }
  
  return rt;
}

/* getcol is simply multiple getpixel calls */
RGB_t* bmp_getcol(uBMP *bmp, uint16_t x) {
  uint32_t n;
  RGB_t *rgb_arr;

  rgb_arr = (RGB_t*) malloc(sizeof(RGB_t) * bmp->bmpHeader.Height);

  for(n=0;n<bmp->bmpHeader.Height;n++)
    rgb_arr[n] = bmp_getpixel(bmp, x, n);
	
  return rgb_arr; // user should free memory after use
}

RGB_t* bmp_getrow(uBMP *bmp, uint16_t y) {
  uint32_t n, offset;
  RGB_t *rgb_arr;
  uint16_t *rgb = NULL;
  uint8_t pad;
  
  if(bmp->bmpHeader.BitsPerPixel == 16)
    rgb = malloc(sizeof(uint16_t) * bmp->bmpHeader.Width);
  
  rgb_arr = (RGB_t*) malloc(sizeof(RGB_t) * bmp->bmpHeader.Width);
  
  pad = 4 - ((bmp->bmpHeader.Width * (bmp->bmpHeader.BitsPerPixel/8))%4);
  if(pad == 4) pad = 0;
  offset = bmp->bmpHeader.DataOffset + (y * ((bmp->bmpHeader.Width * (bmp->bmpHeader.BitsPerPixel/8))+pad)); // offset of first item

  fseek(bmp->fHandle, offset, SEEK_SET);
  if(bmp->bmpHeader.BitsPerPixel == 24)
    fread(rgb_arr, (bmp->bmpHeader.BitsPerPixel/8), bmp->bmpHeader.Width, bmp->fHandle); // read whole rgb
  else
    fread(rgb, (bmp->bmpHeader.BitsPerPixel/8), bmp->bmpHeader.Width, bmp->fHandle); // read whole rgb
  
  if(bmp->bmpHeader.BitsPerPixel == 16) {
    for(n=0;n<bmp->bmpHeader.Width;n++) { // conversion
      rgb_arr[n].r = ( ((rgb[n] & 0xf800) >> 10) * 527 + 23 ) >> 6;
      rgb_arr[n].g = ( ((rgb[n] & 0x07e0) >> 4) * 259 + 33 ) >> 6;
      rgb_arr[n].b = ( (rgb[n] & 0x001f) * 527 + 23 ) >> 6;
    }
    free(rgb);
  }
  
  return rgb_arr; // user should free memory after use
}

uint16_t bmp_getwidth(uBMP *bmp) {
  if(bmp == NULL) return 0;
  
  return bmp->bmpHeader.Width;
}

uint16_t bmp_getheight(uBMP *bmp) {
  if(bmp == NULL) return 0;
  
  return bmp->bmpHeader.Height;
}

void bmp_close(uBMP *bmp) {
  if(bmp == NULL) return;
  //if(bmp->fHandle == NULL) return;
  fclose(bmp->fHandle);
  free(bmp);
}
