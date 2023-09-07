#ifndef ENGINE_STRUCTS
#define ENGINE_STRUCTS
#include <glad\glad.h>
#include <string>
#include <filesystem>
#include "Enums.hpp"
#include "Controller.hpp"

/// <summary>
/// load_shader contains the content used to read a shader and determine the type
/// opengl should compile to
/// </summary>
struct load_shader {
	shader_type type;
	std::filesystem::path path;

	/// <summary>
	/// Takes a directory path and iterates through the files of the directory
	/// If exactly one vertex (.vert) and fragment shader is found (.frag), then it
	/// is a valid program, other extensions such as tessellation (.tesc and .tese)
	/// and geometry (.geom) are also valid, but optional
	/// </summary>
	/// <param name="path">The directory to be iterated through</param>
	/// <returns>The ordered vector of shaders to create a program</returns>
	static std::vector<load_shader> LoadProgramFolder(std::filesystem::path path);

	static std::vector<std::pair<std::string, std::vector<load_shader>>> LoadProgramSubfolders(std::filesystem::path path);
};

struct attribute_setting {
	GLuint location;
	GLint count;
	attribute_type type;
	bool normalized;
};

struct vertex_attribute {
	bool normalized;
	GLuint location;
	GLint count;
	GLuint stride;
	attribute_type type;
	GLuint offset;
};

struct image {
	int width, height, channels;
	unsigned char* data;
	std::filesystem::path path;

	~image() {
		free(data);
	}
};
#endif