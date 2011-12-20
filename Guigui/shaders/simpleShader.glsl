// lightingShader.glsl
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#version 150


// Uniforms : data shared by every shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;


#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;

// Varyings : data to transmit to fragments


void main()
{
    gl_Position = projection * view * model * vertexPosition;
}

#endif




#ifdef _FRAGMENT_


// Varyings : data receved and interpolated from the vertex shaders


// Final output
out vec4 fragColor;

void main()
{
    fragColor = color;
}

#endif
