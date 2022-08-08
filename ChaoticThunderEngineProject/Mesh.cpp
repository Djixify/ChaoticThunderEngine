#include "Mesh.hpp"


Mesh::Mesh() {
	_arraybuffer = new ArrayBuffer();
	_vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
	_vertexindexbuffer = _vertexdatabuffer->CreateIndexBuffer();
}

Mesh::Mesh(std::vector <float>& vertices, std::vector<unsigned int>& vertindices) {
	_arraybuffer = new ArrayBuffer();
	_vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
	_vertexindexbuffer = _vertexdatabuffer->CreateIndexBuffer();
	_vertexdatabuffer->Write(vertices.size() * sizeof(float), &vertices[0]);
	_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
	_arraybuffer->AddAttribute(1, 3, attribute_type::FLOAT32, false);
	_arraybuffer->AddAttribute(2, 2, attribute_type::FLOAT32, false);
	_vertexindexbuffer->Write(vertindices.size() * sizeof(unsigned int), &vertindices[0]);
}

Mesh::Mesh(std::vector <float>& vertices) {
    _arraybuffer = new ArrayBuffer();
    _vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
    _vertexdatabuffer->Write(vertices.size() * sizeof(float), &vertices[0]);
    _arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
    _arraybuffer->AddAttribute(1, 3, attribute_type::FLOAT32, false);
    _arraybuffer->AddAttribute(2, 2, attribute_type::FLOAT32, false);
}

Mesh:: ~Mesh() {
	delete _vertexindexbuffer;
	delete _vertexdatabuffer;
	delete _arraybuffer;
}

void Mesh::Draw() {
    if (_vertexindexbuffer != NULL)
        _vertexindexbuffer->Draw();
    else
        _vertexdatabuffer->Draw();
}


bool IsWhitespace(char c) {
    return c == ' ' || c == '\t';
}

bool IsWhitespaceOrNewline(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

bool IsDecimal(char c) {
    return c == '.' || IsDecimal(c); //ASCII 0-9 and .
}

bool IsInteger(char c) {
    return c >= 48 && c <= 57;
}

bool IsFaceComponent(char c) {
    return IsInteger(c) || "/";
}

bool SkipWhile(std::string& text, bool (*condition)(char), int& pointer) {
    int old_pointer = pointer;
    char curr_char = ' ';
    bool success = true;
    while (success && condition(curr_char)) {
        pointer++;
        success = text[pointer] != '\0' && pointer < text.size();
    }

    if (!success)
        pointer = old_pointer;
    return success;
}

bool ConsumeWhile(std::string& text, bool (*condition)(char), int& pointer, std::string& out_str) {
    int old_pointer = pointer;
    char curr_char = ' ';
    bool success = true;
    while (success && condition(curr_char)) {
        pointer++;
        success = text[pointer] != '\0' && pointer < text.size();
    }
    if (success)
        out_str = text.substr(old_pointer, pointer - old_pointer);
    else
        pointer = old_pointer;
    return success;
}

Mesh* Mesh::LoadObj(std::filesystem::path path) {
    //Do ObjLoad stuff here (body of the function)

    std::string content;
    std::ifstream fs(path.generic_string().c_str(), std::ios::in);

    if (!fs.is_open()) {
        Debug::Logger::Console(Debug::Level::WARNING, "Could not read file %s. File does not exist.", path);
        return 0;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texturecoords;
    std::vector<glm::ivec3> triangleindices;
    while (!fs.eof()) {
        int offset = 0;
        std::string line;
        std::getline(fs, line);
        if (std::strncmp(line.c_str(), "vn", 2) && SkipWhile(line, IsWhitespace, ++++offset)) {
            std::string out_x, out_y, out_z;
            if (ConsumeWhile(line, IsDecimal, offset, out_x)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_y)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_z)) {
                glm::vec3 normal;
                normal.x = std::stof(out_x);
                normal.y = std::stof(out_y);
                normal.z = std::stof(out_z);
                normals.push_back(normal);
            }
        }
        else if (std::strncmp(line.c_str(), "vt", 2) && SkipWhile(line, IsWhitespace, ++++offset)) {
            std::string out_u, out_v;
            if (ConsumeWhile(line, IsDecimal, offset, out_u)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_v)) {
                glm::vec2 texturecoord;
                texturecoord.x = std::stof(out_u);
                texturecoord.y = std::stof(out_v);
                texturecoords.push_back(texturecoord);
            }
        }
        else if (std::strncmp(line.c_str(), "v", 1) && SkipWhile(line, IsWhitespace, ++offset)) {
            std::string out_x, out_y, out_z;
            if (ConsumeWhile(line, IsDecimal, offset, out_x)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_y)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_z)) {
                glm::vec3 position;
                position.x = std::stof(out_x);
                position.y = std::stof(out_y);
                position.z = std::stof(out_z);
                positions.push_back(position);
            }
        }
        else if (std::strncmp(line.c_str(), "f", 1) && SkipWhile(line, IsWhitespace, ++offset)) {
            std::string out_comp;
            std::vector<glm::ivec3> facecomponents;
            while (ConsumeWhile(line, IsFaceComponent, offset, out_comp)) {
                glm::ivec3 facecomponent {-1,-1,-1};
                int component_offset = 0;
                int component_last_offset = 0;
                if ((component_offset = out_comp.find_first_of("/", component_last_offset)) != std::string::npos) {
                    facecomponent.x = std::stoi(out_comp.substr(component_last_offset, component_offset - component_last_offset));
                    component_last_offset = component_offset + 1;
                    //Format: "f v1/vt1 v2/vt2 v3/vt3 ..."
                    if (component_offset = out_comp.find_first_of("/", component_last_offset) && component_offset > component_last_offset) { 
                        facecomponent.y = std::stoi(out_comp.substr(component_last_offset, component_offset - component_last_offset));
                    }
                    component_last_offset = component_offset + 1;
                    //Format: "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ..."
                    if ((component_offset = out_comp.find_first_of("/", component_last_offset)) != std::string::npos) {
                        facecomponent.z = std::stoi(out_comp.substr(component_last_offset, component_offset - component_last_offset));
                    }
                } 
                else { //Format: "f v1 v2 v3 ..."
                    facecomponent.x = std::stoi(out_comp);
                }
                triangleindices.push_back(facecomponent);
            }
            int facecomponents_count = facecomponents.size();
            if (facecomponents_count < 3)
                Controller::Instance()->ThrowException("Not enough components found for face in .obj file, expected atleast 3 components");
            for (int i = 0; i < facecomponents_count - 2; i++) { //Assuming triangle fan for any face consisting of 4+ vertices
                triangleindices.push_back(facecomponents[0]);
                triangleindices.push_back(facecomponents[i+1]);
                triangleindices.push_back(facecomponents[i+2]);
            }
        }
        SkipWhile(line, IsWhitespaceOrNewline, offset);
    }

    fs.close();

    //TODO: create vertex data array (index array exists as positions vector)

    //Instanciate mesh, fill it (see other static methods for inspiration) with the data from the obj file and return it 
}