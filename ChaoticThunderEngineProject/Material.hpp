#ifndef OBJECTS_MATERIAL
#define OBJECTS_MATERIAL
#include <glm.hpp>
#include <map>
#include <string>
#include "Structs.hpp"
#include "Texture.hpp"

class Material {
public:
	float specular_highlight; //Ns, 0 to 1000, how wide the specular highlight should be
	float optical_density; //Ni, 0.001 to 10, default 1, aka. index of refraction, how light bends in the material
	float dissolve; //d, 0.0-1.0, how opaque the object appears (1.) or transparent (0.)
	float sharpness; //sharpness, 0-1000, default 60, how sharp reflections in the object appear (otherwise blurry at lower values)
	int illum; //illum, integer indicating which lighting model to use
	glm::vec3 ambient_color; //Ka, Constant color all around object
	glm::vec3 diffuse_color; //Kd, Color reflected off surfaces
	glm::vec3 specular_color; //Ks, Specular light color (highlight)
	glm::vec3 emissive_color; //Ke, Emissive color
	glm::vec3 transmission_color; //Tf, transmission filter, allows only some % of each color channel through material
	std::map<std::string, Texture*> textures;

	Material();
	~Material();
};
#endif