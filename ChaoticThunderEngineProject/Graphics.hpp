#ifndef ENGINE_GRAPHICS_UTILITY
#define ENGINE_GRAPHICS_UTILITY
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW\glfw3.h>
#include <glad\glad.h>
#include "Window.hpp"

namespace Graphics {
	void InitializeImGUI(Window* window);
	void UpdateVariablesImGUI(Window* window);
	void ClearWindow(Window* window);
	void RenderImGUI(Window* window);
	bool ShouldRenderNormals();
	void TerminateImGUI();
}

#endif