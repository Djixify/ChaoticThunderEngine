#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include <vector>
#include <glm.hpp>
#include <filesystem>

void ObjLoad(const char* path, std::vector <float>& vertices, std::vector<unsigned int>& vertindices);
bool ObjLoad2(std::filesystem::path path, std::vector<float>& vertices, std::vector<unsigned int>& indices);