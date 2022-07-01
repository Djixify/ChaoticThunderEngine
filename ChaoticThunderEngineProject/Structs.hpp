#ifndef ENGINE_STRUCTS
#define ENGINE_STRUCTS
#include <glad\glad.h>
#include "Enums.hpp"

struct vertex_attribute {
    bool normalized;
    GLuint location;
    GLint count;
    GLuint stride;
    attribute_type type;
    GLuint offset;
};

#endif