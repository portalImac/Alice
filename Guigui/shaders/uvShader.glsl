// lightingShader.glsl
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#version 150


// Uniforms : data shared by every shader

#ifdef _VERTEX_

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Attributes : per vertex data
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUv;
in vec4 vertexColor;

// Varyings : data to transmit to fragments
smooth out vec4 localColor;

void main()
{
    //localColor = vec4(vertexUv,0,1);
    localColor = vec4((vertexNormal+1)/2,1);

    gl_Position = projection * view * model * vec4(vertexPosition,1);
}

#endif




#ifdef _FRAGMENT_


// Varyings : data receved and interpolated from the vertex shaders
smooth in vec4 localColor;

// Final output
out vec4 fragColor;

void main()
{
    fragColor = localColor;
}

#endif
