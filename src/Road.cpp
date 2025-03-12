
#include "../include/Road.h"
#include <stb_image.h>
#include <vector>
#include <iostream>
#include <ext/matrix_transform.hpp>

// --------------------- CONSTRUCTOR --------------------- //
Road::Road(float width, float length, const std::string& texturePath) {
   loadTexture(texturePath);
   generateMesh(width, length);
}

// --------------------- DESTRUCTOR --------------------- //
Road::~Road() {
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
   glDeleteTextures(1, &textureID);
}

// --------------------- GENERATE A SINGLE ROAD SEGMENT --------------------- //
void Road::generateMesh(float width, float length) {
   std::vector<float> vertices;
   std::vector<unsigned int> indices;

   float halfWidth = width / 2.0f;
   float halfLength = length / 2.0f;

   // Define road corners
   glm::vec3 corners[4] = {
       { -halfWidth,  0.2f, -halfLength },  // Bottom Left
       {  halfWidth,  0.2f, -halfLength },  // Bottom Right
       {  halfWidth,  0.2f,  halfLength },  // Top Right
       { -halfWidth,  0.2f,  halfLength }   // Top Left
   };

   for (int i = 0; i < 4; i++) {
      float u = (i == 0 || i == 3) ? 0.0f : 1.0f;
      float v = (i == 0 || i == 1) ? 0.0f : 1.0f;
      vertices.insert(vertices.end(), { corners[i].x, corners[i].y, corners[i].z, u, v });
   }
   indices = {
       0, 1, 2,
       2, 3, 0
   };

   indexCount = indices.size();

   // Configure OpenGL buffers
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

// --------------------- LOAD ROAD TEXTURE --------------------- //
void Road::loadTexture(const std::string& texturePath) {
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_2D, textureID);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f); 


   int width, height, nrChannels;
   unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
   if (data) {
      GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      std::cout << "Road texture loaded successfully: " << texturePath << std::endl;
   }
   else {
      std::cerr << "ERROR: Failed to load road texture: " << texturePath << std::endl;
   }
   stbi_image_free(data);
}

// --------------------- RENDER A SINGLE ROAD SEGMENT --------------------- //
void Road::render(Shader& shader, const glm::mat4& projection, const glm::mat4& view, glm::vec3 position, float rotation) {
   shader.use();
   shader.setMat4("view", view);
   shader.setMat4("projection", projection);

   glm::mat4 model = glm::mat4(1.0f);
   model = glm::translate(model, position);
   model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

   shader.setMat4("model", model);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, textureID);
   glBindVertexArray(VAO);
   glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
}
