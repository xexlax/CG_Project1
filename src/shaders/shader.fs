#version 330
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;


struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    float     shininess;
}; 
uniform sampler2D s;
uniform sampler2D texs[10];
uniform sampler2D shadowMap;

uniform Material material;
uniform vec3 viewPos;
uniform Light light;
uniform int texnum;
uniform float trange_l[10];
uniform  int repeat[10];
uniform  int reverse[10];
uniform float trange_r[10];
uniform int type[10];
uniform int shadowdisplay;

float ShadowCalculation(vec4 fragPosLightSpace,float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow

    float shadow=0.0;
   vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y)*texelSize).r; 
        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;


     if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

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
               

              

              if(type[i]==0){
                  //环绕模式
                  TexCoordV.x=(repeat[i]*TexCoordV.x);

                  if(reverse[i]==1){
                  TexCoordV.y=1-TexCoordV.y;
                  TexCoordV.x=1-TexCoordV.x;
                }

              }
              else{
                  //中心模式
                  float theta= TexCoordV.x*(2*3.14159);
                  float r=(TexCoordV.y)/2;

                  

                  
                  if(reverse[i]==1){
                      TexCoordV.x=r*cos(theta)+0.5;

                        TexCoordV.y=1-r*sin(theta)+0.5;
                  
                    }
                    else{
                        r=(1-TexCoordV.y)/2;
                        

                        TexCoordV.x=r*cos(theta)+0.5;

                        TexCoordV.y=r*sin(theta)+0.5;

                    }
                  

                  
                  
                
              }
              

              texcolor*=vec3(texture(texs[i], TexCoordV));
          }
          
    }

	vec3 ambient  =light.ambient;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse* diff ; 
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular*  spec ;

    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);  

    float shadow=0;          
    if(shadowdisplay==1){
         shadow = ShadowCalculation(FragPosLightSpace,bias); 
     }     
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texcolor;    
    
    FragColor = vec4(lighting  , 1.0);
}
