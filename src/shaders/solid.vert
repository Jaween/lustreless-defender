#version 420 core

uniform mat4 mvpMatrix;

layout(location = 0) in vec3 position;

void main()
{
  gl_Position = mvpMatrix * vec4(position, 1.0);
}
