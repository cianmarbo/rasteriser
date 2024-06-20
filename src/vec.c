#include <math.h>
#include "vec.h"

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = v.x,
        .y = cos(angle) * v.y - sin(angle) * v.z,
        .z = sin(angle) * v.y + cos(angle) * v.z
    };

    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = cos(angle) * v.x - sin(angle) * v.z,
        .y = v.y,
        .z = sin(angle) * v.x + cos(angle) * v.z
    };

    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = cos(angle) * v.x - sin(angle) * v.y,
        .y = sin(angle) * v.x + cos(angle) * v.y,
        .z = v.z
    };

    return rotated_vector;
}
