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
uniform sampler2D texs[10];


uniform Material material;
uniform vec3 viewPos;
uniform Light light;
uniform int texnum;
uniform float trange_l[10];
uniform  int repeat[10];
uniform float trange_r[10];

void main()
{
    
    vec3 texcolor=vec3(texture(s, TexCoord));


    if(texnum>=1)
    //texcolor= vec3(mix(texture(texs1, TexCoord), texture(s, TexCoord), 0.2));
    
    for(int i=0;i<texnum;i++){
          vec2 TexCoordV=TexCoord;
          float len=trange_r[i]-trange_l[i];
          if(TexCoordV.y>trange_l[i]&&TexCoordV.y<trange_r[i]){
              TexCoordV.y=(TexCoordV.y-trange_l[i])/len;
              TexCoordV.x=(4*TexCoordV.x);
              texcolor=vec3(texture(texs[i], TexCoordV));
          }
          
    }

	vec3 ambient  =light.ambient*texcolor;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse* diff *texcolor; 
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular*  spec * texcolor;
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 res=(diffuse + specular+ambient);


    
    
    
    FragColor = vec4(res  , 1.0);
}
