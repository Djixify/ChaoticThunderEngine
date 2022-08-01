#include "Buffer.hpp"
#include "Window.hpp"

///////////////////////////////////////
///           Array buffer          ///
///////////////////////////////////////

ArrayBuffer::ArrayBuffer() {
    glGenVertexArrays(1, &_bindingID);
    Debug::Logger::ConsoleOpenGLError("During generation of array buffer");

    Debug::Logger::Console(Debug::Level::CONSTRUCTION, "Created vertex array buffer at %d", this->_bindingID);
}

ArrayBuffer::~ArrayBuffer() {
    _vbos.clear();

    glDeleteVertexArrays(1, &_bindingID);
    Debug::Logger::ConsoleOpenGLError("During destruction of array buffer");

    Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destoryed vertex array buffer at %d", this->_bindingID);
}

unsigned int ArrayBuffer::GetID() { return _bindingID; }

void ArrayBuffer::SetActive() {
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
    this->_vbos.push_back(std::shared_ptr<VertexDataBuffer>(new VertexDataBuffer(this, storage_type)));
    return this->_vbos.back().get();
}

VertexDataBuffer* ArrayBuffer::CreateVertexBuffer(size_t byte_size, void* data, buffer_storage_type storage_type) {
    this->_vbos.push_back(std::shared_ptr<VertexDataBuffer>(new VertexDataBuffer(this, storage_type)));
    this->_vbos.back()->Write(byte_size, data);
    return this->_vbos.back().get();
}


///////////////////////////////////////
///       Vertex data buffer        ///
///////////////////////////////////////

VertexDataBuffer::VertexDataBuffer(ArrayBuffer* parent, buffer_storage_type storage_type) : _parent(parent), _storage_type(storage_type) {
    this->_parent->SetActive();
    this->_buffer_size = 0;

    glGenBuffers(1, &_bindingID);
    Debug::Logger::ConsoleOpenGLError("During generating vertex data buffer");
    glBindBuffer(GL_ARRAY_BUFFER, _bindingID);
    Debug::Logger::ConsoleOpenGLError("During setting buffer as vertex data buffer");

    Debug::Logger::Console(Debug::Level::CONSTRUCTION, "Created vertex data buffer at id: %d", _bindingID);
}

VertexDataBuffer::~VertexDataBuffer() {
    _ebos.clear();

    glDeleteBuffers(1, &_bindingID);
    Debug::Logger::ConsoleOpenGLError("During deletion of vertex data buffer");
    Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destroyed vertex data buffer at %d", _bindingID);
}

unsigned int VertexDataBuffer::GetID() { return this->_bindingID; }

void VertexDataBuffer::SetActive() {
    _parent->SetActive();

    Debug::Logger::Console(Debug::Level::CONTEXT, "Setting context data buffer: %d", this->_bindingID);

    glBindBuffer(GL_ARRAY_BUFFER, this->_bindingID);
    Debug::Logger::ConsoleOpenGLError("During binding vertex data buffer");
}

void VertexDataBuffer::Write(size_t byte_size, void* data) {
    this->SetActive();

    glBufferData(GL_ARRAY_BUFFER, byte_size, data, (GLenum)this->_storage_type);
    Debug::Logger::ConsoleOpenGLError("During writing to vertex data buffer");
    Debug::Logger::Console(Debug::Level::WRITING, "Writing %d bytes of data buffer %d", byte_size, this->_bindingID);
    this->_buffer_size = byte_size;
}

VertexIndexBuffer* VertexDataBuffer::CreateIndexBuffer(buffer_storage_type storage_type) {
    this->_ebos.push_back(std::shared_ptr<VertexIndexBuffer>(new VertexIndexBuffer(this, storage_type)));
    return this->_ebos.back().get();
}

VertexIndexBuffer* VertexDataBuffer::CreateIndexBuffer(size_t count, unsigned int* indicies, buffer_storage_type storage_type) {
    this->_ebos.push_back(std::shared_ptr<VertexIndexBuffer>(new VertexIndexBuffer(this, storage_type)));
    this->_ebos.back()->Write(count, indicies);
    return this->_ebos.back().get();
}

///////////////////////////////////////
///       Vertex index buffer       ///
///////////////////////////////////////

VertexIndexBuffer::VertexIndexBuffer(VertexDataBuffer* parent, buffer_storage_type storage_type) : _parent(parent), _storage_type(storage_type), _buffer_size(0) {
    this->_parent->SetActive();

    glGenBuffers(1, &_bindingID);
    Debug::Logger::ConsoleOpenGLError("During generating vertex index buffer");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bindingID);
    Debug::Logger::ConsoleOpenGLError("During setting buffer as vertex index buffer");
    Debug::Logger::Console(Debug::Level::CONSTRUCTION, "Created vertex index/element buffer at %d", this->_bindingID);
}

VertexIndexBuffer::~VertexIndexBuffer() {
    try {
        glDeleteBuffers(1, &_bindingID);
        Debug::Logger::ConsoleOpenGLError("During deletion of vertex index buffer");
    } catch (std::runtime_error ex) { }
    Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destroyed vertex index/element buffer at %d", _bindingID);
}


unsigned int VertexIndexBuffer::GetID() { return _bindingID; }

void VertexIndexBuffer::SetActive() {
    this->_parent->SetActive();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bindingID);
    Debug::Logger::ConsoleOpenGLError("During binding to vertex index buffer");
    Debug::Logger::Console(Debug::Level::CONTEXT, "Setting context index buffer: %d", _bindingID);
}

void VertexIndexBuffer::Write(size_t count, unsigned int* indicies) {
    this->SetActive();

    Debug::Logger::Console(Debug::Level::WRITING, "Writing %d indicies into index buffer %d", count, this->_bindingID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indicies, (GLenum)this->_storage_type);
    Debug::Logger::ConsoleOpenGLError("During writing to vertex index buffer");
    this->_buffer_size = count;
}

void VertexIndexBuffer::Draw() {
    this->_parent->SetActive();

    glDrawElements(GL_TRIANGLES, this->_buffer_size, GL_UNSIGNED_INT, 0);
    Debug::Logger::ConsoleOpenGLError("During drawing of vertex index buffer");
}

void VertexIndexBuffer::Draw(int offset, int count) {
    this->_parent->SetActive();

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)offset);
    Debug::Logger::ConsoleOpenGLError("During drawing of vertex index buffer");
}
