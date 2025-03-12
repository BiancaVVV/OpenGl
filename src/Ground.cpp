#include "../include/Ground.h"
#include <stb_image.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <ext/matrix_transform.hpp>

// --------------------- CONSTRUCTOR --------------------- //
Ground::Ground(float size, int divisions, const char* texturePath) {
   loadTexture(texturePath);
   generateMesh(size, divisions);
}

// --------------------- DESTRUCTOR --------------------- //
Ground::~Ground() {
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
}

// --------------------- GENERARE MESH --------------------- //
void Ground::generateMesh(float size, int divisions) {
   float halfSize = size / 2.0f;
   float step = size / divisions;
   float uvScale = 1.0f; 

   std::vector<float> vertices;
   std::vector<unsigned int> indices;

   for (int i = 0; i <= divisions; i++) {
      for (int j = 0; j <= divisions; j++) {
         float x = -halfSize + j * step;
         float z = -halfSize + i * step;
        /* float y = sin(x * 0.01f) * cos(z * 0.01f) * 5.0f; */
         float y = 0;

         float u = (float)j / divisions;
         float v = (float)i / divisions;

         vertices.insert(vertices.end(), { x, y, z, u, v });
      }
   }

   for (int i = 0; i < divisions; i++) {
      for (int j = 0; j < divisions; j++) {
         int row1 = i * (divisions + 1);
         int row2 = (i + 1) * (divisions + 1);

         indices.insert(indices.end(), {
             static_cast<unsigned int>(row1 + j),
             static_cast<unsigned int>(row2 + j),
             static_cast<unsigned int>(row1 + j + 1),

             static_cast<unsigned int>(row2 + j),
             static_cast<unsigned int>(row2 + j + 1),
             static_cast<unsigned int>(row1 + j + 1)
            });
      }
   }

   indexCount = indices.size();

   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);
}

// --------------------- ÎNCĂRCARE TEXTURĂ --------------------- //
void Ground::loadTexture(const char* texturePath) {
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_2D, textureID);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, nrChannels;
   unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
   if (data) {
      GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      std::cout << "Ground texture loaded successfully: " << texturePath << std::endl;
   }
   else {
      std::cerr << "ERROR: Failed to load ground texture: " << texturePath << std::endl;
   }
   stbi_image_free(data);
}

// --------------------- RANDAREA TERENULUI --------------------- //
void Ground::render(Shader& shader, const glm::mat4& projection, const glm::mat4& view) {
   shader.use();

   glm::mat4 model = glm::mat4(1.0f);
   model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));  
   model = glm::scale(model, glm::vec3(1.0f)); 

   shader.setMat4("model", model);
   shader.setMat4("view", view);
   shader.setMat4("projection", projection);
   shader.setFloat("groundSize", 50000.0f);  
   shader.setFloat("uvScale", 1.0f);  

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, textureID);
   glBindVertexArray(VAO);
   glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
}



