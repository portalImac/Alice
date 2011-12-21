// lightingShader.glsl
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUvs;
in vec4 vertexColor;

// Varyings : data to transmit to fragments
smooth out vec4 position;
smooth out vec4 normal;
smooth out vec2 uvs;
smooth out vec4 localColor;
smooth out float depth;
smooth out vec4 objectCoordPosition;


void main()
{
    if (filledData[0]) position = model * vertexPosition;
    if (filledData[1]) normal = model * vec4(vertexNormal, 0.0);
    if (filledData[2]) uvs = vertexUvs;
    if (filledData[3]) localColor = vertexColor;

    gl_Position = projection * view * model * vertexPosition;

    depth=(vec3(projection*view*model*vertexPosition)).z;

    objectCoordPosition = vertexPosition;
}

#endif




#ifdef _FRAGMENT_


// Varyings : data receved and interpolated from the vertex shaders
smooth in vec4 position;
smooth in vec4 normal;
smooth in vec2 uvs;
smooth in vec4 localColor;
smooth in float depth;
smooth in vec4 objectCoordPosition;

// Final output
out vec4 fragColor;

void main()
{
    /*if(position[1]<0.0) fragColor=vec4(1.0,0.0,0.0,1.0);
    else fragColor=vec4(0.0,1.0,0.0,1.0);*/

    /*fragColor=vec4(normal.xyz, 1.0);

    float normDepth=depth/2.0;
    fragColor = vec4(normDepth, normDepth, normDepth, 1.0);*/

    // Bandes de couleurs
    vec4 color = vec4(0.0,0.0,0.0,1.0);
    if((mod(trunc(objectCoordPosition.x*40.0), 8.0))==0.0)
	color.r = 1.0;

    if((mod(trunc(objectCoordPosition.y*40.0), 8.0))==0.0)
	color.g = 1.0;

    if((mod(trunc(objectCoordPosition.z*40.0), 8.0))==0.0)
	color.b = 1.0;


    fragColor = color;

}

#endif
