#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

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

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //
    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// texture samplers
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform SpotLight spotlight;


void main()
{
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord)); 
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  light.specular * spec * vec3(texture(material.specular, TexCoord));
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 res=(diffuse + specular)*attenuation;


    //spot-light
    lightDir = normalize(spotlight.position - FragPos);
    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon   = spotlight.cutoff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);    
       
        // 执行光照计算
        // diffuse-spot 
    norm = normalize(Normal);
    
    diff = max(dot(norm, lightDir), 0.0);
    diffuse  = spotlight.diffuse * diff * vec3(texture(material.diffuse, TexCoord)); 
    
    // specular-spot
    viewDir = normalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);  
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular =  spotlight.specular * spec * vec3(texture(material.specular, TexCoord));
    
    // attenuation-spot
    distance    = length(spotlight.position - FragPos);
    attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + spotlight.quadratic * (distance * distance));
    
    res+=(diffuse + specular)*attenuation*intensity;
    
    
    
    
    
    FragColor = vec4(res + ambient , 1.0);

}