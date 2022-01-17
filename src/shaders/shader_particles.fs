#version 330
out vec4 FragColor;
in vec2 TexCoord;

in vec3 FragPos;

struct Light {
     vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform sampler2D s;
uniform Light light;
uniform vec3 skyColor;
void main()
{
    
    vec4 texcolor=texture(s, TexCoord);
    if(texcolor.r < 0.5) 
        discard;

	vec4 ambient  =vec4(skyColor*vec3(texcolor),0);

    vec4 res=(ambient);
    
    FragColor = res;
}
