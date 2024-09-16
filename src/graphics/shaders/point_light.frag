#version 450

layout (location = 0) in vec2 fragOffset;
layout (location = 0) out vec4 outColor;

struct PointLight {
    vec4 position; // ignore w
    vec4 color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientColor;
    PointLight pointLights[10];
    int numLights;
} ubo;

layout(push_constant) uniform Push {
    vec4 position;
    vec4 color;
    float radius;
} push;

const float M_PI = 3.1415926538;
void main() {
    float disSqrd = dot(fragOffset, fragOffset);
    if (disSqrd >= 1.0) {
        discard;
    }

    float cosDist = 0.5 * (cos(disSqrd * M_PI) + 1.0);
    outColor = vec4(push.color.xyz + (cosDist / 2), cosDist);
}