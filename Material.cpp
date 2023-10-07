#include "Material.h"


// TODO: Consolidate model into a model and model loader class.
/*void Material::GetBaseColor(unsigned int nextNode) {

	json node = JSON["materials"][nextNode][pbrMetallicRoughness];

	// Select wether base color or base texture exists.
	glm::vec4 baseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if (node.find("baseColorFactor") != std::string::npos)
	{
		usingBaseTexture = false;

		float color[4];
		for (unsigned int i = 0; i < node["pbrMetallicRoughness"]["baseColorFactor"].size(); i++)
		{
			color[i] = node["pbrMetallicRoughness"]["baseColorFactor"][i];
		}

		// RGBA values.
		baseColor = glm::make_vec4(color);
	}

	// save as texture
	else if (node.find("baseColorTexture") != std::string::npos)
	{
		usingBaseTexture = true;

		// Pass index into texture finding func.
		GetTextureFromIndex(node[baseColorTexture][index], "diffuse");
	}
}

void Material::GetImageTextureFromIndex(unsigned int index, const char* type)
{
	// Sampler will set if texture wrap mirror or streatch.
	unsigned int sampler = JSON["textures"][index]["sampler"];
	unsigned int sourceIndex = JSON["textures"][index][source];

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	std::string texPath = JSON["images"][sourceIndex]["uri"];

	Texture tex = Texture((fileDirectory + texPath).c_str(), type, loadedTex.size());
}*/