#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"

class Skybox {
public:
   Skybox(float terrainSize,const std::vector<std::string>& cubemapFaces);
   ~Skybox();

   void render(const glm::mat4& projection, const glm::mat4& view, Shader& skyboxShader);

private:
   GLuint skyboxVAO, skyboxVBO;
   GLuint cubemapTexture;
   float size;

   void setupSkybox();
   GLuint loadCubemap(const std::vector<std::string>& faces);
};
