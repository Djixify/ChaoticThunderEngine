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

void VerifyShader(const Window& window, unsigned int shader_id) {
    window.SetActive();
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

Shader::Shader(Window& window, int count, load_shader path...) : _window(window) {
    _window.SetActive();
    
    unsigned int* shaders = new unsigned int[count];

    //Read each shader and compile them
    va_list folders;
    va_start(folders, count);
    for (int i = 0; i < count; i++) {
        load_shader shader = va_arg(folders, load_shader);
        const char* shadersource = ReadShaderProgram(shader.path, false);

        //Create and compile the shader
        unsigned int shader_id = glCreateShader((int)shader.type);
        glShaderSource(shader_id, 1, &shadersource, NULL);
        glCompileShader(shader_id);

        //Check for success
        VerifyShader(window, shader_id);

        //Add to be stored for later when we compile the entire program if all succeeded
        shaders[i] = shader_id;
    }
    va_end(folders);

    //Attach shaders to the program
    unsigned int program_id = glCreateProgram();
    for (int i = 0; i < count; i++)
        glAttachShader(program_id, shaders[i]);
    //Link them together to make up the entire program
    glLinkProgram(program_id);

    //Check for success
    VerifyShader(window, program_id);

    //Recover used resources, as the shaders are now built into the program
    for (int i = 0; i < count; i++)
        glDeleteShader(shaders[i]);

    delete[] shaders;

    this->_id = program_id;
}

Shader::~Shader() {
    glDeleteProgram(_id);
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
}

void Shader::SetUniformFloat(const std::string& name, float value)
{
    Controller::Instance()->ThrowException("Not implemented yet");
}

void Shader::SetUniformInteger(const std::string& name, int value)
{
    Controller::Instance()->ThrowException("Not implemented yet");
}

ArrayBuffer* Shader::AddArrayBuffer(const std::string& label)
{
    if (this->_buffermap.find(label) == this->_buffermap.end()) {
        this->_buffermap[label] = new ArrayBuffer(this->_window);
    }
    else {
        Controller::Instance()->ThrowException("Buffer label already exists");
    }
    return this->_buffermap[label];
}

ArrayBuffer* Shader::AddArrayBuffer(const char* label)
{
    std::string slabel(label);
    return this->AddArrayBuffer(slabel);
}

ArrayBuffer* Shader::GetArrayBuffer(const std::string& label)
{
    if (this->_buffermap.find(label) == this->_buffermap.end())
        return nullptr;
    return this->_buffermap[label];
}

ArrayBuffer* Shader::GetArrayBuffer(const char* label)
{
    std::string slabel(label);
    return this->GetArrayBuffer(slabel);
}

void Shader::RemoveArrayBuffer(const std::string& label) {
    ArrayBuffer* buffer = this->GetArrayBuffer(label);
    if (buffer != nullptr) {
        Debug::Logger::Console(Debug::Level::DESTRUCTION, "Destroyed label %s in shader", label);
        delete buffer;
        _buffermap.erase(label);
    }
    else
        Debug::Logger::Console(Debug::Level::DESTRUCTION, "Could not find array buffer by label %s in shader", label);
}

void Shader::RemoveArrayBuffer(const char* label)
{
    std::string slabel(label);
    this->RemoveArrayBuffer(slabel);
}

/*
//Buffer specifically for vertex attributes
//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
glBindBuffer(GL_ARRAY_BUFFER, b.data_addr);
glBufferData(GL_ARRAY_BUFFER, data_size, data, b.gl_data_storage_type);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.elems_addr);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, b.gl_elems_storage_type);
*/