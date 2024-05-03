#version 430 core

layout (location=0) out vec2 textureCoordsOut;

void main()
{
    // Generating Vertices on the GPU
    // mostly because we have a 2D Engine

    // OpenGL Coordinates
    // -1/ 1                1/ 1
    // -1/-1                1/-1
    vec2 vertices[6] =
    {
        // Top Left
        vec2(-1.0,  1.0),

        // Bottom Left
        vec2(-1.0, -1.0),

        // Top Right
        vec2( 1.0,  1.0),

        // Top Right
        vec2( 1.0,  1.0),

        // Bottom Left
        vec2(-1.0, -1.0),

        // Bottom Right
        vec2( 1.0, -1.0)
    };

    float left = 0.0;
    float top = 0.0; // Change top to 0.0
    float right = 1.0;
    float bottom = 1.0; // Change bottom to 1.0

    vec2 textureCoords[6] = {
        vec2(left,top),
        vec2(left,bottom),
        vec2(right,top),
        vec2(right,top),
        vec2(left,bottom),
        vec2(right,bottom)
    };

    gl_Position = vec4(vertices[gl_VertexID], 1.0, 1.0);
    textureCoordsOut = textureCoords[gl_VertexID];
}
