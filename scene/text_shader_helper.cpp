#include "text_shader_helper.h"

static const char *vertex_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "attribute vec4 a_Position;\n"
      "varying vec2 v_TextureCoordinates;\n"
      "uniform mat4 u_Matrix;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = u_Matrix * vec4(a_Position.xy, 0.0, 1.0);\n"
      "   v_TextureCoordinates = a_Position.zw;\n"
      "}\n";

static const char *fragment_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "uniform sampler2D u_TextureUnit;\n"
      "varying vec2 v_TextureCoordinates;\n"
      "uniform vec3 textColor;\n"
      "uniform sampler2D text;\n"
      "void main()\n"
      "{\n"
      "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, v_TextureCoordinates).r);\n"
      "   gl_FragColor = vec4(textColor, 1.0) * sampled;\n"
      "}\n";

SINGLETON_IMPLEMENT(TextShaderHelper)
