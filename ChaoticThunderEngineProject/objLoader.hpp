#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include <vector>
#include <glm.hpp>
#include <filesystem>

void ObjLoad(std::filesystem::path path, std::vector <float>& vertices);
bool ObjLoad2(std::filesystem::path path, std::vector<float>& vertices, std::vector<unsigned int>& indices);