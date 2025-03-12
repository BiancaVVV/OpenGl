#pragma once
#include "Skybox.h"
#include "Ground.h"
#include "Road.h"
#include "Model.h"
#include "Shader.h"
#include <vector>
#include <glm.hpp>

class Scene {
public:
   Scene();
   ~Scene();
   void init();
   void render(const glm::mat4& projection, const glm::mat4& view);
   void addObject(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

private:
   Skybox* skybox;
   Ground* ground;
   std::vector<Road*> roads;
 
   std::vector<Model*> objects;
   std::vector<glm::vec3> objectPositions;
   std::vector<glm::vec3> objectRotations;
   std::vector<glm::vec3> objectScales;


   Shader* skyboxShader;
   Shader* groundShader;
   Shader* roadShader;
   Shader* objectShader;
};
