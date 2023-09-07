#ifndef ENGINE_CONTROLLER_MODULE
#define ENGINE_CONTROLLER_MODULE

#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <cassert>
#include <iostream>
#include "Exception.hpp"

class Window;

/// <summary>
/// The controller is a singleton class managing one or more window instances
/// </summary>
class Controller {
private:
	static Controller* _instance;
	Window** _windows = 0;
	int _window_capacity = 4;
	int _window_count = 0;
	int _context_window = -1;
	bool _hasloadedGLAD = false;

	Exception runtime_err;
public:
	/// <summary>
	/// Initializes the static instance of the controller and glfw to make code more readable
	/// </summary>
	static void Init();
	/// <summary>
	/// Returns a singleton instance of the class
	/// </summary>
	static Controller* Instance();
	/// <summary>
	/// Returns the current number of windows the controller holds
	/// </summary>
	int GetWindowCount();
	/// <summary>
	/// Returns the capacity for how many windows the controller currently support
	/// This can be expanded, see: SetWindowCapacity(int i)
	/// </summary>
	int GetWindowCapacity();
	/// <summary>
	/// Sets the current capacity of windows the controller maintains
	/// If lowering capacity, excess windows will be disposed of
	/// </summary>
	void SetWindowCapacity(int cap);
	/// <summary>
	/// Gets the window at the given index, this assumes a number between 0 to window count - 1
	/// </summary>
	Window* GetWindow(int i) const;
	/// <summary>
	/// Gets the index at which the window is stored at in the controller
	/// </summary>
	int GetWindowIndex(const Window* window);
	/// <summary>
	/// Gets the first window added to the controller
	/// </summary>
	Window* FirstWindow() const;
	/// <summary>
	/// Gets the most recently window added to the controller
	/// </summary>
	Window* LastWindow() const;
	/// <summary>
	/// Gets which window is currently being used to render in (changes with focus by default)
	/// </summary>
	Window* GetContextWindow() const;
	/// <summary>
	/// Gets which window index is currently being used to render in (changes with focus by default)
	/// </summary>
	int GetContextWindowIndex();
	/// <summary>
	/// Set which window should be used for rendering
	/// </summary>
	void SetContextWindow(int i);
	/// <summary>
	/// Stop rendering in any window (disassociates the windows)
	/// </summary>
	void ClearContextWindow();

	/// <summary>
	/// Add a window created outside of the controller
	/// </summary>
	void AddWindow(Window* w);
	/// <summary>
	/// Remove window at the given index
	/// </summary>
	void RemoveWindow(int i);
	/// <summary>
	/// Destroys and clears all windows in the controller
	/// </summary>
	void ClearWindows();

	/// <summary>
	/// Gets the number of allowed vertex attributes provided by opengl
	/// this should always be
	/// </summary>
	/// <returns></returns>
	int GetMaxNumberOfVertexAttributes();

	/// <summary>
	/// Throws an exception that can be inspected by using .what()
	/// </summary>
	/// <param name="msg">Message to write to the instance</param>
	void ThrowException(std::string msg) {
		throw* this->runtime_err.SetMessage(msg);
	}

	/// <summary>
	/// Dispose of windows and the controller
	/// </summary>
	void Dispose();
};
#endif