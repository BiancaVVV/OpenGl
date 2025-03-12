#include "../include/Scene.h"
#include <iostream>
#include <ext/matrix_transform.hpp>

// --------------------- CONSTRUCTOR --------------------- //
Scene::Scene() {
   skybox = nullptr;
   ground = nullptr;

   skyboxShader = nullptr;
   groundShader = nullptr;
   roadShader = nullptr;
   objectShader = nullptr;
}

// --------------------- DESTRUCTOR --------------------- //
Scene::~Scene() {
   delete skybox;
   delete ground;

   for (auto road : roads) {  
      delete road;
   }
   roads.clear();
  

   delete skyboxShader;
   delete groundShader;
   delete roadShader;
   delete objectShader;

   for (auto obj : objects) {
      delete obj;
   }
}

// --------------------- INITIALIZARE SCENĂ --------------------- //
void Scene::init() {

   float terrainSize = 10000.0f;
   float roadWidth = 250.0f;
   float roadLength = 2000.0f;
   std::vector<std::string> faces = {
       "./assets/textures/cube_map/right.png",
       "./assets/textures/cube_map/left.png",
       "./assets/textures/cube_map/top.png",
       "./assets/textures/cube_map/bottom.png",
       "./assets/textures/cube_map/front.png",
       "./assets/textures/cube_map/back.png"
   };

   skybox = new Skybox(terrainSize* 3.0f ,faces);
   ground = new Ground(terrainSize, 1000, "./assets/textures/cube_map/bottom.png");

   roads.push_back(new Road(roadWidth, roadLength, "./assets/textures/road/road_2k.jpg"));  // Jos
   roads.push_back(new Road(roadWidth, roadLength, "./assets/textures/road/road_2k.jpg"));  // Sus
   roads.push_back(new Road(roadWidth, roadLength- roadWidth, "./assets/textures/road/road_2k.jpg"));  // Stânga
   roads.push_back(new Road(roadWidth, roadLength- roadWidth, "./assets/textures/road/road_2k.jpg"));  // Dreapta
  

   skyboxShader = new Shader("./assets/shaders/vertexShaderSky.glsl", "./assets/shaders/fragShaderSky.glsl");
   groundShader = new Shader("./assets/shaders/vertexShaderGround.glsl", "./assets/shaders/fragShaderGround.glsl");
   roadShader = new Shader("./assets/shaders/vertexShaderRoad.glsl", "./assets/shaders/fragShaderRoad.glsl");
   objectShader = new Shader("./assets/shaders/vertexShaderObject.glsl", "./assets/shaders/fragShaderObject.glsl");

 
   Model* house1 = new Model("./assets/models/farm_house_gltf/scene.gltf");
   addObject(house1, glm::vec3(50.0f, 200.0f, 700.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(200.0f));

   Model* house2 = new Model("./assets/models/farm_house_gltf/scene.gltf");
   addObject(house2, glm::vec3(-350.0f, 200.0f, -600.0f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(200.0f));

   Model* lamp1 = new Model("./assets/models/street_lamp/street_lamp_01_4k.gltf");
   addObject(lamp1, glm::vec3(450.0f, 0.0f, 850.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f));

   Model* lamp2 = new Model("./assets/models/street_lamp/street_lamp_01_4k.gltf");
   addObject(lamp2, glm::vec3(-450.0f, 0.0f, 850.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f));

   Model* lamp3 = new Model("./assets/models/street_lamp/street_lamp_01_4k.gltf");
   addObject(lamp3, glm::vec3(450.0f, 0.0f, -850.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f));

   Model* lamp4 = new Model("./assets/models/street_lamp/street_lamp_01_4k.gltf");
   addObject(lamp4, glm::vec3(-550.0f, 0.0f, -850.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f));

   Model* lamp5 = new Model("./assets/models/street_lamp/street_lamp_01_4k.gltf");
   addObject(lamp5, glm::vec3(450.0f, 0.0f, 850.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f));


   Model* flower = new Model("./assets/models/flower/scene.gltf");
   addObject(flower, glm::vec3(-400.0f, 40.0f, -850.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(30.0f));



   Model* tree = new Model("./assets/models/tree/scene.gltf");
   addObject(tree, glm::vec3(-1500.0f, 20.0f, -850.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(30.0f));

   Model* bradz = new Model("./assets/models/bradz/scene.gltf");
   addObject(bradz, glm::vec3(-1200.0f, 10.0f, -350.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(30.0f));

   Model* gard = new Model("./assets/models/gard/scene.gltf");
   addObject(gard, glm::vec3(200.0f, 10.0f, -850.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(30.0f));

  
   Model* light = new Model("./assets/models/light/scene.gltf");
   addObject(light, glm::vec3(700.0f, 10.0f, -750.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(20.0f));



   Model* house3 = new Model("./assets/models/house3/scene.gltf");
   addObject(house3, glm::vec3(1500.0f, 5.0f, -250.0f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(20.0f));
   
   Model* house4 = new Model("./assets/models/house4/scene.gltf");
   addObject(house4, glm::vec3(2400.0f, 5.0f, -250.0f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(20.0f));

 
   Model* flori = new Model("./assets/models/flori/scene.gltf");
   addObject(flori, glm::vec3(1000.0f, 10.0f, -400.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f));


   Model* cat = new Model("./assets/models/cat/scene.gltf");
   addObject(cat, glm::vec3(800.0f, 12.0f, -420.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f));

   Model* cat2 = new Model("./assets/models/cat2/scene.gltf");
   addObject(cat2, glm::vec3(600.0f, 9.0f, -430.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

   Model* cat3 = new Model("./assets/models/cat3/scene.gltf");
   addObject(cat3, glm::vec3(700.0f, 9.0f, -410.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

   Model* casa7 = new Model("./assets/models/casa7/scene.gltf");
   addObject(casa7, glm::vec3(300.0f, 160.0f, -400.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(250.0f));


  

}

// --------------------- ADAUGARE OBIECT ÎN SCENĂ --------------------- //
void Scene::addObject(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
   objects.push_back(model);
   objectPositions.push_back(position);
   objectRotations.push_back(rotation);
   objectScales.push_back(scale);
}




// --------------------- RANDAREA SCENEI --------------------- //
void Scene::render(const glm::mat4& projection, const glm::mat4& view) {
  
 
   ground->render(*groundShader,projection,view);
   skybox->render(projection, view, *skyboxShader);

   float offsetX = 1000.0f;  // Lățimea dreptunghiului (jumatate din road)
   float offsetZ = 1000.0f;  // Lungimea dreptunghiulu (jumatate din road)
   // 🛣 Segmentul de sus 
   roads[0]->render(*roadShader, projection, view, glm::vec3(0.0f, 0.2f, -offsetZ), 90.0f);
   // 🛣 Segmentul de jos 
   roads[1]->render(*roadShader, projection, view, glm::vec3(0.0f, 0.2f, +offsetZ), 90.0f);
   // 🛣 Segmentul din stânga 
   roads[2]->render(*roadShader, projection, view, glm::vec3(-offsetX+125, 0.2f, 0.0f), 180.0f);
   // 🛣 Segmentul din dreapta 
   roads[3]->render(*roadShader, projection, view, glm::vec3(offsetX-125, 0.2f, 0.0f), 180.0f);
  
   //// Randare Obiecte
   objectShader->use();
   objectShader->setMat4("projection", projection);
   objectShader->setMat4("view", view);

   for (size_t i = 0; i < objects.size(); i++) {
      glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), objectPositions[i]);

      // Aplică rotația specifică fiecărui obiect pe toate axele
      modelMatrix = glm::rotate(modelMatrix, glm::radians(objectRotations[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
      modelMatrix = glm::rotate(modelMatrix, glm::radians(objectRotations[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
      modelMatrix = glm::rotate(modelMatrix, glm::radians(objectRotations[i].z), glm::vec3(0.0f, 0.0f, 1.0f));

      // Aplică scala individuală
      modelMatrix = glm::scale(modelMatrix, objectScales[i]);

      objectShader->setMat4("model", modelMatrix);
      objects[i]->Draw(*objectShader);
   }



}
