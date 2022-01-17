#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 skyColor;

void main()
{    
    FragColor = vec4(skyColor*vec3(texture(skybox, TexCoords)),1.0);
}