// shaderTools.glsl
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#version 150


struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float ka;
    float kd;
    float ks;
    float shininess;
};

struct Light
{
    vec4 position;
    float power;
};


// Uniforms : data shared by every shader

uniform mat4 model;
uniform mat4 view;
uniform vec4 eye;
uniform mat4 projection;
uniform Light light;
uniform Material material;
uniform bvec4 filledData; // filledData[0] : true if position, 
                          // filledData[1] : true if normals, 
                          // filledData[2] : true if uvs,
                          // filledData[3] : true if colors.
                          
uniform sampler2D textureUnitDiffuse; // 0
uniform sampler2D textureUnitSpecular; // 1
