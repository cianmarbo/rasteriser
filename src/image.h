#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <time.h>

#include "display.h"

// image file format
#define PPM                 0x1

// pixel strides for 24 and 32-bit colour, only really need RGB constant for PPM conversion
#define PIXEL_STRIDE_RGB    3
#define PIXEL_STRIDE_ARGB   4

// RGB frame buffer to allow us to convert from ARGB to RGB
#define RGB_BUFFER_SIZE_BYTES ( (sizeof(uint8_t) * 3) * (WINDOW_WIDTH * WINDOW_HEIGHT) )

extern uint8_t* rgb_buffer;

// ppm only supports 24-bit colour, so 3 bytes per channel
void argb_to_rgb(void);
void image_write(uint32_t format);
const char* image_filename(void);

#endif