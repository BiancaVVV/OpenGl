#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "Texture.h"

class Road {
public:
   Road(float width, float length, const std::string& texturePath);
   ~Road();

  
   void render(Shader& shader, const glm::mat4& projection, const glm::mat4& view, glm::vec3 position, float rotation);
private:
   GLuint VAO, VBO, EBO;
   GLuint textureID;
   int indexCount;

   void loadTexture(const std::string& texturePath);
   void generateMesh(float width, float length);
  
};
