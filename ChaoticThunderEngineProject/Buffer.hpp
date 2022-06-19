#ifndef ENGINE_BUFFER
#define ENGINE_BUFFER

#include "Enums.hpp"
#include "Window.hpp"
#include <vector>

class VertexIndexBuffer;
class VertexDataBuffer;
class Window;

/// <summary>
/// VERTEX_ARRAY_OBJECT: Buffer that stores pointers to the attributes, required
///     to be made before any other buffer
/// </summary>
class ArrayBuffer {
private:
    Window& _window;
    GLuint _bindingID;
    std::vector<VertexDataBuffer> _vbos;
    std::vector<vertex_attribute> _attributes;
public:
    static ArrayBuffer none;

    ArrayBuffer(Window& window);
    ~ArrayBuffer();
    unsigned int GetID();
    void SetActive();

    void AddAttribute(GLuint location, GLint count, attribute_type attr, bool normalized = false);
    void SetAttribute(GLuint location, GLint count, attribute_type attr, bool normalized = false);
    void RemoveAttribute(GLuint location);
    char AttributeCount();

    VertexDataBuffer* CreateVertexBuffer(buffer_storage_type storage_type = STATIC);
    VertexDataBuffer* CreateVertexBuffer(unsigned int byte_size, void* data, buffer_storage_type storage_type = STATIC);
};


/// <summary>
/// VERTEX_BUFFER_OBJECT: Buffer that stores the actual attribute data that the 
///     array object points to
/// </summary>
class VertexDataBuffer {
private:
    ArrayBuffer& _parent;
    GLuint _bindingID;
    buffer_storage_type _storage_type;
    unsigned int _buffer_size;
    std::vector<VertexIndexBuffer> _ebos;

    VertexDataBuffer(ArrayBuffer& parent=ArrayBuffer::none, buffer_storage_type storage_type = STATIC);
    friend class ArrayBuffer;
public:
    static VertexDataBuffer none;
    ~VertexDataBuffer();
    VertexDataBuffer& operator=(const VertexDataBuffer&);
    unsigned int GetID();
    void SetActive();
    void Write(unsigned int byte_size, void* data);
    void Draw();
    void Draw(int offset, int count);

    VertexIndexBuffer* CreateIndexBuffer(buffer_storage_type storage_type = STATIC);
    VertexIndexBuffer* CreateIndexBuffer(unsigned int count, unsigned int* indicies, buffer_storage_type storage_type = STATIC);
};


/// <summary>
/// ELEMENT_BUFFER_OBJECT: Connecting indices from the vertex buffer object to
///     form elements ex. 3 indices makes one triangle, indices may repeat 
///     multiple times, saving space
/// </summary>
class VertexIndexBuffer {
private:
    static VertexIndexBuffer none;

    VertexDataBuffer& _parent;
    GLuint _bindingID;
    buffer_storage_type _storage_type;
    unsigned int _initial_capacity;

    VertexIndexBuffer(VertexDataBuffer& parent=VertexDataBuffer::none, buffer_storage_type storage_type = STATIC);
    friend class VertexDataBuffer;
public:
    ~VertexIndexBuffer();
    VertexIndexBuffer& operator=(const VertexIndexBuffer&);
    unsigned int GetID();
    void SetActive();
    void Write(unsigned int count, unsigned int* indicies);
    void Draw();
    void Draw(int offset, int count);
};

#endif