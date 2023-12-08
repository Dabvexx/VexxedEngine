#include "Model.h"

Model::Model(const char* file)
{
	std::string text = GetFileContents(file);
	JSON = json::parse(text);

	Model::file = file;
	data = GetData();

	// This simply has to traverse every single node in the meshes section in order to multiload.
	// i do need to figure out how to place it in the correct places.
	// They are placed correctly just not rotated correctly
	
	json nodes = JSON["scenes"][0]["nodes"];

	std::vector <unsigned int> nodesToProcess(nodes.size());

	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		nodesToProcess[i] = JSON["scenes"][0]["nodes"][i];
	}

	std::cout << "Root Nodes to traverse: ";

	for (unsigned int i = 0; i < nodesToProcess.size(); i++)
	{
		std::cout << nodesToProcess[i] << " ";
	}

	std::cout << std::endl;

	for (unsigned int i = 0; i < nodesToProcess.size(); i++)
	{
		std::cout << "Root node to be traversed: " << nodesToProcess[i]  << std::endl;
		TraverseNode(nodesToProcess[i]);
	}
	//TraverseNode(0);
}

void Model::Draw(Shader& shader, Camera& camera)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
	}
}

void Model::LoadMesh(unsigned int indMesh)
{
	// Get all accessor indices
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];
	unsigned int matAccInd = JSON["meshes"][indMesh]["primitives"][0]["material"];

	std::vector <float> posVec = GetFloats(JSON["accessors"][posAccInd]);
	std::vector <glm::vec3> positions = GroupFloatsVec3(posVec);
	std::vector <float> normalVec = GetFloats(JSON["accessors"][normalAccInd]);
	std::vector <glm::vec3> normals = GroupFloatsVec3(normalVec);
	std::vector <float> texVec = GetFloats(JSON["accessors"][texAccInd]);
	std::vector <glm::vec2> texUVs = GroupFloatsVec2(texVec);
	// material = JSON["materials"][matAccInd];
	std::cout << "Assembling vertices" << std::endl;
	std::vector<Vertex> vertices = AssembleVertices(positions, normals, texUVs);
	std::cout << "Getting indices" << std::endl;
	std::vector<GLuint> indices = GetIndices(JSON["accessors"][indAccInd]);
	std::cout << "Getting textures" << std::endl;
	std::vector<Texture> textures = GetTextures(JSON["meshes"][indMesh]["primitives"][0]);

	std::cout << "Finished loading mesh" << std::endl;
	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::TraverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	// Current node.
	json node = JSON["nodes"][nextNode];

	std::cout << "Traversing node: " << nextNode << std::endl;

	// Get translation if exists.
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		std::cout << "Found translation" << std::endl;
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
		{
			transValues[i] = (node["translation"][i]);
		}
		translation = glm::make_vec3(transValues);
	}

	// Get rotation if exists.
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		std::cout << "Found rotation" << std::endl;

		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}

	// Get scale if exists.
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		std::cout << "Found scale" << std::endl;
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
		{
			scaleValues[i] = (node["scale"][i]);
		}
		scale = glm::make_vec3(scaleValues);
	}

	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		std::cout << "Found Matrix" << std::endl;

		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
		{
			matValues[i] = (node["matrix"][i]);
		}
		matNode = glm::make_mat4(matValues);
	}

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Multiply all matrices together
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	if (node.find("mesh") != node.end())
	{
		std::cout << "Found mesh" << std::endl;

		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		std::cout << "Loading mesh" << std::endl;

		LoadMesh(node["mesh"]);
	}

	// Check if node has children and apply function to them with matNextNode
	if (node.find("children") != node.end())
	{
		std::cout << "Mesh has child" << std::endl;
		for (unsigned int i = 0; i < node["children"].size(); i++)
		{
			TraverseNode(node["children"][i], matNextNode);
		}
	}

	std::cout << "Finished traversing node: " << nextNode << std::endl;
}

std::vector <unsigned char> Model::GetData()
{
	// Create a place to store the raw text, and get the uri of the .bin file
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	// Store raw text data into bytesText
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = GetFileContents((fileDirectory + uri).c_str());

	// Transform the raw text data into bytes and put them in a vector
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::GetFloats(json accessor)
{
	std::vector<float> floatVec;

	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::GetIndices(json accessor)
{
	std::vector<GLuint> indices;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}
	/*else if (componentType == 5126)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			float value;
			std::memcpy(&value, bytes, sizeof(float));
			indices.push_back((GLuint)value);
		}
	}*/

	return indices;
}

std::vector<Texture> Model::GetTextures(json accessor)
{
	std::cout << "Looking for material" << std::endl;
	// See if a material exists on the object
	unsigned int materialInd;

	std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	if (accessor.find("material") != accessor.end())
	{
		printf("Found material\n");
		// Get the material index
		materialInd = accessor["material"];
	}
	else
	{
		// create a black texture
		// put in a vector
		// return just that
		float color[3] =
		{
			0,
			0,
			0,
		};

		// Save the RGB texture to a temporary image file
		const char* textureName = "TempRgbTexture" + loadedTex.size();
		stbi_write_png(textureName, 1, 1, 3, color, loadedTex.size());

		// Use the existing Texture class by passing the image file path
		Texture diffuse(textureName, "diffuse", 0);
		textures.push_back(diffuse);
		loadedTex.push_back(diffuse);
		loadedTexName.push_back(textureName);

		// Delete the temporary image file
		remove(textureName);
		return textures;
	}

	json matPath = JSON["materials"][materialInd]["pbrMetallicRoughness"];

	// Check if this uses basecolor or baseColorTexture.
	unsigned int texInd;
	printf("Checking for baseColorType\n");

	if (matPath.find("baseColorTexture") != matPath.end())
	{
		printf("Found Base Color Texture\n");
		// Get the index into the images array.
		texInd = matPath["baseColorTexture"]["index"];

		// First check if there is a textures node
		// If there is, go to the index of the textures node, 
		// get the source, then use the image[source] and use that as the image

		unsigned int texSourceInd = JSON["textures"][texInd]["source"];

		std::string texPath = JSON["images"][texSourceInd]["uri"];

		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				std::cout << "Found image: " << texPath << std::endl;
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
				printf("Found diffuse texture at: %s\n", (fileDirectory + texPath).c_str());
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
		}		
	}
	else if (matPath.find("baseColorFactor") != matPath.end())
	{
		printf("Found baseColorFactor\n");
		printf("Creating temp texture\n");
		// This will create a small 1x1 texture that is simply basecolor only.
		// May be better to make a second texture constructor that takes a array and uses it as the bytes.
		json colorData = matPath["baseColorFactor"];
		float color[4] =
		{ 
			(float)colorData[0],
			(float)colorData[1],
			(float)colorData[2],
			(float)colorData[3]
		};

		//color[0] = (int)(color[0] * 255);
		//color[1] = (int)(color[1] * 255);
		//color[2] = (int)(color[2] * 255);
		//color[3] = (int)(color[3] * 255);

		std::cout << "Color data: " <<
			color[0] << " " <<
			color[1] << " " <<
			color[2] << " " <<
			color[3] << std::endl;
		//printf("Color data: %s, %s, %s, %s\n", color[0], color[1], color[2], color[3]);

		// Save the RGB texture to a temporary image file
		const char* textureName = "___TempRgbTexture" + loadedTex.size();
		stbi_write_png(textureName, 1, 1, 4, color, loadedTex.size());

		// Use the existing Texture class by passing the image file path
		Texture diffuse(textureName, "diffuse", loadedTex.size());
		textures.push_back(diffuse);
		loadedTex.push_back(diffuse);
		loadedTexName.push_back(textureName);

		// Delete the temporary image file
		remove(textureName);
	}

	// Go on to finding the specular textures.

	/*std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	std::cout << "Begin searching for images" << std::endl;

	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		std::string texPath = JSON["images"][i]["uri"];

		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				std::cout << "Found image: " << texPath << std::endl;
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			// find material image uri here.
			// get material index through mesh
			// get image texture through material

			if (texPath.find("baseColor") != std::string::npos || texPath.find("diffuse") != std::string::npos)
			{
				printf("Found diffuse texture");
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
			// Load specular texture
			else if (texPath.find("metallicRoughness") != std::string::npos || texPath.find("specular") != std::string::npos)
			{
				printf("Found specular texture");
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
		}
	}*/

	return textures;
}



/*void Model::GetBaseColor(unsigned int nextNode) {

	json node = JSON["materials"][nextNode]["pbrMetallicRoughness"];

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

void GetTexturesFromMaterial() 
{

}*/

void Model::GetImageTextureFromIndex(unsigned int index, const char* type)
{
	// Sampler will set if texture wrap mirror or streatch.
	unsigned int sampler = JSON["textures"][index]["sampler"];
	unsigned int sourceIndex = JSON["textures"][index]["source"];

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	std::string texPath = JSON["images"][sourceIndex]["uri"];

	Texture tex = Texture((fileDirectory + texPath).c_str(), type, loadedTex.size());
}

std::vector<Vertex> Model::AssembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}

	return vertices;
}

std::vector<glm::vec2> Model::GroupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<glm::vec3> Model::GroupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<glm::vec4> Model::GroupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}
