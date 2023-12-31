#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the currentPosition from the Vertex Shader.
in vec3 crntPos;
// Inputs the normal from the Vertex Shader.
in vec3 normal;
// Inputs the color from the Vertex Shader.
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader.
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;

uniform sampler2D specular0;

// Lighting variables.
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 PointLight()
{
	// Configurable Inverse Square Law light falloff approximation
	vec3 lightVec = lightPos - crntPos;
	
	float dist = length(lightVec);
	float a = 1.0f;
	float b = 0.07f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// Ambient lighting AKA Brightness;
	float ambient = 0.10f;

	// Diffuse lighting
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightVec);
	// Max with zero so we dont get negative colors.
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// Specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specPower = 16;
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), specPower);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 DirectLight()
{
	// Ambient lighting AKA Brightness;
	float ambient = 0.50f;

	// Diffuse lighting
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	// Max with zero so we dont get negative colors.
	// Absolute value helps with transparency.
	float diffuse = max(abs(dot(normal, lightDirection)), 0.0f);

	// Specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specPower = 16;
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), specPower);
	float specular = specAmount * specularLight;

	if(texture(diffuse0, texCoord).a < 0.1)
	{
		discard;
	}

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 SpotLight()
{
	// Controls the area that is lit up.
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// Ambient lighting AKA Brightness;
	float ambient = 0.20f;

	// Diffuse lighting
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	// Max with zero so we dont get negative colors.
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// Specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specPower = 16;
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), specPower);
	float specular = specAmount * specularLight;

	// Calculate intensity of crntPos based on angle to center of light cone.
	float angle = dot(vec3(0.0f, -1.9f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

float near = 0.1f;
float far = 100.0f;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5f, float offset = 5.0f)
{
	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main()
{
	// Output final color.
	float depth = logisticDepth(gl_FragCoord.z);
	FragColor = DirectLight() * (1.0f - depth) + vec4(depth * vec3(0.07f, 0.13f, 0.17f), 1.0);
	// FragColor = DirectLight();
	//FragColor = PointLight();
}