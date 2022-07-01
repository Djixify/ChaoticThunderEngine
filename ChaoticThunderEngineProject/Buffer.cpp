#include "Buffer.hpp"
#include "Debug.hpp"
#include "Controller.hpp"

///////////////////////////////////////
///           Array buffer          ///
///////////////////////////////////////

ArrayBuffer::ArrayBuffer(Window& window) : _window(window) {
    this->_window.SetActive();

    glGenVertexArrays(1, &this->_bindingID);

    std::vector<VertexDataBuffer> vertex_data_buffers(8);
    this->_vbos = vertex_data_buffers;
    Debug::Logger::Console(Debug::Level::CONSTRUCTION, "Created vertex array buffer at %d", this->_bindingID);
}

char empty_arraybuffer = 0;
ArrayBuffer* ArrayBuffer::none = (ArrayBuffer*)&empty_arraybuffer;

ArrayBuffer::~ArrayBuffer() {
    while (_vbos.size() > 0) {
        delete &_vbos.back();
        _vbos.pop_back();
    }
    _vbos.shrink_to_fit();
    glDeleteVertexArrays(1, &_bindingID);

    Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destoryed vertex array buffer at %d", this->_bindingID);
}

unsigned int ArrayBuffer::GetID() { return _bindingID; }

void ArrayBuffer::SetActive() {
    this->_window.SetActive();

    Debug::Logger::Console(Debug::Level::CONTEXT, "Setting context array buffer: %d", this->_bindingID);
    
    glBindVertexArray(_bindingID);
}

void ArrayBuffer::AddAttribute(GLuint location, GLint count, attribute_type attr, bool normalized)
{
    this->SetActive();

    if (count > 4 || count < 0)
        Controller::Instance()->ThrowException("Invalid count given for attribute, can only accept between 1 and 4");

    for (vertex_attribute vattr : _attributes) {
        if (vattr.location == location) {
            Controller::Instance()->ThrowException("Location was already added, use set to overwrite");
        }
    }

    GLuint stride = 0;
    for (vertex_attribute vattr : _attributes) {
        stride += vattr.count * AttributeSize(vattr.type);
    }
    stride += count * AttributeSize(attr);
    vertex_attribute newvattr{ normalized, location, count, stride, attr };

    _attributes.push_back(newvattr);

    GLuint offset = 0;
    for (vertex_attribute vattr : _attributes) {
        glVertexAttribPointer(
            vattr.location,                  //Location (used as reference in GLSL)
            vattr.count,                     //Number of values in attribute
            (GLuint)vattr.type,              //Type
            vattr.normalized ? GL_TRUE : GL_FALSE,
            stride,                         //Stride 
            (void*)(offset)                 //Vertex offset
        );
        offset += vattr.count * AttributeSize(vattr.type);
        glEnableVertexAttribArray(vattr.location);
    }
}

void ArrayBuffer::SetAttribute(GLuint location, GLint count, attribute_type attr, bool normalized)
{
    Controller::Instance()->ThrowException("Not implemented");
}

void ArrayBuffer::RemoveAttribute(GLuint location)
{
    Controller::Instance()->ThrowException("Not implemented");
}

char ArrayBuffer::AttributeCount()
{
    return _attributes.size();
}

VertexDataBuffer* ArrayBuffer::CreateVertexBuffer(buffer_storage_type storage_type) {
    VertexDataBuffer buffer(*this, storage_type);
    this->_vbos.push_back(buffer);
    return &this->_vbos.back();
}

VertexDataBuffer* ArrayBuffer::CreateVertexBuffer(unsigned int byte_size, void* data, buffer_storage_type storage_type) {
    VertexDataBuffer buffer(*this, storage_type);
    this->_vbos.push_back(buffer);
    buffer.Write(byte_size, data);
    return &this->_vbos.back();
}


///////////////////////////////////////
///       Vertex data buffer        ///
///////////////////////////////////////

VertexDataBuffer::VertexDataBuffer(ArrayBuffer& parent, buffer_storage_type storage_type) : _parent(parent), _storage_type(storage_type) {
    _parent.SetActive();
    _buffer_size = 0;

    glGenBuffers(1, &_bindingID);
    glBindBuffer(GL_ARRAY_BUFFER, _bindingID);

    std::vector<VertexIndexBuffer> vertex_index_buffers;
    this->_ebos = vertex_index_buffers;

    Debug::Logger::Console(Debug::Level::CONSTRUCTION, "Created vertex data buffer at id: %d", _bindingID);
}

VertexDataBuffer::~VertexDataBuffer() {
    while (_ebos.size() > 0) {
        delete& _ebos.back();
        _ebos.pop_back();
    }
    glDeleteBuffers(1, &_bindingID);
    Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destroyed vertex data buffer at %d", _bindingID);
}

VertexDataBuffer& VertexDataBuffer::operator=(const VertexDataBuffer& other)
{
    Debug::Logger::Console(Debug::Level::COPY, "Copying vertex data buffer from index: %d", other._bindingID);
    VertexDataBuffer newbuffer(other._parent);
    newbuffer._bindingID = other._bindingID;
    newbuffer._storage_type = other._storage_type;
    newbuffer._ebos.reserve(other._ebos.size());
    for (int i = 0; i < other._ebos.size(); i++)
        newbuffer._ebos[i] = other._ebos[i];
    newbuffer._buffer_size = other._buffer_size;
    return newbuffer;
}

unsigned int VertexDataBuffer::GetID() { return _bindingID; }

void VertexDataBuffer::SetActive() {
    _parent.SetActive();

    Debug::Logger::Console(Debug::Level::CONTEXT, "Setting context data buffer: %d", _bindingID);
    
    glBindBuffer(GL_ARRAY_BUFFER, _bindingID);
}

void VertexDataBuffer::Write(unsigned int byte_size, void* data) {
    this->SetActive();

    if (_buffer_size > 0 && byte_size > _buffer_size)
        Controller::Instance()->ThrowException("Tried to write index array to buffer larger than initial capacity");

    Debug::Logger::Console(Debug::Level::WRITING, "Writing %d bytes of data buffer %d", byte_size, this->_bindingID);
    glBindBuffer(GL_ARRAY_BUFFER, this->_bindingID);
    glBufferData(GL_ARRAY_BUFFER, byte_size, data, this->_storage_type);
}

void VertexDataBuffer::Draw()
{
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void VertexDataBuffer::Draw(int offset, int count)
{
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

VertexIndexBuffer* VertexDataBuffer::CreateIndexBuffer(buffer_storage_type storage_type) {
    VertexIndexBuffer buffer(*this, storage_type);
    this->_ebos.push_back(buffer);
    return &this->_ebos.back();
}

VertexIndexBuffer* VertexDataBuffer::CreateIndexBuffer(unsigned int count, unsigned int* indicies, buffer_storage_type storage_type) {
    VertexIndexBuffer buffer(*this, storage_type);
    buffer.Write(count, indicies);
    this->_ebos.push_back(buffer);
    return &this->_ebos.back();
}

///////////////////////////////////////
///       Vertex index buffer       ///
///////////////////////////////////////

VertexIndexBuffer::VertexIndexBuffer(VertexDataBuffer& parent, buffer_storage_type storage_type) : _parent(parent), _storage_type(storage_type) {
    _parent.SetActive();
    _initial_capacity = 0;

    glGenBuffers(1, &_bindingID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bindingID);
    Debug::Logger::Console(Debug::Level::CONSTRUCTION, "Created vertex index/element buffer at %d", this->_bindingID);
}

VertexIndexBuffer::~VertexIndexBuffer() {
    glDeleteBuffers(1, &_bindingID);
    Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destroyed vertex index/element buffer at %d", _bindingID);
}

VertexIndexBuffer& VertexIndexBuffer::operator=(const VertexIndexBuffer& other)
{
    Debug::Logger::Console(Debug::Level::COPY, "Copying vertex data buffer from index: %d", other._bindingID);
    VertexIndexBuffer newbuffer(other._parent);
    newbuffer._bindingID = other._bindingID;
    newbuffer._storage_type = other._storage_type;
    newbuffer._initial_capacity = other._initial_capacity;
    return newbuffer;
}

unsigned int VertexIndexBuffer::GetID() { return _bindingID; }

void VertexIndexBuffer::SetActive() {
    _parent.SetActive();

    Debug::Logger::Console(Debug::Level::CONTEXT, "Setting context index buffer: %d", _bindingID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bindingID);
}

void VertexIndexBuffer::Write(unsigned int count, unsigned int* indicies) {
    this->SetActive();

    if (_initial_capacity > 0 && count > _initial_capacity)
        Controller::Instance()->ThrowException("Tried to write index array to buffer larger than initial capacity");

    Debug::Logger::Console(Debug::Level::WRITING, "Writing %d indicies into index buffer %d", count, this->_bindingID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_bindingID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indicies, this->_storage_type);
    _initial_capacity = count;
}

void VertexIndexBuffer::Draw() {

}

void VertexIndexBuffer::Draw(int offset, int count) {

}
