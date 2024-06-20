#include <stdio.h>
#include <stdbool.h>

#include "display.h"
#include "vec.h"
#include "image.h"

#define N_POINTS 9 * 9 * 9 // 729 points
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {
    .x = 0,
    .y = 0,
    .z = -5 //set to 0 for 2001 a space odyssey effect!!
};

vec3_t cube_rotation = {
    .x = 0,
    .y = 0,
    .z = 0
};

float fov_factor = 500; //set to 2 for 2001 a space odyssey effect!!

int ticks_last_frame = 0;

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

    // initialize all vectors for cube
    int point_counter = 0;

    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                vec3_t new_point = { .x = x, .y = y, .z = z};
                cube_points[point_counter] = new_point;
                point_counter++;
            }
        }
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

// recieves vec3 and returns projected 2d vector
vec2_t project(vec3_t point) {

    vec2_t projected_point = {
        .x = (point.x / point.z) * fov_factor,
        .y = (point.y / point.z) * fov_factor
    };

    return projected_point;
}

void update(void) {

    //while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticks_last_frame + FRAME_TIME_LENGTH));

    int wait_time = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticks_last_frame);

    if (wait_time > 0 && wait_time <= FRAME_TIME_LENGTH) {
        SDL_Delay(wait_time);
    }

    ticks_last_frame = SDL_GetTicks();

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (int i = 0; i < N_POINTS; i++) {
        vec3_t point = cube_points[i];

        vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
        transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

        transformed_point.z -= camera_position.z;

        // project the current point
        vec2_t projected_point = project(transformed_point);

        // append projected 2d vector to an array 
        projected_points[i] = projected_point;
    }
}

void render(void) {

    //SDL_RenderClear(renderer);
    clear_framebuffer();

    //draw_grid(GRID_BROKEN, RED, 35);
    draw_rect(RECT_UNFILLED, 30, 30, 100, 100, WHITE);

    for (int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        
        draw_rect(
            RECT_FILLED,
            projected_point.x + (WINDOW_WIDTH / 2) , // adding width/2 and height/2 moves the cube to the centre of the window
            projected_point.y + (WINDOW_HEIGHT / 2),
            4,
            4,
            YELLOW
        );
    }

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
        update();
        render();
    }

    quit();

    return 0;
}
