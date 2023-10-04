#include "Material.h"

void Material::GetBaseColor(unsigned int nextNode) {

	json node = JSON["materials"][nextNode];

	// Select wether base color or base texture exists.
	glm::vec4 baseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

}