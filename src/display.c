#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

uint32_t* frame_buffer = NULL;

void draw_pixel(uint32_t posX, uint32_t posY, uint32_t colour) {
    if (posX < WINDOW_WIDTH && posY < WINDOW_HEIGHT) {
        frame_buffer[(WINDOW_WIDTH * posY) + posX] = colour;
    }
}

void draw_rect(uint32_t mode, uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, uint32_t colour) {

    if (posX < WINDOW_WIDTH && posY < WINDOW_HEIGHT) {
        if (mode == RECT_FILLED) {
            for (int y = posY; y < (posY + height); y++) {
                for (int x = posX; x < (posX + width); x++) {
                    frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
                }
            }

        } else if (mode == RECT_UNFILLED) {
            for (int y = posY; y < (posY + height); y++) {
                for (int x = posX; x < (posX + width); x++) {
                    if (y == posY || y == (posY + height) - 1) {
                        frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
                    } else {
                        frame_buffer[(WINDOW_WIDTH * y) + posX] = colour;
                        frame_buffer[(WINDOW_WIDTH * y) + (posX + width - 1)] = colour;
                    }
                }
            }
        }
    }
}

// draw a grid with lines or a grid with dots
void draw_grid(uint32_t mode, uint32_t colour, uint32_t spacing) {

    if(mode == GRID_CONTINUOUS) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            for (int x = 0; x < WINDOW_WIDTH; x++) {
                if (y % spacing == 0 || x % spacing == 0) {
                    frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
                }
            }
        }
    }

    if(mode == GRID_BROKEN) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            for (int x = 0; x < WINDOW_WIDTH; x++) {
                if (y % spacing == 0 && x % spacing == 0) {
                    frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
                }
            }
        }
    }

}

void draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {

}

void clear_framebuffer_colour(uint32_t colour) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            frame_buffer[(WINDOW_WIDTH * y) + x] = colour;
        }
    }
}

void clear_framebuffer(void) {
    for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
        frame_buffer[i] = 0xff000000;
    }
}

void render_framebuffer(void) {

    SDL_UpdateTexture(texture, NULL, frame_buffer, (WINDOW_WIDTH * sizeof(uint32_t)));

    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

bool init_window(void) {

    if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL failed to init...\n");
        return false;
    }

    window = SDL_CreateWindow(
        "rasteriser",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (!window) {
        fprintf(stderr, "There was an error creating a window...\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "There was an error creating an SDL Renderer...\n");
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void destroy_window(void) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);

    SDL_Quit();
}