#include <stdio.h>
#include <stdbool.h>

#include "display.h"
#include "vec.h"
#include "image.h"
#include "mesh.h"

// #define N_POINTS 9 * 9 * 9 // 729 points
// vec3_t cube_points[N_POINTS];

// #define N_POINTS 48 // 3 points per triangle, 2 triangles per face, 6 faces
// vec2_t projected_points[N_POINTS];

triangle_t triangles_to_render[N_MESH_FACES];

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

    // un-needed as we have below solution
    //while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticks_last_frame + FRAME_TIME_LENGTH));

    int wait_time = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticks_last_frame);

    if (wait_time > 0 && wait_time <= FRAME_TIME_LENGTH) {
        SDL_Delay(wait_time);
    }

    ticks_last_frame = SDL_GetTicks();

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    // for (int i = 0; i < N_POINTS; i++) {
    //     vec3_t point = cube_points[i];

    //     vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
    //     //transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
    //     //transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

    //     transformed_point.z -= camera_position.z;

    //     // project the current point
    //     vec2_t projected_point = project(transformed_point);

    //     // append projected 2d vector to an array 
    //     projected_points[i] = projected_point;
    // }

    //loop all triangle faces of the mesh
    // int proj_points_index = 0;
    for (int i = 0; i < N_MESH_FACES; i++) {
        face_t mesh_face = mesh_faces[i]; 

        vec3_t face_vertices[3]; //array of 3 vec3's
        face_vertices[0] = mesh_vertices[mesh_face.a - 1]; //select 1st triangle vertices from mesh_vertices array
        face_vertices[1] = mesh_vertices[mesh_face.b - 1]; //select 2nd triangle vertices from mesh_vertices array
        face_vertices[2] = mesh_vertices[mesh_face.c - 1]; //select 3rd triangle vertices from mesh_vertices array

        triangle_t projected_triangle; // a triangle is an array of 3 vec2's

        for (int j = 0; j < 3; j++) {
            vec3_t vertex = face_vertices[j];

            vertex = vec3_rotate_x(vertex, cube_rotation.x);
            vertex = vec3_rotate_y(vertex, cube_rotation.y);
            vertex = vec3_rotate_z(vertex, cube_rotation.z);

            vertex.z -= camera_position.z;

            vec2_t projected_point = project(vertex);

            // translate projected points to centre of screen
            // rather than doing it in render()
            projected_point.x += (WINDOW_WIDTH / 2);
            projected_point.y += (WINDOW_HEIGHT / 2);

            projected_triangle.points[j] = projected_point;
        }

        triangles_to_render[i] = projected_triangle;

        /*
        
        BELOW IS MY FIRST ATTEMPT TO RENDER CUBE POINTS WITHOUT CONSIDERING TRIANGLES
        
        */

        // vec3_t transformed_point1 = vec3_rotate_x(face_vertices[0], cube_rotation.x);
        // vec3_t transformed_point2 = vec3_rotate_x(face_vertices[1], cube_rotation.x);
        // vec3_t transformed_point3 = vec3_rotate_x(face_vertices[2], cube_rotation.x);

        // transformed_point1.z -= camera_position.z;
        // transformed_point2.z -= camera_position.z;
        // transformed_point3.z -= camera_position.z;

        // vec2_t projected_point = project(transformed_point1);
        // vec2_t projected_point1 = project(transformed_point2);
        // vec2_t projected_point2 = project(transformed_point3);

        // projected_points[proj_points_index] = projected_point;
        // projected_points[proj_points_index + 1] = projected_point1;
        // projected_points[proj_points_index + 2] = projected_point2;

        // proj_points_index+=3;
    }


}

void render(void) {

    //SDL_RenderClear(renderer);
    clear_framebuffer();

    draw_grid(GRID_BROKEN, RED, 35);
    draw_rect(RECT_UNFILLED, 30, 30, 100, 100, WHITE);


    for (int i = 0; i < N_MESH_FACES; i++) {
        
        triangle_t triangle = triangles_to_render[i];

        for (int j = 0; j < 3; j++) {
            draw_rect(
                RECT_FILLED,
                triangle.points[j].x,
                triangle.points[j].y,
                1,
                1,
                YELLOW
            ); 

            int next = (j + 1) % 3;

            if (triangle.points[j].x > triangle.points[next].x) {
                vec2_t p0 = triangle.points[j];
                vec2_t p1 = triangle.points[next];

                triangle.points[j] = p1;
                triangle.points[next] = p0;
            }

            float dx = triangle.points[next].x - triangle.points[j].x;
            float dy = triangle.points[next].y - triangle.points[j].y;

            float m = dy / dx;

            // for(int x = triangle.points[j].x; x < triangle.points[next].x; x++) {
            //     int y = m * (x - triangle.points[j].x) + triangle.points[j].y;
            //     draw_rect(RECT_FILLED, x, y, 1, 1, WHITE);
            //     //draw_pixel(x, y, WHITE); - draw_pixel causes crashes?
            // }

            /*

            *************************************************************************
            Some notes on the solution for drawing lines between two triangle points
            *************************************************************************

            TWO APPROACHES :)

            ......................
            
            With the above solution, I am calculating the y position based on the
            "point slope form" of the equation of a line (y = m(x1-x0) + y0) for every 
            x position between one triangle point and the next. When you do this
            calculation, for example, lets say the difference in x points was 2, you
            are calculating what the y position would be if your origin was 0,0 - that 
            is why you must add the initial starting y position at the end to get the
            actual next y position

            There is an alernate approach here; instead of calculating the y for every x 
            between the two triangle points, you can simply add the slope to the current
            y position every time you increment on x.

            This means, for example, you can simply do 

            y = 225
            new y = 225 + -0.219

            rather than

            y = -0.219 * (difference between x points) + 225 (225 being the initial y pos)

            As of the time of writing, while I haven't yet implemented the second solution,
            I believe it may be faster as it doesn't involve doing a multiplication
            for every x between the two triangle points you are drawing the line between.

            */


            /* 
            
            Further explanation in order for you to make a connection in your head between the above
            solution and the below one:

            when you use the point slope form method, for example, 

            lets say you have the point 175,225 and the slope -0.219

            I want to find what y will be when x is 176.....
            
            using the point slope form:

            y = m (x2 - x1) + y1

            y = -0.219(176 - 175) + 225
            y = -0.219(1) + 225

            SO, by doing -0.219(1) we are finding out what y will be if our origin was 0,0 and 1 was the next point -
            we then add 225 to find out what our y point really is

            However, by using an iterative solution instead, we don't have to do any multiplication, as by
            iterating between our current x point and the next x point and doing an addition assignment, theres no need

            For example:

            y = -0.219(2) + 225 = 224.562

            is exactly the same as iterating twice

            on the first iteration y would be 224.781 as 225 - 0.219 = 224.781

            and then on the second iteration we would simply add -0.219 to that, leaving us with 224.562; again 
            the exact same result as if we did -0.219(2) + 225 to find out what the y would be if our x was 2
            s
            */

           float currentY = triangle.points[j].y; //declaring currentY as int causes issues, investigate
           for(int x = triangle.points[j].x; x < triangle.points[next].x; x++) {
                currentY += m;
                draw_pixel(x, currentY, WHITE);
           }

        }
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
