#include "Buffer.hpp"
#include "Debug.hpp"
#include "Controller.hpp"

///////////////////////////////////////
///           Array buffer          ///
///////////////////////////////////////

ArrayBuffer::ArrayBuffer(Window* window) : _window(window) {
    this->_window->SetActive();

    unsigned int tmp = 0;
    glGenVertexArrays(1, &tmp);
    Debug::Logger::ConsoleOpenGLError("During generation of array buffer");

    std::vector<VertexDataBuffer> vertex_data_buffers;
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
    Debug::Logger::ConsoleOpenGLError("During destruction of array buffer");

    Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destoryed vertex array buffer at %d", this->_bindingID);
}

unsigned int ArrayBuffer::GetID() { return _bindingID; }

void ArrayBuffer::SetActive() {
    this->_window->SetActive();

    Debug::Logger::Console(Debug::Level::CONTEXT, "Setting context array buffer: %d", this->_bindingID);
    
    glBindVertexArray(_bindingID);
    Debug::Logger::ConsoleOpenGLError("During setting array buffer as active");
}

void ArrayBuffer::AddAttribute(GLuint location, GLint count, attribute_type attr, bool normalized)
{
    this->SetActive();

    if (count > 4 || count < 0)
        Controller::Instance()->ThrowException("Invalid count given for attribute, can only accept between 1 and 4");

    for (int i = 0; i < _attributes.size(); i++) {
        if (_attributes[i].location == location) {
            Controller::Instance()->ThrowException("Location was already added, use set to overwrite");
        }
    }

    GLuint stride = 0;
    for (int i = 0; i < _attributes.size(); i++) {
        stride += _attributes[i].count * AttributeSize(_attributes[i].type);
    }
    stride += count * AttributeSize(attr);
    vertex_attribute newvattr{ normalized, -1, count, stride, attr };

    _attributes.push_back(newvattr);

    GLuint offset = 0;
    for (int i = 0; i < _attributes.size(); i++) {
        _attributes[i].location = i;
        _attributes[i].stride = stride;
        _attributes[i].offset = offset;
        glEnableVertexAttribArray(_attributes[i].location);
        Debug::Logger::ConsoleOpenGLError("During enabling vertex attribute");
        glVertexAttribPointer(
            _attributes[i].location,                       //Location (used as reference in GLSL)
            _attributes[i].count,                          //Number of values in attribute
            (GLuint)_attributes[i].type,                   //Type
            _attributes[i].normalized ? GL_TRUE : GL_FALSE,
            _attributes[i].stride,                         //Stride 
            (void*)(_attributes[i].offset)                 //Vertex offset
        );
        Debug::Logger::ConsoleOpenGLError("During setting vertex attribute");
        offset += _attributes[i].count * AttributeSize(_attributes[i].type);
    }
}

void ArrayBuffer::SetAttribute(GLuint location, GLint count, attribute_type attr, bool normalized)
{
    this->SetActive();

    if (count > 4 || count < 0)
        Controller::Instance()->ThrowException("Invalid count given for attribute, can only accept between 1 and 4");

    vertex_attribute newvattr{ normalized, -1, count, 0, attr };

    bool found = false;
    int loc = 0;
    for (int i = 0; i < _attributes.size(); i++) {
        if (_attributes[i].location == location) {
            found = true;
            loc = i;
            _attributes[i] = newvattr;
        }
    }
    if (!found)
        return; //should probably return bool as indication for success or failure

    GLuint stride = 0;
    for (int i = 0; i < _attributes.size(); i++) {
        stride += _attributes[i].count * AttributeSize(_attributes[i].type);
    }

    _attributes.push_back(newvattr);

    GLuint offset = 0;
    for (int i = 0; i < _attributes.size(); i++) {
        _attributes[i].location = i;
        _attributes[i].stride = stride;
        _attributes[i].offset = offset;
        glEnableVertexAttribArray(_attributes[i].location);
        Debug::Logger::ConsoleOpenGLError("During enabling vertex attribute");
        glVertexAttribPointer(
            _attributes[i].location,                       //Location (used as reference in GLSL)
            _attributes[i].count,                          //Number of values in attribute
            (GLuint)_attributes[i].type,                   //Type
            _attributes[i].normalized ? GL_TRUE : GL_FALSE,
            _attributes[i].stride,                         //Stride 
            (void*)(_attributes[i].offset)                 //Vertex offset
        );
        Debug::Logger::ConsoleOpenGLError("During setting vertex attribute");
        offset += _attributes[i].count * AttributeSize(_attributes[i].type);
    }
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
    Debug::Logger::ConsoleOpenGLError("During generating vertex data buffer");
    glBindBuffer(GL_ARRAY_BUFFER, _bindingID);
    Debug::Logger::ConsoleOpenGLError("During setting buffer as vertex data buffer");

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
    Debug::Logger::ConsoleOpenGLError("During deletion of vertex data buffer");
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

    Debug::Logger::Console(Debug::Level::CONTEXT, "Setting context data buffer: %d", this->_bindingID);

    glBindBuffer(GL_ARRAY_BUFFER, this->_bindingID);
    Debug::Logger::ConsoleOpenGLError("During binding vertex data buffer");
}

void VertexDataBuffer::Write(unsigned int byte_size, void* data) {
    this->SetActive();

    if (this->_buffer_size > 0 && byte_size > this->_buffer_size)
        Controller::Instance()->ThrowException("Tried to write index array to buffer larger than initial capacity");

    //glBindBuffer(GL_ARRAY_BUFFER, this->_bindingID);
    //Debug::Logger::ConsoleOpenGLError("During binding vertex data buffer");
    glBufferData(GL_ARRAY_BUFFER, byte_size, data, this->_storage_type);
    Debug::Logger::ConsoleOpenGLError("During writing to vertex data buffer");
    Debug::Logger::Console(Debug::Level::WRITING, "Writing %d bytes of data buffer %d", byte_size, this->_bindingID);
}

void VertexDataBuffer::Draw()
{
    this->SetActive();
    //Todo
}

void VertexDataBuffer::Draw(int offset, int count)
{
    this->SetActive();
    //Todo
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
    Debug::Logger::ConsoleOpenGLError("During generating vertex index buffer");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bindingID);
    Debug::Logger::ConsoleOpenGLError("During setting buffer as vertex index buffer");
    Debug::Logger::Console(Debug::Level::CONSTRUCTION, "Created vertex index/element buffer at %d", this->_bindingID);
}

VertexIndexBuffer::~VertexIndexBuffer() {
    glDeleteBuffers(1, &_bindingID);
    Debug::Logger::ConsoleOpenGLError("During deletion of vertex index buffer");
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


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bindingID);
    Debug::Logger::ConsoleOpenGLError("During binding to vertex index buffer");
    Debug::Logger::Console(Debug::Level::CONTEXT, "Setting context index buffer: %d", _bindingID);
}

void VertexIndexBuffer::Write(unsigned int count, unsigned int* indicies) {
    this->SetActive();

    if (_initial_capacity > 0 && count > _initial_capacity)
        Controller::Instance()->ThrowException("Tried to write index array to buffer larger than initial capacity");

    Debug::Logger::Console(Debug::Level::WRITING, "Writing %d indicies into index buffer %d", count, this->_bindingID);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_bindingID);
    //Debug::Logger::ConsoleOpenGLError("During binding to vertex index buffer");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indicies, this->_storage_type);
    Debug::Logger::ConsoleOpenGLError("During writing to vertex index buffer");
    _initial_capacity = count;
}

void VertexIndexBuffer::Draw() {
    // draw points 0-3 from the currently bound VAO with current in-use shader
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    Debug::Logger::ConsoleOpenGLError("During drawing of vertex index buffer");
}

void VertexIndexBuffer::Draw(int offset, int count) {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)offset);
    Debug::Logger::ConsoleOpenGLError("During drawing of vertex index buffer");
}
