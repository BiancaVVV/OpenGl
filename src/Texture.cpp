#define STB_IMAGE_IMPLEMENTATION
#include "../include/Texture.h"
#include <stb_image.h>
#define TINYEXR_IMPLEMENTATION
#include <tinyexr.h>
#include <iostream>


Texture::Texture(const aiTexture* aiTex) {
   glGenTextures(1, &ID);
   glBindTexture(GL_TEXTURE_2D, ID);

   if (aiTex->mHeight == 0) {  // FBX Embedded Texture (compressed)
      int width, height, channels;
      unsigned char* image = stbi_load_from_memory(
         reinterpret_cast<unsigned char*>(aiTex->pcData), aiTex->mWidth, &width, &height, &channels, 0);
      if (image) {
         GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
         glGenerateMipmap(GL_TEXTURE_2D);
         stbi_image_free(image);
      }
   }
   else {  // Uncompressed texture
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aiTex->mWidth, aiTex->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, aiTex->pcData);
      glGenerateMipmap(GL_TEXTURE_2D);
   }
}


Texture::Texture(const char* path, bool hdr) {
   glGenTextures(1, &ID);
   glBindTexture(GL_TEXTURE_2D, ID);
   isHDR = hdr;


   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, channels;

   if (hdr) {
    /*  stbi_set_flip_vertically_on_load(true);*/
      float* data = nullptr;

      // Detectăm dacă fișierul este EXR
      std::string filename(path);
      if (filename.substr(filename.find_last_of(".") + 1) == "exr") {
         const char* err = nullptr;
         int ret = LoadEXR(&data, &width, &height, path, &err);
         if (ret != TINYEXR_SUCCESS) {
            std::cerr << "Error loading EXR texture: " << err << "\n";
            FreeEXRErrorMessage(err);
            return;
         }
         channels = 4; // EXR e mereu RGBA
      }
      else {
         data = stbi_loadf(path, &width, &height, &channels, 0);
      }

      if (data) {
         GLenum format = (channels == 3) ? GL_RGB16F : GL_RGBA16F;
         GLenum baseFormat = (channels == 3) ? GL_RGB : GL_RGBA;

         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, baseFormat, GL_FLOAT, data);

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         std::cout << "HDR Texture loaded: " << path << " (" << width << "x" << height << ")\n";

         stbi_image_free(data);
      }
      else {
         std::cerr << "Error loading HDR texture: " << path << "\n";
      }
   }
   else {
    /*  stbi_set_flip_vertically_on_load(true);*/
      unsigned char* image = stbi_load(path, &width, &height, &channels, 0);
      if (image) {
         GLenum format;
         if (channels == 1)
            format = GL_RED;
         else if (channels == 3)
            format = GL_RGB;
         else
            format = GL_RGBA;

         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
         glGenerateMipmap(GL_TEXTURE_2D);
         std::cout << "Texture loaded: " << path << " (" << width << "x" << height << ")\n";

         stbi_image_free(image);
      }
      else {
         std::cerr << "Error loading texture: " << path << "\n";
      }
   }
}

void Texture::bind() {
   glBindTexture(GL_TEXTURE_2D, ID);
}
