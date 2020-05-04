#version 430
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;                                           
                                                                                    
uniform mat4 lightViewProjectionMatrix;
uniform mat4 model;                                                            
                                                                                                                                                                       
void main()                                                                         
{                                                                                   
    gl_Position = lightViewProjectionMatrix * model * vec4(aPos, 1.0);
}
