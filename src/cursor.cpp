#include "headers/cursor.h"
#include "headers/settings.h"
#include "headers/skybox.h"
#include "headers/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"



 float vertices[] = {
        // positions                   // texture coords
         0.5f,  0.5f, 0.0f,      1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,      1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,      0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,      0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

Cursor::Cursor(/* args */)
{
    x= SCR_WIDTH/2;;
    y= SCR_HEIGHT/2;
    cursor_type=CURSOR_SELECT;

}

Cursor::~Cursor()
{
    glDeleteVertexArrays(1, &cVAO);
    glDeleteBuffers(1, &cVBO);
    glDeleteBuffers(1, &cEBO);
}

void Cursor::Move(float mx, float my){
    x=mx;
    y=my;
}

void Cursor::init(){
     unsigned int VAO,VBO,EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

  

    glGenTextures(1, &cursorTexture);
    glBindTexture(GL_TEXTURE_2D, cursorTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("D:\\Study\\OpenGL2020.12\\VSCproj\\resources\\cursor0.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    cVAO=VAO;
    cVBO=VBO;
    cEBO=EBO;

}

void Cursor::Draw(Shader shader){



        glBindTexture(GL_TEXTURE_2D, cursorTexture);

        // render container
        shader.use();
       
        glBindVertexArray(cVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      

}

