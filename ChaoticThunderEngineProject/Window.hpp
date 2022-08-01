#ifndef ENGINE_WINDOW
#define ENGINE_WINDOW

#include <GLFW\glfw3.h>
#include <map>
#include <memory>
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Structs.hpp"
#include "BaseCamera.hpp"
#include "Debug.hpp"

class Window {
private:
    //The GLFW context window that is obfuscated by this class, accessible if necessay
    //using GetGLContext()
    GLFWwindow* _glfwwindow;

    //Shader related values
    int _activeshader;
    std::map<std::string, std::shared_ptr<Shader>> _shaders;

    //Camera related variables
    int _activecamera;
    std::vector<std::shared_ptr<BaseCamera>> _cameras;

    //Variables that change fundamental rendering
    bool _environment_fill_mesh;

    //Time related variables
    clock_t _start_time; 
    float _delta_time;
    float _current_time;

    //Mouse related variables
    glm::vec2 _prev_pos;
    bool _invalid_prev_pos;

    friend class Shader;
    friend class ArrayBuffer;
public:
    void ProcessKeyChanged(GLFWwindow* window, int key, int scancode, int action, int mods);
    void ProcessKeyContinuous();
    void ProcessMousePosition(GLFWwindow* window, double xpos, double ypos);
    void ProcessMouseKeyChanged(GLFWwindow* window, int button, int action, int mods);
    void ProcessMouseKeyContinuous();
    void ProcessMouseEnterLeave(GLFWwindow* window, bool entered);
    void ProcessScrollWheel(GLFWwindow* window, double xoffset, double yoffset);
    void ProcessResize(GLFWwindow* window, int width, int height);

    glm::vec4 ClearColor;

    Window(std::string title, int width, int height, Window* other = nullptr);
    ~Window();

    void SetCursorInputType(Controls::cursor_input_type input_type);

    GLFWwindow* GetGLContext() const;
    void SetActive() const;
    void GetSize(int& width, int& height);
    float GetAspectRatio();

    glm::vec2 GetMousePos();

    float UpdateTime();
    float GetCurrentTimeSec();
    float GetDeltaTimeSec();
    void Clear();

    void AddShader(std::string shader_name, std::vector<load_shader> shaders);
    void AddShader(std::string shader_name, int count, load_shader shaders...);
    void RemoveShader(std::string shader_name);
    Shader* GetShader(std::string shader_name);
    std::vector<std::string> GetShaderLabels();

    BaseCamera* GetActiveCamera();
};

#endif
