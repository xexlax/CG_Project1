#version 330
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    float     shininess;
}; 
uniform sampler2D s;

uniform Material material;
uniform vec3 viewPos;
uniform Light light;

void main()
{

	vec3 ambient  =vec3(texture(s, TexCoord));
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(viewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  =  diff * vec3(texture(s, TexCoord)); 
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  spec * vec3(texture(s, TexCoord));
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 res=(diffuse+specular);


    
    
    
    FragColor = vec4(res  , 1.0);
}
