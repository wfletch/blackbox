#version 450

// Layout location sets storage location where we get from
layout (location = 0) in vec2 position; // This variable is supplied by a vertex buffer.
layout (location = 1) in vec3 color;
layout (push_constant) uniform Push {
  mat2 transform;
  vec2 offset;
  vec3 color;
} push;
/// 
layout(location = 0) out vec3 fragColor;
void main() {
    gl_Position = vec4(push.transform * position + push.offset, 0.0, 1.0);

    fragColor = vec3(push.color);
}