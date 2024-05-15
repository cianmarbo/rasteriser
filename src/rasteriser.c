#include <stdio.h>
#include <stdbool.h>

#include "display.h"
#include "vec.h"
#include "image.h"

bool running = NULL;

bool setup(void) {

    frame_buffer = (uint32_t*) malloc(FRAME_BUFFER_SIZE_BYTES);
    rgb_buffer = (uint8_t*) calloc(PIXEL_STRIDE_RGB * WINDOW_WIDTH * WINDOW_HEIGHT, sizeof(uint8_t));

    if (!frame_buffer) {
        fprintf(stderr, "There was a problem allocating memory for the frame buffer...\n");
        return false;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    if (!texture) {
        fprintf(stderr, "There was a problem creating an SDL texture...\n");
        return false;
    }

    return true;
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_p) {
                argb_to_rgb();
                image_write(PPM);
            }
            break;
        }
    }
}

void render(void) {

    SDL_RenderClear(renderer);
    clear_framebuffer();

    draw_grid(GRID_BROKEN, RED, 35);
    draw_rect(RECT_UNFILLED, 30, 30, 100, 100, WHITE);

    render_framebuffer();

    SDL_RenderPresent(renderer);
}

void quit(void) {
    destroy_window();
    free(frame_buffer);
    free(rgb_buffer);
}

int main(int argc, char** argv) {

    time_t t = time(NULL);
    printf("UTC:   %s", asctime(localtime(&t)));

    running = init_window();

    if (!setup()) {
        fprintf(stderr, "Setup process failed...\n");
        return -1;
    }

    while (running) {
        process_input();
        render();
    }

    quit();

    return 0;
}
