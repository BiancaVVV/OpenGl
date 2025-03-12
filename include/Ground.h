#pragma once
#include <glad/glad.h>
#include <glm.hpp>
#include "Shader.h"

class Ground {
public:
   Ground(float size, int divisions, const char* texturePath);
   ~Ground();

   void render(Shader& shader, const glm::mat4& projection, const glm::mat4& view);

private:
   GLuint VAO, VBO, EBO, textureID;
   int indexCount;

   void generateMesh(float size, int divisions);
   void loadTexture(const char* texturePath);
};
