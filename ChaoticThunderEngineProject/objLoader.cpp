#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "objLoader.hpp"
#include "Controller.hpp"

void objLoad(const char* path, std::vector<float> &vertices, std::vector<unsigned int> &vertindices) {
	
	std::vector <unsigned int> uvindices, normindices;
	std::vector <glm::vec3> tempvert;
	std::vector <glm::vec2> tempuv;
	std::vector <glm::vec3> tempnormal;
	
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("No file found\n");
		exit(1);
	}

	while (1) {
		char header[100]; //first word of line
		int res = fscanf(file, "%s", header);
		if (res == EOF)
			break;		 //quits when file ends
		if (strncmp(header, "vn", 2) == 0) {
			glm::vec3 normvert;
			fscanf(file, "%f %f %f\n", &normvert.x, &normvert.y, &normvert.z);
			tempnormal.push_back(normvert);
		}//reads the normal vertices and pushes them into a temp vector
		else if (strncmp(header, "vt", 2) == 0) {
			glm::vec2 uvert;
			fscanf(file, "%f %f\n", &uvert.x, &uvert.y);
			tempuv.push_back(uvert);
		}//reads the UV vertices and pushes them into a temp vector
		else if (strncmp(header, "v", 1) == 0) {
			glm::vec3 geovert;
			fscanf(file, "%f %f %f\n", &geovert.x, &geovert.y, &geovert.z);
			tempvert.push_back(geovert);
		}//reads the geometric vertices and pushes them into a temp vector
		
		
		
		else if (strncmp(header, "f", 1) == 0) {
			unsigned int vertIndex[4], vertUV[4], vertNormal[4];
			int indices = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertIndex[0], &vertUV[0], &vertNormal[0], &vertIndex[1], &vertUV[1], &vertNormal[1], &vertIndex[2], &vertUV[2], &vertNormal[2], &vertIndex[3], &vertUV[3], &vertNormal[3]);
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
				uvindices.push_back(vertUV[1]);
				uvindices.push_back(vertUV[2]);
				uvindices.push_back(vertUV[3]);
				normindices.push_back(vertNormal[0]);
				normindices.push_back(vertNormal[1]);
				normindices.push_back(vertNormal[2]);
				normindices.push_back(vertNormal[1]);
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
	for (int i = 0; i < vertindices.size(); i++) {
		glm::vec3 vertex = tempvert[vertindices[i] - 1];
		glm::vec2 uv = tempuv[uvindices[i] - 1];
		glm::vec3 normal = tempnormal[normindices[i] - 1];
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);
		/*vertices.push_back(normal.x);
		vertices.push_back(normal.y);
		vertices.push_back(normal.z);
		vertices.push_back(uv.x);
		vertices.push_back(uv.y);*/
	}
}
	
