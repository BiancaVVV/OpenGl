#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include "Shader.h"
#include "Texture.h"

struct Vertex {
   glm::vec3 Position;
   glm::vec3 Normal;    
   glm::vec2 TexCoords;
};

class Mesh {
public:
   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   std::vector<Texture> textures;

   Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
   void Draw(Shader& shader);

private:
   GLuint VAO, VBO, EBO;
   void setupMesh();
};
