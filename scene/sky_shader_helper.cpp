#include "sky_shader_helper.h"

static const char *vertex_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "attribute vec3 a_Position;\n"
      "uniform mat4 u_Matrix;\n"
      "varying vec3 v_Position;\n"
      "void main()\n"
      "{\n"
      "   v_Position = a_Position;\n"
      "   v_Position.z = -v_Position.z;\n"
      "   gl_Position = u_Matrix * vec4(a_Position, 1.0);\n"
      "   gl_Position = gl_Position.xyww;\n"
      "}\n";

static const char *fragment_shader
    = "#ifdef GL_ES\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "uniform samplerCube u_TextureUnit;\n"
      "varying vec3 v_Position;\n"
      "void main()\n"
      "{\n"
      "   gl_FragColor = textureCube(u_TextureUnit, v_Position);\n"
      "}\n";

SINGLETON_IMPLEMENT(SkyShaderHelper)
