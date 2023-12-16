#include "texture_shader_helper.h"

static const char *vertex_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "attribute vec4 a_Position;\n"
      "attribute vec2 a_TextureCoordinates;\n"
      "varying vec2 v_TextureCoordinates;\n"
      "uniform mat4 u_Matrix;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = u_Matrix * a_Position;\n"
      "   v_TextureCoordinates = a_TextureCoordinates;\n"
      "}\n";

static const char *fragment_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "uniform sampler2D u_TextureUnit;\n"
      "varying vec2 v_TextureCoordinates;\n"
      "void main()\n"
      "{\n"
      "   gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);\n"
      "}\n";

SINGLETON_IMPLEMENT(TextureShaderHelper)
