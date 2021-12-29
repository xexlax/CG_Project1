#include <string>
#include <vector>
#include "shader.h"
#include "camera.h"

class SkyBox{
    public:
    std::string dirpath="D:\\Study\\OpenGL2020.12\\VSCproj\\resources\\skybox";
    std::vector<std::string> faces;

    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    
    // utility function for loading a 2D texture from file
    // ---------------------------------------------------
    unsigned int loadTexture(char const * path);


    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front) 
    // -Z (back)
    // -------------------------------------------------------
    
    
    void Init();

    void Draw(Camera camera,Shader skyShader);
    unsigned int loadCubemap(std::vector<std::string> faces);
    SkyBox();
    ~SkyBox();
};
