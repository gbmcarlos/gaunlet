#version 410 core

layout (vertices = 4) out;

struct EntityPropertySet {
    uint textureIndex;
    float leftSizeFactor;
    float rightSizeFactor;
    float bottomSizeFactor;
    float topSizeFactor;
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

struct FrustumPlane {
    vec3 normal;
    float distance;
};

layout (std140) uniform CameraFrustum {
    FrustumPlane nearPlane;
    FrustumPlane farPlane;
    FrustumPlane leftPlane;
    FrustumPlane rightPlane;
    FrustumPlane bottomPlane;
    FrustumPlane topPlane;
};

uniform float u_targetTessellationLevel;
uniform float u_tessellationLevelSlope;

in vec2 v_textureCoordinates[];
in vec3 v_normal[];
flat in uint v_entityIndex[];

out vec2 tc_textureCoordinates[];
out vec3 tc_normal[];

bool withinFrustum(vec4 corner0, vec4 corner1, vec4 corner2, vec4 corner3);
bool quadOver(FrustumPlane plane, vec4 corner0, vec4 corner1, vec4 corner2, vec4 corner3);
bool pointOver(FrustumPlane plane, vec4 point);

void main() {

    tc_textureCoordinates[gl_InvocationID] = v_textureCoordinates[gl_InvocationID];
    tc_normal[gl_InvocationID] = v_normal[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {

        vec4 vertexPosition0 = gl_in[0].gl_Position;
        vec4 vertexPosition1 = gl_in[1].gl_Position;
        vec4 vertexPosition2 = gl_in[2].gl_Position;
        vec4 vertexPosition3 = gl_in[3].gl_Position;

        if (!withinFrustum(vertexPosition0, vertexPosition1, vertexPosition2, vertexPosition3)) {
            gl_TessLevelOuter[0] = -1;
            gl_TessLevelOuter[1] = -1;
            gl_TessLevelOuter[2] = -1;
            gl_TessLevelOuter[3] = -1;

            gl_TessLevelInner[0] = -1;
            gl_TessLevelInner[1] = -1;

        } else {

            vertexPosition0 = view * vertexPosition0;
            vertexPosition1 = view * vertexPosition1;
            vertexPosition2 = view * vertexPosition2;
            vertexPosition3 = view * vertexPosition3;

            float distance0 = abs(vertexPosition0.z);
            float distance1 = abs(vertexPosition1.z);
            float distance2 = abs(vertexPosition2.z);
            float distance3 = abs(vertexPosition3.z);

            float tessellationLevel0 = max((distance0 * u_tessellationLevelSlope) + 1, u_targetTessellationLevel);
            float tessellationLevel1 = max((distance1 * u_tessellationLevelSlope) + 1, u_targetTessellationLevel);
            float tessellationLevel2 = max((distance2 * u_tessellationLevelSlope) + 1, u_targetTessellationLevel);
            float tessellationLevel3 = max((distance3 * u_tessellationLevelSlope) + 1, u_targetTessellationLevel);

            gl_TessLevelOuter[0] = tessellationLevel0;
            gl_TessLevelOuter[1] = tessellationLevel1;
            gl_TessLevelOuter[2] = tessellationLevel2;
            gl_TessLevelOuter[3] = tessellationLevel3;

            gl_TessLevelInner[0] = max(tessellationLevel1, tessellationLevel3);
            gl_TessLevelInner[1] = max(tessellationLevel0, tessellationLevel2);
        }

    }

}

bool withinFrustum(vec4 corner0, vec4 corner1, vec4 corner2, vec4 corner3) {

    if (!quadOver(nearPlane, corner0, corner1, corner2, corner3)) {
        return false;
    }

    if (!quadOver(farPlane, corner0, corner1, corner2, corner3)) {
        return false;
    }

    if (!quadOver(leftPlane, corner0, corner1, corner2, corner3)) {
        return false;
    }

    if (!quadOver(rightPlane, corner0, corner1, corner2, corner3)) {
        return false;
    }

    if (!quadOver(bottomPlane, corner0, corner1, corner2, corner3)) {
        return false;
    }

    if (!quadOver(topPlane, corner0, corner1, corner2, corner3)) {
        return false;
    }

    return true;

}

bool quadOver(FrustumPlane plane, vec4 corner0, vec4 corner1, vec4 corner2, vec4 corner3) {

    if (pointOver(plane, corner0)) {
        return true;
    }

    if (pointOver(plane, corner1)) {
        return true;
    }

    if (pointOver(plane, corner2)) {
        return true;
    }

    if (pointOver(plane, corner3)) {
        return true;
    }

    return false;

}

bool pointOver(FrustumPlane plane, vec4 point) {

    float distance = dot(plane.normal, vec3(point)) - plane.distance;
    return distance > 0;

}
