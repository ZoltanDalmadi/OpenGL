#include "GLText.h"


GLText::GLText(const char *type) : m_type(type)
{
}

void GLText::initialize()
{
  color = glm::vec3(1.0f);
  m_VAO.create();
  m_VBO.setUsage(GLTools::GLBufferObject::Usage::DynamicDraw);
  m_VBO.create();

  m_VAO.bind();
  m_VBO.bind();

  m_VBO.upload(NULL, sizeof(GLfloat) * 6 * 4);


  m_VAO.setAttributeArray(0, 4, 4 * sizeof(GLfloat), 0);

  m_VAO.unbind();

  initCharacters();
}

void GLText::initCharacters()
{
  FT_Library ft;

  if (FT_Init_FreeType(&ft))
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

  FT_Face face;

  if (FT_New_Face(ft, m_type.c_str(), 0, &face))
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

  FT_Set_Pixel_Sizes(face, 0, 48);

  if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

  for (GLubyte c = 0; c < 128; c++)
  {
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }

    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Now store character for later use
    Character character =
    {
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      face->glyph->advance.x
    };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  // Destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void GLText::draw(GLTools::GLShaderProgram& shaderProgram, std::string text,
                  GLfloat x,
                  GLfloat y,
                  GLfloat scale)
{
  // Activate corresponding render state
  shaderProgram.use();
  shaderProgram.setUniformValue("textColor", color.x, color.y,
                                color.z);
  glActiveTexture(GL_TEXTURE0);
  m_VAO.bind();

  // Iterate through all characters
  std::string::const_iterator c;

  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = Characters[*c];

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    // Update VBO for each character
    GLfloat vertices[6][4] =
    {
      { xpos, ypos + h, 0.0, 0.0 },
      { xpos, ypos, 0.0, 1.0 },
      { xpos + w, ypos, 1.0, 1.0 },

      { xpos, ypos + h, 0.0, 0.0 },
      { xpos + w, ypos, 1.0, 1.0 },
      { xpos + w, ypos + h, 1.0, 0.0 }
    };
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // Update content of VBO memory

    m_VBO.bind();
    m_VBO.write(0, vertices, sizeof(vertices));
    m_VBO.unbind();
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.Advance >> 6) *
         scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }

  m_VAO.unbind();
  glBindTexture(GL_TEXTURE_2D, 0);
}

GLText::~GLText()
{
}
