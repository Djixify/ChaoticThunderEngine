#ifndef ENGINE_GRAPHICS_UTILITY
#define ENGINE_GRAPHICS_UTILITY
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW\glfw3.h>
#include <glad\glad.h>

namespace Graphics {
    void InitializeImGUI(GLFWwindow* window);
    void ClearWindow(GLFWwindow* window);
    void RenderImGUI(GLFWwindow* window);
}

#endif