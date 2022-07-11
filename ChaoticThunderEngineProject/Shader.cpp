#include <cstdarg>
#include "Shader.hpp"
#include "File.hpp"
#include "Controller.hpp"
#include "Buffer.hpp"


const char* ReadShaderProgram(const std::string path, bool debug_print) {
    std::string content;
    std::ifstream fileStream(path, std::ios::in);

    if (!fileStream.is_open()) {
        Debug::Logger::Console(Debug::Level::WARNING, "Could not read file %s. File does not exist.", path);
        return 0;
    }

    std::string line;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    if (debug_print)
        std::cout << std::endl << std::endl << content << std::endl << std::endl;

    char* res = new char[content.size() + 1];
    strcpy_s(res, content.size() + 1, content.c_str());
    return res;
}

void VerifyShader(const Window* window, unsigned int shader_id) {
    window->SetActive();
    //Check for success
    int  success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        std::string log = "Failed to compile shader:\n";
        log.append(infoLog);
        log.append("\n");
        Controller::Instance()->ThrowException(log);
    }
}

void VerifyProgram(const Window* window, unsigned int program_id) {
    window->SetActive();
    //Check for success
    int  success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program_id, 512, NULL, infoLog);
        std::string log = "Failed to link program:\n";
        log.append(infoLog);
        log.append("\n");
        Controller::Instance()->ThrowException(log);
    }
}

Shader::Shader(Window* window, int count, load_shader path...) : _window(window) {
    _window->SetActive();
    
    unsigned int* shaders = new unsigned int[count];

    //Read each shader and compile them
    va_list folders;
    va_start(folders, count);
    for (int i = 0; i < count; i++) {
        load_shader shader = va_arg(folders, load_shader);
        const char* shadersource = ReadShaderProgram(shader.path, false);

        //Create and compile the shader
        unsigned int shader_id = glCreateShader((int)shader.type);
        Debug::Logger::ConsoleOpenGLError("During creation of shader");
        glShaderSource(shader_id, 1, &shadersource, NULL);
        Debug::Logger::ConsoleOpenGLError("During setting shader source");
        glCompileShader(shader_id);
        Debug::Logger::ConsoleOpenGLError("During compilation of shader");

        //Check for success
        VerifyShader(window, shader_id);

        //Add to be stored for later when we compile the entire program if all succeeded
        shaders[i] = shader_id;
    }
    va_end(folders);

    //Attach shaders to the program
    unsigned int program_id = glCreateProgram();
    Debug::Logger::ConsoleOpenGLError("During creation of program");
    for (int i = 0; i < count; i++) {
        glAttachShader(program_id, shaders[i]);
        Debug::Logger::ConsoleOpenGLError("During attaching shader to program");
    }
    //Link them together to make up the entire program
    glLinkProgram(program_id);
    Debug::Logger::ConsoleOpenGLError("During linking shaders in program");

    //Check for success
    VerifyProgram(window, program_id);

    //Recover used resources, as the shaders are now built into the program
    for (int i = 0; i < count; i++) {
        glDeleteShader(shaders[i]);
        Debug::Logger::ConsoleOpenGLError("During deleletion of shader");
    }

    delete[] shaders;

    this->_id = program_id;
}

Shader::~Shader() {
    glDeleteProgram(_id);
    Debug::Logger::ConsoleOpenGLError("During deleletion of program");
    _buffermap.clear();
    //delete this;
}

unsigned int Shader::GetID() const
{
    return this->_id;
}

void Shader::Use()
{
    glUseProgram(this->GetID());
    Debug::Logger::ConsoleOpenGLError("During setting program to active");
}

bool Shader::SetUniform(std::string name, float value1)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform1f(loc, value1);
    return res;
}

bool Shader::SetUniform(std::string name, float value1, float value2)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform2f(loc, value1, value2);
    return res;
}

bool Shader::SetUniform(std::string name, float value1, float value2, float value3)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform3f(loc, value1, value2, value3);
    return res;
}

bool Shader::SetUniform(std::string name, float value1, float value2, float value3, float value4)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform4f(loc, value1, value2, value3, value4);
    return res;
}

bool Shader::SetUniform(std::string name, int value1)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform1i(loc, value1);
    return res;
}

bool Shader::SetUniform(std::string name, int value1, int value2)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform2i(loc, value1, value2);
    return res;
}

bool Shader::SetUniform(std::string name, int value1, int value2, int value3)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform3i(loc, value1, value2, value3);
    return res;
}

bool Shader::SetUniform(std::string name, int value1, int value2, int value3, int value4)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform4i(loc, value1, value2, value3, value4);
    return res;
}

bool Shader::SetUniform(std::string name, unsigned int value1)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform1ui(loc, value1);
    return res;
}

bool Shader::SetUniform(std::string name, unsigned int value1, unsigned int value2)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform2ui(loc, value1, value2);
    return res;
}

bool Shader::SetUniform(std::string name, unsigned int value1, unsigned int value2, unsigned int value3)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform3ui(loc, value1, value2, value3);
    return res;
}

bool Shader::SetUniform(std::string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
{
    int loc = glGetUniformLocation(_id, name.c_str());
    bool res = loc > -1;
    if (res)
        glUniform4ui(loc, value1, value2, value3, value4);
    return res;
}

ArrayBuffer* Shader::AddArrayBuffer(std::string label)
{
    if (this->_buffermap.find(label) == this->_buffermap.end()) {
        this->_buffermap[label] = new ArrayBuffer(this->_window);
    }
    else {
        Controller::Instance()->ThrowException("Buffer label already exists");
    }
    return this->_buffermap[label];
}

ArrayBuffer* Shader::GetArrayBuffer(std::string label)
{
    if (this->_buffermap.find(label) == this->_buffermap.end())
        return nullptr;
    return this->_buffermap[label];
}

void Shader::RemoveArrayBuffer(std::string label) {
    ArrayBuffer* buffer = this->GetArrayBuffer(label);
    if (buffer != nullptr) {
        Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destroyed label %s in shader", label);
        delete buffer;
        _buffermap.erase(label);
    }
    else
        Debug::Logger::Console(Debug::Level::DESTRUCTION, "Could not find array buffer by label %s in shader", label);
}

/*
//Buffer specifically for vertex attributes
//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
glBindBuffer(GL_ARRAY_BUFFER, b.data_addr);
glBufferData(GL_ARRAY_BUFFER, data_size, data, b.gl_data_storage_type);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.elems_addr);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, b.gl_elems_storage_type);
*/