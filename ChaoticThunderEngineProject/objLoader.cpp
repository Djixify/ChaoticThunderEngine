#include "ObjLoader.hpp"
#include "Controller.hpp"
#include "Debug.hpp"
#include <fstream>

void ObjLoad(const char* path, std::vector<float> & vertices, std::vector<unsigned int> &vertindices) {
	
	std::vector <unsigned int> uvindices, normindices;
	std::vector <glm::vec3> tempvert;
	std::vector <glm::vec2> tempuv;
	std::vector <glm::vec3> tempnormal;
	
	FILE* file;
	fopen_s(&file, path, "r");

	if (file == NULL) {
		printf("No file found\n");
		exit(1);
	}

	int misses = 0;
	while (1) {
		char header[100]; //first word of line
		int res = fscanf_s(file, "%s", header);
		if (res == EOF)
			break;		 //quits when file ends

		//reads the normal vertices and pushes them into a temp vector
		if (strncmp(header, "vn", 2) == 0) { 
			glm::vec3 normvert;
			fscanf_s(file, "%f %f %f\n", &normvert.x, &normvert.y, &normvert.z);
			tempnormal.push_back(normvert);
		}//reads the UV vertices and pushes them into a temp vector
		else if (strncmp(header, "vt", 2) == 0) {
			glm::vec2 uvert;
			fscanf_s(file, "%f %f\n", &uvert.x, &uvert.y);
			tempuv.push_back(uvert);
		}//reads the geometric vertices and pushes them into a temp vector
		else if (strncmp(header, "v", 1) == 0) {
			glm::vec3 geovert;
			int matches = fscanf_s(file, "%f %f %f\n", &geovert.x, &geovert.y, &geovert.z);
			if (matches != 0)
				tempvert.push_back(geovert);
			else misses++;
		}
		else if (strncmp(header, "f", 1) == 0) {
			unsigned int vertIndex[4], vertUV[4], vertNormal[4];
			int indices = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertIndex[0], &vertUV[0], &vertNormal[0], &vertIndex[1], &vertUV[1], &vertNormal[1], &vertIndex[2], &vertUV[2], &vertNormal[2], &vertIndex[3], &vertUV[3], &vertNormal[3]);
			if (indices == 9) {
				vertindices.push_back(vertIndex[0]);
				vertindices.push_back(vertIndex[1]);
				vertindices.push_back(vertIndex[2]);
				uvindices.push_back(vertUV[0]);
				uvindices.push_back(vertUV[1]);
				uvindices.push_back(vertUV[2]);
				normindices.push_back(vertNormal[0]);
				normindices.push_back(vertNormal[1]);
				normindices.push_back(vertNormal[2]);
				

			}
			else if (indices == 12) {
				vertindices.push_back(vertIndex[0]);
				vertindices.push_back(vertIndex[1]);
				vertindices.push_back(vertIndex[2]);
				vertindices.push_back(vertIndex[0]);
				vertindices.push_back(vertIndex[2]);
				vertindices.push_back(vertIndex[3]);
				uvindices.push_back(vertUV[0]);
				uvindices.push_back(vertUV[1]);
				uvindices.push_back(vertUV[2]);
				uvindices.push_back(vertUV[0]);
				uvindices.push_back(vertUV[2]);
				uvindices.push_back(vertUV[3]);
				normindices.push_back(vertNormal[0]);
				normindices.push_back(vertNormal[1]);
				normindices.push_back(vertNormal[2]);
				normindices.push_back(vertNormal[0]);
				normindices.push_back(vertNormal[2]);
				normindices.push_back(vertNormal[3]);
			}
			else {
				printf("Not a standard .obj file, cannot read\n");
				exit(1);
			}
		}

	}

	if (vertindices.size() != normindices.size() || vertindices.size() != uvindices.size())
		Controller::Instance()->ThrowException("Vectors not of equal size");

	vertices.resize(tempvert.size() * 8);
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] = 4.0f;
	}
	for (int i = 0; i < vertindices.size(); i++) {
		glm::vec3 vertex = tempvert[vertindices[i] - 1];
		glm::vec2 uv = tempuv[uvindices[i] - 1];
		glm::vec3 normal = tempnormal[normindices[i] - 1];
		int offset = (vertindices[i] - 1) * 8;
		vertices[offset + 0] = vertex.x;
		vertices[offset + 1] = vertex.y;
		vertices[offset + 2] = vertex.z;
		vertices[offset + 3] = normal.x;
		vertices[offset + 4] = normal.y;
		vertices[offset + 5] = normal.z;
		vertices[offset + 6] = uv.x;
		vertices[offset + 7] = uv.y;
	}
}