#include "Material.hpp"
#include "Texture.hpp"

Material::Material() :
    specular_highlight(360.f),
    optical_density(1.f),
    dissolve(1.f),
    sharpness(60.f),
    illum(0),
    ambient_color(glm::vec3(0.8f)),
    diffuse_color(glm::vec3(1.f)),
    specular_color(glm::vec3(1.f)),
    emissive_color(glm::vec3(0.f)),
    transmission_color(glm::vec3(1.f))
{

}

Material::~Material() {
    for (std::map<std::string, Texture*>::iterator iter = textures.begin(); iter != textures.end(); iter++)
        delete iter->second;
    textures.clear();
}