#version 410 core

layout (vertices = 4) out;

struct EntityPropertySet {
    mat4 transform;
    vec4 color;
    uint textureIndex;
    float tessellationLevel;
    float minTessellationLevel;
    float maxTessellationLevel;
    float minCameraDistance;
    float maxCameraDistance;
    int entityId;
};

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

// Uniforms
layout (std140) uniform EntityPropertySets {
    EntityPropertySet entityPropertySets[100];
};

layout (std140) uniform SceneProperties {
    mat4 view;
    mat4 projection;
    DirectionalLight directionalLight;
};

in vec2 v_textureCoordinates[];
in vec3 v_normal[];
flat in uint v_entityIndex[];

out vec2 tc_textureCoordinates[];
out vec3 tc_normal[];
flat out uint tc_entityIndex[];

void main() {

    tc_textureCoordinates[gl_InvocationID] = v_textureCoordinates[gl_InvocationID];
    tc_entityIndex[gl_InvocationID] = v_entityIndex[gl_InvocationID];
    tc_normal[gl_InvocationID] = v_normal[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {

        uint entityIndex = v_entityIndex[gl_InvocationID];

        if (entityPropertySets[entityIndex].tessellationLevel > 0) {

            gl_TessLevelOuter[0] = entityPropertySets[entityIndex].tessellationLevel;
            gl_TessLevelOuter[1] = entityPropertySets[entityIndex].tessellationLevel;
            gl_TessLevelOuter[2] = entityPropertySets[entityIndex].tessellationLevel;
            gl_TessLevelOuter[3] = entityPropertySets[entityIndex].tessellationLevel;

            gl_TessLevelInner[0] = entityPropertySets[entityIndex].tessellationLevel;
            gl_TessLevelInner[1] = entityPropertySets[entityIndex].tessellationLevel;

        } else {

            float minTessellationLevel = entityPropertySets[entityIndex].minTessellationLevel;
            float maxTessellationLevel = entityPropertySets[entityIndex].maxTessellationLevel;
            float minCameraDistance = entityPropertySets[entityIndex].minCameraDistance;
            float maxCameraDistance = entityPropertySets[entityIndex].maxCameraDistance;

            mat4 modelTransform = entityPropertySets[entityIndex].transform;

            vec4 vertexPosition0 = view * modelTransform * gl_in[0].gl_Position;
            vec4 vertexPosition1 = view * modelTransform * gl_in[1].gl_Position;
            vec4 vertexPosition2 = view * modelTransform * gl_in[2].gl_Position;
            vec4 vertexPosition3 = view * modelTransform * gl_in[3].gl_Position;

            float distance0 = clamp( (abs(vertexPosition0.z) - minCameraDistance) / (maxCameraDistance - minCameraDistance), 0.0, 1.0 );
            float distance1 = clamp( (abs(vertexPosition1.z) - minCameraDistance) / (maxCameraDistance - minCameraDistance), 0.0, 1.0 );
            float distance2 = clamp( (abs(vertexPosition2.z) - minCameraDistance) / (maxCameraDistance - minCameraDistance), 0.0, 1.0 );
            float distance3 = clamp( (abs(vertexPosition3.z) - minCameraDistance) / (maxCameraDistance - minCameraDistance), 0.0, 1.0 );

            float tessellationLevel0 = mix(maxTessellationLevel, minTessellationLevel, min(distance2, distance0) );
            float tessellationLevel1 = mix(maxTessellationLevel, minTessellationLevel, min(distance0, distance1) );
            float tessellationLevel2 = mix(maxTessellationLevel, minTessellationLevel, min(distance1, distance3) );
            float tessellationLevel3 = mix(maxTessellationLevel, minTessellationLevel, min(distance3, distance2) );

            gl_TessLevelOuter[0] = tessellationLevel0;
            gl_TessLevelOuter[1] = tessellationLevel1;
            gl_TessLevelOuter[2] = tessellationLevel2;
            gl_TessLevelOuter[3] = tessellationLevel3;

            gl_TessLevelInner[0] = max(tessellationLevel1, tessellationLevel3);
            gl_TessLevelInner[1] = max(tessellationLevel0, tessellationLevel2);

        }

    }

}
