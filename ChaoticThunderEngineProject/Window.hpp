#ifndef ENGINE_WINDOW
#define ENGINE_WINDOW

#include <GLFW\glfw3.h>
#include <map>
#include <memory>
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Structs.hpp"
#include "Camera.hpp"
#include "Debug.hpp"

class Window {
private:
    GLFWwindow* _glfwwindow;

    int _activeshader;
    std::map<std::string, std::shared_ptr<Shader>> _shaders;

    std::map<std::string, std::shared_ptr<ArrayBuffer>> _buffers;

    int _activecamera;
    std::vector<std::shared_ptr<Camera>> _cameras;

    float _deltatime = 0.0f, _lastframetime = 0.0f;

    void ProcessKeyChanged(GLFWwindow* window, int key, int scancode, int action, int mods);
    void ProcessKeyContinuous();
    void ProcessMousePosition(GLFWwindow* window, double xpos, double ypos);
    void ProcessMouseKeyChanged(GLFWwindow* window, int button, int action, int mods);
    void ProcessMouseKeyContinuous(GLFWwindow* window, int button, int action, int mods);
    void ProcessMouseEnterLeave(GLFWwindow* window, int entered);
    void ProcessScrollWheel(GLFWwindow* window, double xoffset, double yoffset);
    void ProcessResize(GLFWwindow* window, int width, int height);

    friend class Shader;
    friend class ArrayBuffer;
public:
    glm::vec4 ClearColor;

    Window(std::string title, int width, int height, Window* other = nullptr);
    ~Window();

    void SetCursorInputType(Controls::cursor_input_type input_type);

    GLFWwindow* GetGLContext() const;
    void SetActive() const;
    void GetSize(int& width, int& height);

    void Clear();
    void UpdateUniforms();

    void AddShader(std::string shader_name, int count, load_shader shaders...);
    void RemoveShader(std::string shader_name);
    Shader* GetShader(std::string shader_name);
    std::vector<std::string> GetShaderLabels();

    ArrayBuffer* AddArrayBuffer(std::string label);
    ArrayBuffer* GetArrayBuffer(std::string label);
    void RemoveArrayBuffer(std::string label);

    Camera* GetActiveCamera();
};

#endif
