#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>
#include <glm/glm.hpp>

#include"ShaderClass.h"

class Material
{
	public:
		bool doubleSided;
		//glm::vec4 emissiveColor;

		float metalicFactor;
		float roughnessFactor;

		bool usingBaseTexture;
		glm::vec4 baseColor;
		
		// Get either the baseColorTexture or the baseColorFactor RGBA values depending on which exists using node.find
		void GetBaseColor(unsigned int nextNode);
};

#endif // !MATERIAL_CLASS_H
