#include "Light.h"
#include <iostream>
Light::Light(int light_type, glm::vec3 light_color, float light_brightness, glm::vec3 pos_or_dir) {

	type = light_type;
	color = light_color;
	brightness = light_brightness;
	direction = pos_or_dir;
	position = pos_or_dir;

}