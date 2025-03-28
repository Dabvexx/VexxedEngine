#include "Texture.h"

// Define the static variable
GLuint Texture::next_id = 0;

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	ID = next_id++;
	static GLuint textureIDGlobal = 0;

	printf("Begin loading texture: %s\n", image);
	// assign texture type of texture object.
	type = texType;
	int imgWidth, imgHeight, numColCh;
	//int imgWidth;
	//int imgHeight;
	//int numColCh;

	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(image, &imgWidth, &imgHeight, &numColCh, 0);

	if (bytes == NULL) {
		printf("\nError loading texture %s\nReason: %s\n", image, stbi_failure_reason());
		return;
	}
	printf("Texture %s of ID % d loaded with width of % d, height of % d, and %d channels\n",image, ID, imgWidth, imgHeight, numColCh);

	std::cout << (int)bytes[0] << " " << (int)bytes[1] << " " << (int)bytes[2] << " " << (int)bytes[3] << std::endl;
	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	if (numColCh == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			imgWidth,
			imgHeight,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			imgWidth,
			imgHeight,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			imgWidth,
			imgHeight,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");

	// Generates MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(GL_TEXTURE_2D, 0);
	printf("Finished loading texture: %s\n", image);
}

Texture::Texture(float color[4], const char* texType, GLuint slot)
{
	ID = next_id++;
	printf("Begin loading base color texture");
	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the OpenGL Texture object
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// This is the most probably source of the issue, loading this data as image data.
	// needs to be convered to unsigned char with size of 4 * sizeof unsigned char
	// No idea im to tired for this shit rn
	unsigned char data[4] = 
	{ 
		color[0],
		color[1],
		color[2],
		color[3]
		/*(int)powf(color[0], 1 / 2.2) * 255,
		(int)powf(color[1], 1 / 2.2) * 255,
		(int)powf(color[2], 1 / 2.2) * 255,
		(int)powf(color[3], 1 / 2.2) * 255,*/
	};

		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			1,
			1,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data
		);

		// Generates MipMaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbinds the OpenGL Texture object so that it can't accidentally be modified
		glBindTexture(GL_TEXTURE_2D, 0);
		printf("Finished loading color as texture\n");
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
	printf("Deleted texture, ID: %d\n", ID);
}