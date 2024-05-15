#include "image.h"
#include "display.h"

uint8_t* rgb_buffer = NULL;

// ppm only supports 24-bit colour, so 3 bytes per channel
void argb_to_rgb(void) {

    int rgb_buffer_counter = 0;

    for(int i = 0; i < (WINDOW_WIDTH * WINDOW_HEIGHT); i++) {
        rgb_buffer[rgb_buffer_counter] = frame_buffer[i] >> 16;
        rgb_buffer[rgb_buffer_counter+1] = frame_buffer[i] >> 8;
        rgb_buffer[rgb_buffer_counter+2] = frame_buffer[i];

        rgb_buffer_counter+=3;
    }
}

// write frame buffer to an image, hoping to support more formats soon
void image_write(uint32_t format) {
    const char* file_name = image_filename();
    if (format == PPM) {
        FILE* file = fopen(file_name, "wb");
        fprintf(file, "P6\n%i %i\n255\n", WINDOW_WIDTH, WINDOW_HEIGHT);
        fwrite(rgb_buffer, 1, RGB_BUFFER_SIZE_BYTES, file);
        fclose(file);
    }
}

// return date as file name for image exports
const char* image_filename(void) {
    time_t t = time(NULL);
    return asctime(localtime(&t));
}
