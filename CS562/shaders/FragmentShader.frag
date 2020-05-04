#version 430
#extension GL_EXT_texture_array : enable

in vec4 Position;
in vec2 Texcoord;
in vec3 Normal;

uniform vec3 diffuseMaterial;
uniform vec3 lightDirection;

layout(location = 0) out vec4 outcolor;

uniform sampler2DArrayShadow depthTextures;

uniform int numOfCascades;
uniform	float angleBias = 0.006f;

uniform mat4 viewMatrix;
uniform mat4 inverseViewMatrix;
uniform mat4 viewProjecMatrices[4];
uniform vec4 cascadedSplits;

uniform bool render_zones;

vec4 CascadeIndicator = vec4(0.0, 0.0, 0.0, 0.0);

uint findCascade(float zDepthValue)
{
	uint cascadeIdx = 0;

	for(uint i = 0; i < numOfCascades - 1; ++i)
	{
		if(zDepthValue < cascadedSplits[i])
		{
			cascadeIdx = i + 1;
			
			if(render_zones)
			{
				if (i == 0) 
				    CascadeIndicator = vec4(0.1, 0.0, 0.0, 0.0);
				else if (i == 1)
				    CascadeIndicator = vec4(0.0, 0.1, 0.0, 0.0);
				else if (i == 2)
				    CascadeIndicator = vec4(0.0, 0.0, 0.1, 0.0);
			}
		}
	}
	return cascadeIdx;

}


vec3 obtainShadow()
{	
	vec3 viewPosition = Position.xyz;

	float depthViewSpace = Position.z;

	uint cascadeIdx = findCascade(depthViewSpace);	
	
	vec3 negativLigthDir = lightDirection * -1.0f;

	//Take the corresponding projection matrix 
	mat4 lightViewProjectionMatrix = viewProjecMatrices[cascadeIdx];
	
	// Move the fragment to shadow coordinates
	vec4 fragmentModelViewPosition = vec4(viewPosition,1.0f);
	vec4 fragmentModelPosition = inverseViewMatrix * fragmentModelViewPosition;
	vec4 fragmentShadowPosition = lightViewProjectionMatrix * fragmentModelPosition;

	//Perspective division
	fragmentShadowPosition.xyz /= fragmentShadowPosition.w;

	//Move from NDC to 0 - 1 coordinate system
	fragmentShadowPosition = fragmentShadowPosition * 0.5f + 0.5f;

	//The depth will be the Z coordinate
    float currentDepth = fragmentShadowPosition.z;
	
	//To sample the shadow map we set the Z position 
	fragmentShadowPosition.z = cascadeIdx;   

	float bias = max(angleBias * (1.0 - dot(Normal, negativLigthDir)), 0.0008);	

	vec2 texelSize = 1.0 / textureSize(depthTextures, 0).xy;

	float totalShadow = 0.0;

	float pcf = shadow2DArray(depthTextures, vec4(fragmentShadowPosition.xyz, currentDepth - bias )).r;
	
	float shadow = 0.0;
	if(currentDepth > pcf)
		shadow += 1.0;	

	return vec3(shadow); 
}

void main()
{  
	vec3 negativLigthDir = lightDirection * -1.0f;	
	vec3 ambientColor = diffuseMaterial * 0.3f;	
	
	float diffuseCoefficient = max(0.0, dot(Normal, negativLigthDir));	
    vec3 diffuse = diffuseCoefficient * diffuseMaterial * 0.5f;

	vec3 inShadow = vec3(1.0) - obtainShadow();

    vec4 prev_color = vec4(((ambientColor + inShadow * (diffuse)) * diffuseMaterial),1.0f); 
	
    outcolor = prev_color + CascadeIndicator;
}