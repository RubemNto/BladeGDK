#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;
void main() {

  outColor = texture(texSampler, fragTexCoord);
  // test the fragment color strenght
  if (outColor.w <= 0) {
    // discard transparent background fragment
    discard;
  }
}