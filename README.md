# uBMP
uBMP is a library for microcontrollers ( and all small size RAM devices ) to read bitmap files

----------
Library uses **FatFS** for fopen, fread, fseek and fclose functions.

Library consist of these functions:

- **uBMP \*bmp_open(char \*file);**

Open a file and read header and allocate a uBMP structure and return the pointer to allocated uBMP structure.

- **uint16_t bmp\_getwidth(uBMP \*bmp);**

- **uint16_t bmp\_getheight(uBMP \*bmp);**

Read the width/height of opened bitmap file, take uBMP structure as input to read.

- **RGB\_t bmp\_getpixel(uBMP \*bmp, uint16\_t x, uint16\_t y);**

Read a pixel from given (x,y) coordinate, return a RGB_t structure.

- **RGB\_t\* bmp_getcol(uBMP \*bmp, uint16\_t x);**

- **RGB\_t\* bmp_getrow(uBMP \*bmp, uint16\_t y);**

Read a column/row from given x or y, allocate RGB\_t array and return it. User **SHOULD** free RGB\_t array after use.

- **void bmp_close(uBMP \*bmp);**

Close opened bitmap file, and free allocated structure.

- **void bmp\_loadsize(char \*file, SIZE\_t \*size);**

Load bitmap size with just filename and one function call. and fill SIZE_t address.


And thats it, a simple bitmap library!