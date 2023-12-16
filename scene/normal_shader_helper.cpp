#include "normal_shader_helper.h"

static const char *vertex_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "attribute vec4 posAttr;\n"
      "attribute vec4 colAttr;\n"
      "varying vec4 col;\n"
      "uniform mat4 matrix;\n"
      "uniform vec3 vKa;\n"
      "uniform vec3 vKd;\n"
      "uniform vec3 vKs;\n"
      "void main()\n"
      "{\n"
      "   col = colAttr;\n"
      "   gl_Position = matrix * posAttr;\n"
      "}\n";

static const char *fragment_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "varying vec4 col;\n"
      "void main()\n"
      "{\n"
      "   gl_FragColor = col;\n"
      "}\n";

SINGLETON_IMPLEMENT(NormalShaderHelper)
