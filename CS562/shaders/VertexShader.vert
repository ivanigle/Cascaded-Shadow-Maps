#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewMatrix2;
uniform mat3 normalMatrix;

//Vertex to fragment values
out vec4 Position;
out vec2 Texcoord;
out vec3 Normal;

void main(void)
{
	gl_Position = projectionMatrix * modelViewMatrix  * vec4(position,1.0);

	Texcoord = texcoord;
	Position = (modelViewMatrix2*vec4(position,1.0));
	Normal   = normalize(normalMatrix*normal);
}