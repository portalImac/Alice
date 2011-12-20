// lightingShader.glsl
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#ifdef _VERTEX_

// Attributes : per vertex data
attribute vec4 vertexPosition;
attribute  vec3 vertexNormal;
attribute  vec2 vertexUvs;
attribute  vec4 vertexColor;

// Varyings : data to transmit to fragments
varying vec4 position;
varying vec4 normal;
varying vec2 uvs;
varying vec4 localColor;

void main()
{
    if (filledData[0]) position = model * vertexPosition;
    if (filledData[1]) normal = model * vec4(vertexNormal, 0.0);
    if (filledData[2]) uvs = vertexUvs;
    if (filledData[3]) localColor = vertexColor;

    gl_Position = projection * view * model * vertexPosition;
}

#endif




#ifdef _FRAGMENT_


// Varyings : data receved and interpolated from the vertex shaders
varying vec4 position;
varying vec4 normal;
varying vec2 uvs;
varying vec4 localColor;

void main()
{
    vec4 diffuseColorMix=vec4(material.diffuse);
    // If color
    if (filledData[3]) diffuseColorMix=mix(localColor, material.diffuse, 0.3);
    
    // If no normal
    if (!filledData[1]) gl_FragColor = diffuseColorMix;
    else
    {
        vec4 L=normalize(light.position); // Direction of light from fragment -> light.position[3]==0.0 : Directional light
        if (light.position.w==1.0) L=normalize(light.position-position); //   -> light.position[3]==1.0 : Point light
        vec4 V=normalize(eye-position); // Direction from fragment to eye
        vec4 R=normalize(reflect(-L, normal)); // Direction of reflected light beam, from fragment
        vec4 N=normalize(normal); // Normal
    
        float ambientValue=light.power;
        float diffuseValue=light.power * max( dot(L, N), 0.0);
        float specularValue=light.power * pow( max(dot(R, V), 0.0), material.shininess);
        vec4 ambientContribution=vec4(ambientValue*material.ka*material.ambient.rgb, material.ambient.a);
        vec4 diffuseContribution=vec4(diffuseValue*material.kd*diffuseColorMix.rgb, diffuseColorMix.a);
        vec4 specularContribution=vec4(specularValue*material.ks*material.specular.rgb, material.specular.a);
  
        gl_FragColor = ambientContribution + diffuseContribution + specularContribution;
    }
}

#endif
