#include "Material.h"

Material::Material() {
	
	color = glm::vec3(0, 0, 0);
	diffuse = 0;
	specular = 0;
}
Material::Material(glm::vec3 c, float d, float s, float sh) {
	
	color = c;
	diffuse = d;
	specular = s;
	shine = sh;
}