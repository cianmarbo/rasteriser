#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

// window configuration
#define WINDOW_WIDTH        1024
#define WINDOW_HEIGHT       768

// colours
#define BLUE                0xff0000ff
#define GREEN               0xff00ff00
#define RED                 0xffff0000
#define GREY                0xff333333
#define WHITE               0xffffffff
#define YELLOW              0xffffff00

// grid modes
#define GRID_CONTINUOUS     0x1
#define GRID_BROKEN         0x2

// rectangle drawing modes
#define RECT_FILLED         0x1
#define RECT_UNFILLED       0x2

// frame buffer size in bytes
#define FRAME_BUFFER_SIZE_BYTES (sizeof(uint32_t) * (WINDOW_WIDTH * WINDOW_HEIGHT))

#define FPS 30
#define FRAME_TIME_LENGTH (1000 / FPS)

extern int ticks_last_frame;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* texture;

extern uint32_t* frame_buffer;

void draw_pixel(uint32_t posX, uint32_t posY, uint32_t colour);
void draw_rect(uint32_t mode, uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, uint32_t colour);
void draw_grid(uint32_t mode, uint32_t colour, uint32_t spacing);

void clear_framebuffer(void);
void clear_framebuffer_colour(uint32_t colour);

void render_framebuffer(void);

bool init_window(void);

void destroy_window(void);

#endif