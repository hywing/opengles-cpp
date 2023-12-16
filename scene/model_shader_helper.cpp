#include "model_shader_helper.h"

static const char *vertex_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "attribute vec3 aPos;\n"
      "attribute vec3 aNormal;\n"
      "attribute vec2 aTexCoords;\n"
      "uniform mat4 vMatrix;\n"
      "varying vec2 TexCoords;\n"
      "void main(){\n"
      "   TexCoords = aTexCoords;\n"
      "   gl_Position = vMatrix*vec4(aPos,1);\n"
      "}";

static const char *fragment_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "varying vec2 TexCoords;\n"
      "uniform sampler2D texture_diffuse1;\n"
      "void main() {\n"
      "   gl_FragColor=texture2D(texture_diffuse1,TexCoords);\n"
      "}";

SINGLETON_IMPLEMENT(ModelShaderHelper)
