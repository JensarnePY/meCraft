#version 460 core

struct Vertex {
    float position[3];
    float texUV[2];
    float face;
    float blockID;
};

layout(binding = 0, std430) readonly buffer ssboVertices {
    Vertex data[];
};

layout(binding = 1, std430) readonly buffer ssboIndices {
    uint indices[];
};

uniform mat4 camMatrix;
uniform mat4 model;

out vec2 texCoord;
out float face;
out float blockID;

vec3 getPosition(int index) {
    return vec3(data[index].position[0],
                data[index].position[1],
                data[index].position[2]);
}

vec2 getUV(int index) {
    return vec2(data[index].texUV[0],
                data[index].texUV[1]);
}

void main()
{
    int v = int(indices[gl_VertexID]);

    vec3 worldPos = vec3(model * vec4(getPosition(v), 1.0));
    texCoord = getUV(v);
    face     = data[v].face;
    blockID  = data[v].blockID;

    gl_Position = camMatrix * vec4(worldPos, 1.0);
}