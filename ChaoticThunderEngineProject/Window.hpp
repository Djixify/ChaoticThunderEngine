#ifndef ENGINE_WINDOW
#define ENGINE_WINDOW

#include <GLFW\glfw3.h>
#include <map>
#include <memory>
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Structs.hpp"

class Window {
private:
    GLFWwindow* _glfwwindow;
    std::map<std::string, std::shared_ptr<Shader>> _shaders;
    std::map<std::string, std::shared_ptr<ArrayBuffer>> _buffers;
    friend class Shader;
public:
    Window(std::string title, int width, int height, Window* other = nullptr);
    ~Window();
    void AddShader(std::string shader_name, int count, load_shader shaders...);
    void RemoveShader(std::string shader_name);
    Shader* GetShader(std::string shader_name);
    std::vector<std::string> GetShaderLabels();
    GLFWwindow* GetGLContext() const;
    void SetActive() const;

    ArrayBuffer* AddArrayBuffer(std::string label);
    ArrayBuffer* GetArrayBuffer(std::string label);
    void RemoveArrayBuffer(std::string label);
};

#endif
