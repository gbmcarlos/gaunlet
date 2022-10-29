struct EntityPropertySet {
    mat4 transform;
    vec4 color;
    uint textureIndex;
    int entityId;
};

layout (std140) uniform EntityPropertySets {
    EntityPropertySet entityPropertySets[100];
};