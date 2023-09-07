#include "Mesh.hpp"
#include "MeshCollection.hpp"
#include "Material.hpp"
#include "File.hpp"
#include "Texture.hpp"

Mesh::Mesh() {
	_arraybuffer = new ArrayBuffer();
	_vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
	_vertexindexbuffer = _vertexdatabuffer->CreateIndexBuffer();
}

Mesh::Mesh(std::vector<float>& vertices, std::vector<attribute_setting> attrs, std::filesystem::path materialpath) {
    _arraybuffer = new ArrayBuffer();
    _vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
    _vertexindexbuffer = nullptr;
    _vertexdatabuffer->Write(vertices.size() * sizeof(float), &vertices[0]);
    for (int i = 0; i < attrs.size(); i++)
        _arraybuffer->AddAttribute(attrs[i].location, attrs[i].count, attrs[i].type, attrs[i].normalized);
}

Mesh::Mesh(std::vector <float>& vertices, std::vector<unsigned int>& vertindices, std::vector<attribute_setting> attrs, std::filesystem::path materialpath) {
	_arraybuffer = new ArrayBuffer();
	_vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
	_vertexindexbuffer = _vertexdatabuffer->CreateIndexBuffer();
	_vertexdatabuffer->Write(vertices.size() * sizeof(float), &vertices[0]);
    for (int i = 0; i < attrs.size(); i++)
        _arraybuffer->AddAttribute(attrs[i].location, attrs[i].count, attrs[i].type, attrs[i].normalized);
	_vertexindexbuffer->Write(vertindices.size() * sizeof(unsigned int), &vertindices[0]);
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

bool IsNewline(char c) {
    return c == '\n' || c == '\r';
}


bool IsWhitespaceOrNewline(char c) {
    return IsWhitespace(c) || IsNewline(c);
}

bool IsInteger(char c) {
    return c == '-' || (c >= 48 && c <= 57);
}

bool IsDecimal(char c) {
    return c == '.' || IsInteger(c); //ASCII 0-9 and .
}

bool IsFaceComponent(char c) {
    return IsInteger(c) || c == '/';
}

bool SkipWhile(std::string& text, bool (*condition)(char), int& pointer) {
    int old_pointer = pointer;
    if (!condition(text[pointer]))
        return false;
    do {
        pointer++;
    } while (text[pointer] != '\0' && pointer < text.size() && condition(text[pointer]));
    return true;
}

bool ConsumeWhile(std::string& text, bool (*condition)(char), int& pointer, std::string& out_str, bool inverted = false) {
    int old_pointer = pointer;
    if (!(condition(text[pointer]) ^ inverted))
        return false;
    do {
        pointer++;
    } while (text[pointer] != '\0' && pointer < text.size() && (condition(text[pointer]) ^ inverted));
    out_str = text.substr(old_pointer, pointer - old_pointer);
    return true;
}

bool ConsumeKeyword(std::string& text, std::string keyword, int& pointer) {
    int old_pointer = pointer;
    int size = keyword.size();
    bool success = true;
    for (int i = 0; i < size && success; i++, pointer++) {
        if (text[pointer] != keyword[i])
            success = false;
    }
    if (!success) {
        pointer = old_pointer;
    }
    return success;
}

Mesh* SpawnMeshFromObject(std::string objectname, std::string materialfile, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texturecoords, std::vector<glm::ivec3> triangleindices) {
    //TODO: create vertex data array (index array exists as positions vector)
    bool has_normals = triangleindices.back().z > -1;
    bool has_uvs = triangleindices.back().y > -1;
    int vertex_size = 3 + (has_normals ? 3 : 0) + (has_uvs ? 2 : 0);
    int vertices_size = triangleindices.size() * vertex_size;
    std::vector<float> vertices(vertices_size);
    for (int i = 0; i < triangleindices.size(); i++) {
        glm::ivec3 triangle = triangleindices[i];
        int offset = i * vertex_size;
        int current_pointer = 0;
        glm::vec3 position = positions[triangle.x - 1];
        vertices[offset + current_pointer++] = position.x;
        vertices[offset + current_pointer++] = position.y;
        vertices[offset + current_pointer++] = position.z;
        if (has_uvs) {
            glm::vec2 uv = texturecoords[triangle.y - 1];
            vertices[offset + current_pointer++] = uv.x;
            vertices[offset + current_pointer++] = uv.y;
        }
        if (has_normals) {
            glm::vec3 normal = normals[triangle.z - 1];
            vertices[offset + current_pointer++] = normal.x;
            vertices[offset + current_pointer++] = normal.y;
            vertices[offset + current_pointer++] = normal.z;
        }
    }

    std::vector<attribute_setting> attributesettings;
    attributesettings.push_back({ 0, 3, attribute_type::FLOAT32, false });
    if (has_uvs)
        attributesettings.push_back({ 1, 2, attribute_type::FLOAT32, false });
    if (has_normals)
        attributesettings.push_back({ has_uvs ? 2U : 1U, 3, attribute_type::FLOAT32, false });

    //Instanciate mesh, fill it (see other static methods for inspiration) with the data from the obj file and return it
    Mesh* mesh = new Mesh(vertices, attributesettings, materialfile);
    mesh->Name = objectname;
    return mesh;
}


void LoadMTL(std::map<std::string, Material*> materials, std::filesystem::path path) {
    std::string content;
    std::ifstream fs(path.generic_string().c_str(), std::ios::in);

    if (materials.find("default") == materials.end())
        materials["default"] = new Material();

    if (!fs.is_open()) {
        Debug::Logger::Console(Debug::Level::WARNING, "Could not read file %s. File does not exist.", path);
        return;
    }

    std::string currentmaterial = "default";
    while (fs.good() && !fs.eof()) {
        int offset = 0;
        std::string line;
        std::getline(fs, line);
        if (line[0] == '#' || line.size() < 3)
            continue;
        if (ConsumeKeyword(line, "newmtl", offset) && SkipWhile(line, IsWhitespace, offset)) {
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, currentmaterial, true)) {
                materials[currentmaterial] = new Material();
            }
        }
        else if (ConsumeKeyword(line, "Ns", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string tmp;
            if (ConsumeWhile(line, IsDecimal, offset, tmp)) {
                materials[currentmaterial]->specular_highlight = std::stof(tmp);
            }
        }
        else if (ConsumeKeyword(line, "Ni", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string tmp;
            if (ConsumeWhile(line, IsDecimal, offset, tmp)) {
                materials[currentmaterial]->optical_density = std::stof(tmp);
            }
        }
        else if (ConsumeKeyword(line, "d", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string tmp;
            if (ConsumeWhile(line, IsDecimal, offset, tmp)) {
                materials[currentmaterial]->dissolve = std::stof(tmp);
            }
        }
        else if (ConsumeKeyword(line, "illum", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string tmp;
            if (ConsumeWhile(line, IsInteger, offset, tmp)) {
                materials[currentmaterial]->illum = std::stof(tmp);
            }
        }
        else if (ConsumeKeyword(line, "sharpness", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string tmp;
            if (ConsumeWhile(line, IsDecimal, offset, tmp)) {
                materials[currentmaterial]->sharpness = std::stof(tmp);
            }
        }
        else if (ConsumeKeyword(line, "map_Bump", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string normaltexturepath;
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, normaltexturepath, true)) {
                std::filesystem::path path(normaltexturepath);
                std::filesystem::path texturepath = path.parent_path() / normaltexturepath;
                materials[currentmaterial]->textures["normal_texture"] = new Texture(texturepath, false);
            }
        }
        else if (ConsumeKeyword(line, "map_Ka", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string ambienttexturepath;
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, ambienttexturepath, true)) {
                std::filesystem::path path(ambienttexturepath);
                std::filesystem::path texturepath = path.parent_path() / ambienttexturepath;
                materials[currentmaterial]->textures["ambient_texture"] = new Texture(texturepath, false);
            }
        }
        else if (ConsumeKeyword(line, "map_Kd", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string diffusetexturepath;
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, diffusetexturepath, true)) {
                std::filesystem::path path(diffusetexturepath);
                std::filesystem::path texturepath = path.parent_path() / diffusetexturepath;
                materials[currentmaterial]->textures["diffuse_texture"] = new Texture(texturepath, false);
            }
        }
        else if (ConsumeKeyword(line, "map_Ks", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string speculartexturepath;
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, speculartexturepath, true)) {
                std::filesystem::path path(speculartexturepath);
                std::filesystem::path texturepath = path.parent_path() / speculartexturepath;
                materials[currentmaterial]->textures["specular_texture"] = new Texture(texturepath, false);
            }
        }
        else if (ConsumeKeyword(line, "map_Ks", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string speculartexturepath;
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, speculartexturepath, true)) {
                std::filesystem::path path(speculartexturepath);
                std::filesystem::path texturepath = path.parent_path() / speculartexturepath;
                materials[currentmaterial]->textures["specular_texture"] = new Texture(texturepath, false);
            }
        }
        else if (ConsumeKeyword(line, "map_Ns", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string specularhighlighttexturepath;
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, specularhighlighttexturepath, true)) {
                std::filesystem::path path(specularhighlighttexturepath);
                std::filesystem::path texturepath = path.parent_path() / specularhighlighttexturepath;
                materials[currentmaterial]->textures["specular_highlight_texture"] = new Texture(texturepath, false);
            }
        }
        else if (ConsumeKeyword(line, "refl", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string reflectivitytexturepath;
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, reflectivitytexturepath, true)) {
                std::filesystem::path path(reflectivitytexturepath);
                std::filesystem::path texturepath = path.parent_path() / reflectivitytexturepath;
                materials[currentmaterial]->textures["reflectivity_texture"] = new Texture(texturepath, false);
            }
        }
        else if (ConsumeKeyword(line, "Ka", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string out_r, out_g, out_b;
            if (ConsumeWhile(line, IsDecimal, offset, out_r)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_g)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_b)) {
                glm::vec3 ambient;
                materials[currentmaterial]->ambient_color.r = std::stof(out_r);
                materials[currentmaterial]->ambient_color.g = std::stof(out_g);
                materials[currentmaterial]->ambient_color.b = std::stof(out_b);
            }
        }
        else if (ConsumeKeyword(line, "Kd", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string out_r, out_g, out_b;
            if (ConsumeWhile(line, IsDecimal, offset, out_r)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_g)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_b)) {
                glm::vec3 ambient;
                materials[currentmaterial]->diffuse_color.r = std::stof(out_r);
                materials[currentmaterial]->diffuse_color.g = std::stof(out_g);
                materials[currentmaterial]->diffuse_color.b = std::stof(out_b);
            }
        }
        else if (ConsumeKeyword(line, "Ks", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string out_r, out_g, out_b;
            if (ConsumeWhile(line, IsDecimal, offset, out_r)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_g)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_b)) {
                glm::vec3 ambient;
                materials[currentmaterial]->specular_color.r = std::stof(out_r);
                materials[currentmaterial]->specular_color.g = std::stof(out_g);
                materials[currentmaterial]->specular_color.b = std::stof(out_b);
            }
        }
        else if (ConsumeKeyword(line, "Ke", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string out_r, out_g, out_b;
            if (ConsumeWhile(line, IsDecimal, offset, out_r)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_g)
                && SkipWhile(line, IsWhitespace, offset)
                && ConsumeWhile(line, IsDecimal, offset, out_b)) {
                glm::vec3 ambient;
                materials[currentmaterial]->emissive_color.r = std::stof(out_r);
                materials[currentmaterial]->emissive_color.g = std::stof(out_g);
                materials[currentmaterial]->emissive_color.b = std::stof(out_b);
            }
        }
    }
    fs.close();

    if (!fs.eof())
        Controller::Instance()->ThrowException("Something went wrong with file at path: " + path.generic_string());
}

MeshCollection* Mesh::LoadObj(std::filesystem::path path) {
    //Do ObjLoad stuff here (body of the function)

    std::string content;
    std::ifstream fs(path.generic_string().c_str(), std::ios::in);

    if (!fs.is_open()) {
        Debug::Logger::Console(Debug::Level::WARNING, "Could not read file %s. File does not exist.", path);
        return 0;
    }

    MeshCollection* meshes = new MeshCollection();

    int object_count = 0;

    std::string objectname = "default";
    std::string materialfile = "default";
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texturecoords;
    std::vector<glm::ivec3> triangleindices;
    std::map<std::string, Material*> materials;
    
    while (fs.good() && !fs.eof()) {
        int offset = 0;
        std::string line;
        std::getline(fs, line);
        if (line[0] == '#' || line.size() < 3)
            continue;
        if (ConsumeKeyword(line, "mtllib", offset) && SkipWhile(line, IsWhitespace, offset)) {
            if (!ConsumeWhile(line, IsWhitespaceOrNewline, offset, materialfile, true)) {
                materialfile = nullptr;
            }
            else {
                LoadMTL(materials, path.parent_path() / materialfile);
            }
        } //TODO: usemtl
        else if (ConsumeKeyword(line, "o", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string tmp;
            if (ConsumeWhile(line, IsWhitespaceOrNewline, offset, tmp, true)) {
                if (triangleindices.size() > 0) { //We had buffered data, store that to the current object and ready for new one
                    meshes->Add(SpawnMeshFromObject(objectname, materialfile, positions, normals, texturecoords, triangleindices));
                    positions.clear();
                    normals.clear();
                    texturecoords.clear();
                    triangleindices.clear();
                }
                objectname = tmp;
            }
        }
        else if (ConsumeKeyword(line, "vn", offset) && SkipWhile(line, IsWhitespace, offset)) {
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
        else if (ConsumeKeyword(line, "vt", offset) && SkipWhile(line, IsWhitespace, offset)) {
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
        else if (ConsumeKeyword(line, "v", offset) && SkipWhile(line, IsWhitespace, offset)) {
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
        else if (ConsumeKeyword(line, "f", offset) && SkipWhile(line, IsWhitespace, offset)) {
            std::string out_comp;
            std::vector<glm::ivec3> facecomponents;
            while (ConsumeWhile(line, IsFaceComponent, offset, out_comp)) {
                glm::ivec3 facecomponent {-1,-1,-1};
                int component_offset = 0;
                int component_last_offset = 0;
                if ((component_offset = out_comp.find_first_of("/", component_last_offset)) != std::string::npos) {
                    facecomponent.x = std::stoi(out_comp.substr(component_last_offset, component_offset - component_last_offset));
                    component_last_offset = component_offset+1;
                    //Format: "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ..."
                    if ((component_offset = out_comp.find_first_of("/", component_last_offset)) != std::string::npos) {
                        if (component_offset > component_last_offset) //If fails, do "f v1//vn1 v2//vn2 v3//vn3"
                            facecomponent.y = std::stoi(out_comp.substr(component_last_offset, component_offset - component_last_offset));
                        component_offset++;
                        facecomponent.z = std::stoi(out_comp.substr(component_offset, out_comp.size() - component_offset));
                    }
                    else {//Format: "f v1/vt1 v2/vt2 v3/vt3 ..."
                        facecomponent.y = std::stoi(out_comp.substr(component_last_offset));
                    }
                } 
                else { //Format: "f v1 v2 v3 ..."
                    facecomponent.x = std::stoi(out_comp);
                }
                facecomponents.push_back(facecomponent);
                SkipWhile(line, IsWhitespace, offset);
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
    }
    fs.close();

    if (!fs.eof())
        Controller::Instance()->ThrowException("Something went wrong with file at path: " + path.generic_string());

    //Flush remaining data to the latest object:
    if (triangleindices.size() > 0) { //We had buffered data, store that to the current object and ready for new one
        meshes->Add(SpawnMeshFromObject(objectname, materialfile, positions, normals, texturecoords, triangleindices));
        positions.clear();
        normals.clear();
        texturecoords.clear();
        triangleindices.clear();
    }


    return meshes;
}
