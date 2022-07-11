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

#endif