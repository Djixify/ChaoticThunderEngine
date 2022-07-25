#ifndef ENGINE_STRUCTS
#define ENGINE_STRUCTS
#include <glad\glad.h>
#include <string>
#include "Enums.hpp"

struct load_shader {
    shader_type type;
    std::string path;
};

struct vertex_attribute {
    bool normalized;
    GLuint location;
    GLint count;
    GLuint stride;
    attribute_type type;
    GLuint offset;
};

struct vertex_xyz {
    float x, y, z;
    vertex_xyz(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct vertex_xyz_rgb : vertex_xyz {
    float r, g, b;
    vertex_xyz_rgb(float x, float y, float z, float r, float g, float b) : vertex_xyz(x,y,z) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

struct vertex_xyz_txy : vertex_xyz {
    float x, y, z, tx, ty;
    vertex_xyz_txy(float x, float y, float z, float tx, float ty) : vertex_xyz(x, y, z) {
        this->tx = tx;
        this->ty = ty;
    }
};
#endif