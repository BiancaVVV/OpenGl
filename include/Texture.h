#pragma once

#include <glad/glad.h>
#include <string>s
#include <assimp/texture.h>

class Texture {
public:
   GLuint ID;
   bool isHDR;

   Texture(const char* path, bool hdr = false);
   Texture(const aiTexture* aiTex);
   void bind();
};
