struct EntityPropertySet {
    mat4 transform;
    vec4 color;
    uint textureIndex;
    int entityId;
    float thickness;
    float fade;
};

layout (std140) uniform EntityPropertySets {
    EntityPropertySet entityPropertySet[100];
};