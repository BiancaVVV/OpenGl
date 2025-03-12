#include "../include/Model.h"
#include "../include/Texture.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& path) {
   loadModel(path);
}

void Model::Draw(Shader& shader) {
   for (Mesh& mesh : meshes)
      mesh.Draw(shader);
}

void Model::loadModel(const std::string& path) {
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(path,
      aiProcess_Triangulate |
      aiProcess_FlipUVs |
      aiProcess_GenNormals |
      aiProcess_OptimizeMeshes |
      aiProcess_ConvertToLeftHanded
   );

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
      return;
   }

   directory = path.substr(0, path.find_last_of('/'));
   processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
   for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
   }
   for (unsigned int i = 0; i < node->mNumChildren; i++)
      processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   std::vector<Texture> textures;

   // Procesăm vertecșii
   for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

      if (mesh->mNormals) {
         vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
      }
      else {
         vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
      }

      if (mesh->mTextureCoords[0]) {
         vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      }
      else {
         vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      }

      vertices.push_back(vertex);
   }

   // Procesăm indecșii
   for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
         indices.push_back(face.mIndices[j]);
      }
   }

 
   if (mesh->mMaterialIndex >= 0) {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, scene);
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

      std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, scene);
      textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

      std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, scene);
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

      std::vector<Texture> roughnessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, scene);
      textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

   }

   return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene) {
   std::vector<Texture> textures;

   for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);

      std::string filename = directory + "/" + str.C_Str();
      std::cout << "🔍 Checking texture: " << filename << std::endl;

      // 📌 Verificăm dacă textura este încorporată (GLB format)
      if (str.C_Str()[0] == '*') {
         const aiTexture* tex = scene->GetEmbeddedTexture(str.C_Str());
         if (tex) {
            std::cout << "✅ Embedded texture detected! Loading directly from memory.\n";
            Texture texture(tex);
            textures.push_back(texture);
            continue;
         }
      }

      // 📌 Textură normală (externă)
      Texture texture(filename.c_str());
      if (texture.ID == 0) {
         std::cerr << " Failed to load texture: " << filename << std::endl;
      }
      else {
         textures.push_back(texture);
         std::cout << "Successfully loaded: " << filename << std::endl;
      }
   }

   return textures;
}




