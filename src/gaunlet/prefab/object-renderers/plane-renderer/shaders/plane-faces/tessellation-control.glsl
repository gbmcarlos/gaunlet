#version 410 core

layout (vertices = 4) out;

struct EntityPropertySet {
    float leftSizeFactor;
    float bottomSizeFactor;
    float rightSizeFactor;
    float topSizeFactor;
    uint textureIndex;
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

int getTessellationLevel(vec4 edgeStart, vec4 edgeEnd, float sizeFactor, float triangleSize);
float getEdgeProjectedLength(vec4 edgeStart, vec4 edgeEnd);

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

            float triangleSize = 5.0f;
            uint entityIndex = v_entityIndex[gl_InvocationID];

            float tessellationLevelLeft = getTessellationLevel(vertexPosition0, vertexPosition3, entityPropertySets[entityIndex].leftSizeFactor, triangleSize);
            float tessellationLevelBottom = getTessellationLevel(vertexPosition0, vertexPosition1, entityPropertySets[entityIndex].bottomSizeFactor, triangleSize);
            float tessellationLevelRight = getTessellationLevel(vertexPosition1, vertexPosition2, entityPropertySets[entityIndex].rightSizeFactor, triangleSize);
            float tessellationLevelTop = getTessellationLevel(vertexPosition2, vertexPosition3, entityPropertySets[entityIndex].topSizeFactor, triangleSize);

            float tessellationLevelVertical = max(tessellationLevelBottom, tessellationLevelTop);
            float tessellationLevelHorizontal = max(tessellationLevelLeft, tessellationLevelRight);

            gl_TessLevelOuter[0] = tessellationLevelLeft;
            gl_TessLevelOuter[1] = tessellationLevelBottom;
            gl_TessLevelOuter[2] = tessellationLevelRight;
            gl_TessLevelOuter[3] = tessellationLevelTop;

            gl_TessLevelInner[0] = tessellationLevelVertical;
            gl_TessLevelInner[1] = tessellationLevelHorizontal;

        }

    }

}

int getTessellationLevel(vec4 edgeStart, vec4 edgeEnd, float sizeFactor, float triangleSize) {

    float tessellationFactor = getEdgeProjectedLength(edgeStart, edgeEnd);
    tessellationFactor = tessellationFactor / triangleSize;

    // A size factor of 0 means that there is no neighbour
    if (sizeFactor == 0.0f) {
        return 4;
    }

    // A size factor of 0.5f means that the neighbour is smaller, and this quad is bigger
    if (sizeFactor == 0.5f) {
        return 8;
    }

    // A size factor of 1.0f means that this quad and the neighbour are the same size;
    if (sizeFactor == 1.0f) {
        return 4;
    }

    // A size factor of 2.0f means that the neigbour is bigger, and this quad is smaller
    if (sizeFactor == 2.0f) {
        return 2;
    }

}

float getEdgeProjectedLength(vec4 edgeStart, vec4 edgeEnd) {

    vec4 p0 = projection * view * edgeStart;
    vec4 p1 = projection * view * edgeEnd;
    float edgeLength = distance(p0.xy / p0.w, p1.xy / p1.w);
    return edgeLength;

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
