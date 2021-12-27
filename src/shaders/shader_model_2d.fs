#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform Material material;
uniform vec3 viewPos;
uniform Light light;


void main()
{    

    
    
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(viewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    if(diff>0.5) diff=1;
    
    else diff=0;
    
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords)); 
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    if(spec>0.8) spec=1;
    
    else spec=0;
    vec3 specular =  light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    



    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 res=(diffuse)*attenuation;


    
    
    
    FragColor = vec4(res + ambient , 1.0);
}