#ifndef ENGINE_BUFFER
#define ENGINE_BUFFER

#include <vector>
#include <memory>
#include <glad\glad.h>
#include "Enums.hpp"
#include "Structs.hpp"
#include "Debug.hpp"
#include "Controller.hpp"

class VertexIndexBuffer;
class VertexDataBuffer;
class Window;

namespace BufferInfo {
    static int number_of_buffers;

}

/// <summary>
/// VERTEX_ARRAY_OBJECT: Buffer that stores pointers to the attributes, required
///     to be made before any other buffer
/// </summary>
class ArrayBuffer {
private:
    static int _alive_buffers;

    unsigned int _bindingID;
    std::vector<vertex_attribute> _attributes;
    std::vector<VertexDataBuffer*> _vbos;
public:
    static int GetAliveBuffers() { return _alive_buffers; };

    ArrayBuffer();
    ~ArrayBuffer();
    unsigned int GetID();
    void SetActive();

    void AddAttribute(GLuint location, GLint count, attribute_type attr, bool normalized = false);
    void SetAttribute(GLuint location, GLint count, attribute_type attr, bool normalized = false);
    void RemoveAttribute(GLuint location);
    char AttributeCount();

    VertexDataBuffer* CreateVertexBuffer(buffer_storage_type storage_type = buffer_storage_type::STATIC);
    VertexDataBuffer* CreateVertexBuffer(size_t byte_size, void* data, buffer_storage_type storage_type = buffer_storage_type::STATIC);
};


/// <summary>
/// VERTEX_BUFFER_OBJECT: Buffer that stores the actual attribute data that the 
///     array object points to
/// </summary>
class VertexDataBuffer {
private:
    static int _alive_buffers;
    static int _memory_used;

    ArrayBuffer* _parent;
    GLuint _bindingID;
    buffer_storage_type _storage_type;
    unsigned int _buffer_size;
    std::vector<VertexIndexBuffer*> _ebos;

    VertexDataBuffer(ArrayBuffer* parent, buffer_storage_type storage_type = buffer_storage_type::STATIC);
    friend class ArrayBuffer;
public:
    static int GetAliveBuffers() { return _alive_buffers; };
    static int GetMemoryUsed() { return _memory_used; };

    ~VertexDataBuffer();
    unsigned int GetID();
    void SetActive();
    void Write(size_t byte_size, void* data);

    VertexIndexBuffer* CreateIndexBuffer(buffer_storage_type storage_type = buffer_storage_type::STATIC);
    VertexIndexBuffer* CreateIndexBuffer(size_t count, unsigned int* indicies, buffer_storage_type storage_type = buffer_storage_type::STATIC);
};


/// <summary>
/// ELEMENT_BUFFER_OBJECT: Connecting indices from the vertex buffer object to
///     form elements ex. 3 indices makes one triangle, indices may repeat 
///     multiple times, saving space
/// </summary>
class VertexIndexBuffer {
private:
    static int _alive_buffers;
    static int _memory_used;

    VertexDataBuffer* _parent;
    GLuint _bindingID;
    buffer_storage_type _storage_type;
    unsigned int _buffer_size;

    VertexIndexBuffer(VertexDataBuffer* parent, buffer_storage_type storage_type = buffer_storage_type::STATIC);
    friend class VertexDataBuffer;
public:
    static int GetAliveBuffers() { return _alive_buffers; };
    static int GetMemoryUsed() { return _memory_used; };

    ~VertexIndexBuffer();
    unsigned int GetID();
    void SetActive();
    void Write(size_t byte_size, unsigned int* indicies);

    void Draw();
    void Draw(int offset, int count);
};

#endif