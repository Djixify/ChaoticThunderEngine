#ifndef ENGINE_STRUCTS
#define ENGINE_STRUCTS
#include <glad\glad.h>
#include "Enums.hpp"

struct vertex_attribute {
    bool normalized;
    GLuint location;
    GLint count;
    GLuint offset;
    attribute_type type;
};

#endif