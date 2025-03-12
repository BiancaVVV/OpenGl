#include "../include/Skybox.h"
#include <stb_image.h>

// --------------------- SKYBOX CONSTRUCTOR --------------------- //


// --------------------- SKYBOX DESTRUCTOR --------------------- //
Skybox::~Skybox() {
   glDeleteVertexArrays(1, &skyboxVAO);
   glDeleteBuffers(1, &skyboxVBO);
   glDeleteTextures(1, &cubemapTexture);
}

// --------------------- CONSTRUCTOR SKYBOX --------------------- //
Skybox::Skybox(float terrainSize, const std::vector<std::string>& cubemapFaces) {
   size = terrainSize;  // Skybox-ul este de 2 ori mai mare decât terenul
   cubemapTexture = loadCubemap(cubemapFaces);
   setupSkybox();
}

// --------------------- CONFIGURARE SKYBOX --------------------- //
void Skybox::setupSkybox() {
   float skyboxVertices[] = {
      // Dreapta
      size, -size, -size,  size, size, -size,  size, size, size,
      size, size, size,  size, -size, size,  size, -size, -size,

      // Stânga
      -size, -size, -size,  -size, -size, size,  -size, size, size,
      -size, size, size,  -size, size, -size,  -size, -size, -size,

      // Sus
      -size, size, -size,  -size, size, size,  size, size, size,
      size, size, size,  size, size, -size,  -size, size, -size,

      // Jos
      -size, -size, -size,  size, -size, -size,  size, -size, size,
      size, -size, size,  -size, -size, size,  -size, -size, -size,

      // Față
      -size, -size, size,  size, -size, size,  size, size, size,
      size, size, size,  -size, size, size,  -size, -size, size,

      // Spate
      -size, -size, -size,  -size, size, -size,  size, size, -size,
      size, size, -size,  size, -size, -size,  -size, -size, -size
   };

   glGenVertexArrays(1, &skyboxVAO);
   glGenBuffers(1, &skyboxVBO);
   glBindVertexArray(skyboxVAO);
   glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}


// --------------------- ÎNCĂRCARE TEXTURI CUBEMAP --------------------- //
GLuint Skybox::loadCubemap(const std::vector<std::string>& faces) {
   GLuint textureID;
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

   int width, height, nrChannels;
   for (unsigned int i = 0; i < faces.size(); i++) {
      unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
      if (data) {
         GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
         stbi_image_free(data);
      }
      else {
         std::cerr << "ERROR: Failed to load cubemap texture: " << faces[i] << std::endl;
      }
   }

   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   return textureID;
}

// --------------------- RANDAREA SKYBOX-ULUI --------------------- //
void Skybox::render(const glm::mat4& projection, const glm::mat4& view, Shader& skyboxShader) {
   glDepthFunc(GL_LEQUAL);

   skyboxShader.use();
   skyboxShader.setMat4("projection", projection);

   // Eliminăm translația din view matrix pentru ca skybox-ul să fie infinit
   glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
   skyboxShader.setMat4("view", skyboxView);

   // Creăm matricea model pentru un skybox mai mare
   glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(size));
   skyboxShader.setMat4("model", model);

   glBindVertexArray(skyboxVAO);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
   glDrawArrays(GL_TRIANGLES, 0, 36);

   glDepthFunc(GL_LESS);
}


