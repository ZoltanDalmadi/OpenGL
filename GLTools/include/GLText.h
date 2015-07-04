#pragma once
#include <glm\glm.hpp>
#include <string>
#include <map>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "GLVertexArrayObject.h"
#include "GLBufferObject.h"
#include "GLShaderProgram.h"
namespace GLTools
{
class GLText
{
public:
  GLText(const char *type);
  void initialize();
  void draw(GLTools::GLShaderProgram& shaderProgram, std::string text, GLfloat x,
            GLfloat y,
            GLfloat scale);
  ~GLText();


  glm::vec3 color;
private:
  std::string m_type;
  GLTools::GLVertexArrayObject m_VAO;
  GLTools::GLBufferObject m_VBO;

  void initCharacters();
  struct Character
  {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
  };

  std::map<GLchar, Character> Characters;
};

}

