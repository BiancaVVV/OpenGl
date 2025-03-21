#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
   unsigned int ID;

   Shader(const char* vertexPath, const char* fragmentPath) {
      std::string vertexCode;
      std::string fragmentCode;
      std::ifstream vShaderFile;
      std::ifstream fShaderFile;

      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      vShaderFile.close();
      fShaderFile.close();
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();

      const char* vShaderCode = vertexCode.c_str();
      const char* fShaderCode = fragmentCode.c_str();

      unsigned int vertex, fragment;
      int success;
      char infoLog[512];

      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
      glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
      if (!success) {
         glGetShaderInfoLog(vertex, 512, NULL, infoLog);
         std::cout << "Vertex Shader compilation failed:\n" << infoLog << std::endl;
      }

      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);
      glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
      if (!success) {
         glGetShaderInfoLog(fragment, 512, NULL, infoLog);
         std::cout << "Fragment Shader compilation failed:\n" << infoLog << std::endl;
      }

      ID = glCreateProgram();
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);
      glLinkProgram(ID);
      glGetProgramiv(ID, GL_LINK_STATUS, &success);
      if (!success) {
         glGetProgramInfoLog(ID, 512, NULL, infoLog);
         std::cout << "Shader linking failed:\n" << infoLog << std::endl;
      }

      glDeleteShader(vertex);
      glDeleteShader(fragment);
   }

   void use() {
      glUseProgram(ID);
   }

   void setBool(const std::string& name, bool value) const {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
   }

   void setInt(const std::string& name, int value) const {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
   }

   void setFloat(const std::string& name, float value) const {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
   }

   void setMat4(const std::string& name, const glm::mat4& mat) const {
      glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
   }

   void setVec3(const std::string& name, const glm::vec3& value) const {
      int location = glGetUniformLocation(ID, name.c_str());
      if (location != -1) glUniform3fv(location, 1, &value[0]);
   }

   void setVec4(const std::string& name, const glm::vec4& value) const {
      int location = glGetUniformLocation(ID, name.c_str());
      if (location != -1) glUniform4fv(location, 1, &value[0]);
   }

};