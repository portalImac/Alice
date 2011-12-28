
// A few additional math fonctions



// Returns the dot product between a and b
GLfloat dotProduct(GLfloat * a, GLfloat * b)
{
    GLfloat result=0.0;
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
    {
        result+=a[iCoord]*b[iCoord];
    }
    return result;
}


// Returns the scalar triple product between a, b and c
GLfloat scalarTriple(GLfloat * a, GLfloat * b, GLfloat * c)
{
    GLfloat result[4];
    vectorProduct (b, c, result);
    return dotProduct(a, result);
}


// Multiplies a matrix to a point/vector
void multMatrixToPoint(float * pos, float * mat)
{
    float newPos[4];
    newPos[0]=pos[0]*mat[0]+pos[1]*mat[4]+pos[2]*mat[8] +pos[3]*mat[12];
    newPos[1]=pos[0]*mat[1]+pos[1]*mat[5]+pos[2]*mat[9] +pos[3]*mat[13];
    newPos[2]=pos[0]*mat[2]+pos[1]*mat[6]+pos[2]*mat[10]+pos[3]*mat[14];
    newPos[3]=pos[0]*mat[3]+pos[1]*mat[7]+pos[2]*mat[11]+pos[3]*mat[15];
    pos[0]=newPos[0];
    pos[1]=newPos[1];
    pos[2]=newPos[2];
    if (pos[3]!=0.0)
    {
        pos[0]/=newPos[3];
        pos[1]/=newPos[3];
        pos[2]/=newPos[3];
        pos[3]/=newPos[3];
    }
}


// Builds transposed matrix
void setToTranspose(float * M, float * Mt)
{
    for (GLuint iLines=0 ; iLines<4 ; iLines++)
        for (GLuint iColumn=0 ; iColumn<4 ; iColumn++)
            Mt[iLines*4+iColumn]=M[iColumn*4+iLines];
}


// Expensive matrix inversion
void getInverseGenericMatrix(float * A, float * B)
{
	B[0]  =  A[5]*A[10]*A[15] - A[5]*A[14]*A[11] - A[6]*A[9]*A[15] + A[6]*A[13]*A[11] + A[7]*A[9]*A[14] - A[7]*A[13]*A[10];
	B[1]  = -A[1]*A[10]*A[15] + A[1]*A[14]*A[11] + A[2]*A[9]*A[15] - A[2]*A[13]*A[11] - A[3]*A[9]*A[14] + A[3]*A[13]*A[10];
	B[2]  =  A[1]*A[6]*A[15]  - A[1]*A[14]*A[7]  - A[2]*A[5]*A[15] + A[2]*A[13]*A[7]  + A[3]*A[5]*A[14] - A[3]*A[13]*A[6];
	B[3]  = -A[1]*A[6]*A[11]  + A[1]*A[10]*A[7]  + A[2]*A[5]*A[11] - A[2]*A[9]*A[7]   - A[3]*A[5]*A[10] + A[3]*A[9]*A[6];
	B[4]  = -A[4]*A[10]*A[15] + A[4]*A[14]*A[11] + A[6]*A[8]*A[15] - A[6]*A[12]*A[11] - A[7]*A[8]*A[14] + A[7]*A[12]*A[10];
	B[5]  =  A[0]*A[10]*A[15] - A[0]*A[14]*A[11] - A[2]*A[8]*A[15] + A[2]*A[12]*A[11] + A[3]*A[8]*A[14] - A[3]*A[12]*A[10];
	B[6]  = -A[0]*A[6]*A[15]  + A[0]*A[14]*A[7]  + A[2]*A[4]*A[15] - A[2]*A[12]*A[7]  - A[3]*A[4]*A[14] + A[3]*A[12]*A[6];
	B[7]  =  A[0]*A[6]*A[11]  - A[0]*A[10]*A[7]  - A[2]*A[4]*A[11] + A[2]*A[8]*A[7]   + A[3]*A[4]*A[10] - A[3]*A[8]*A[6];
	B[8]  =  A[4]*A[9]*A[15]  - A[4]*A[13]*A[11] - A[5]*A[8]*A[15] + A[5]*A[12]*A[11] + A[7]*A[8]*A[13] - A[7]*A[12]*A[9];
	B[9]  = -A[0]*A[9]*A[15]  + A[0]*A[13]*A[11] + A[1]*A[8]*A[15] - A[1]*A[12]*A[11] - A[3]*A[8]*A[13] + A[3]*A[12]*A[9];
	B[10] =  A[0]*A[5]*A[15]  - A[0]*A[13]*A[7]  - A[1]*A[4]*A[15] + A[1]*A[12]*A[7]  + A[3]*A[4]*A[13] - A[3]*A[12]*A[5];
	B[11] = -A[0]*A[5]*A[11]  + A[0]*A[9]*A[7]   + A[1]*A[4]*A[11] - A[1]*A[8]*A[7]   - A[3]*A[4]*A[9]  + A[3]*A[8]*A[5];
	B[12] = -A[4]*A[9]*A[14]  + A[4]*A[13]*A[10] + A[5]*A[8]*A[14] - A[5]*A[12]*A[10] - A[6]*A[8]*A[13] + A[6]*A[12]*A[9];
	B[13] =  A[0]*A[9]*A[14]  - A[0]*A[13]*A[10] - A[1]*A[8]*A[14] + A[1]*A[12]*A[10] + A[2]*A[8]*A[13] - A[2]*A[12]*A[9];
	B[14] = -A[0]*A[5]*A[14]  + A[0]*A[13]*A[6]  + A[1]*A[4]*A[14] - A[1]*A[12]*A[6]  - A[2]*A[4]*A[13] + A[2]*A[12]*A[5];
	B[15] =  A[0]*A[5]*A[10]  - A[0]*A[9]*A[6]   - A[1]*A[4]*A[10] + A[1]*A[8]*A[6]   + A[2]*A[4]*A[9]  - A[2]*A[8]*A[5];	

	float det = A[0]*B[0] + A[4]*B[1] + A[8]*B[2] + A[12]*B[3];
	for (unsigned int i=0; i<16; ++i)
	    B[i]=B[i]/det;
}





//______________________________________________________________________________
// In case the FBO causes pain



// Prints potential errors in FBO initialization
void getFBOErrors()
{
    GLenum fboStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        switch (fboStatus)
        {
            case GL_FRAMEBUFFER_UNDEFINED:
                std::cout<<"Oops, no window exists?"<<std::endl;
            break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                std::cout<<"Check the status of each attachment"<<std::endl;
            break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                std::cout<<"Attach at least one buffer to the FBO"<<std::endl;
            break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                std::cout<<"Check that all attachments enabled via glDrawBuffers exist in FBO"<<std::endl;
                
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                std::cout<<"Check that the buffer specified via glReadBuffer exists in FBO"<<std::endl;
            break;
            
            case GL_FRAMEBUFFER_UNSUPPORTED:
                std::cout<<"Reconsider formats used for attached buffers"<<std::endl;
            break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                std::cout<<"Make sure the number of samples for each attachment is the same"<<std::endl;
            break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                std::cout<<"Make sure the number of layers for each attachment is the same"<<std::endl;
            break;
        }
    }
}




//______________________________________________________________________________
// A quick cube



// // Builds a cube
void buildCube(Object * object)
{
    std::cout<<"       - cube"<<std::endl;

    object->nbVertices=4*6;
    object->nbIndices=6*6;

    GLfloat L=1.0;
    GLuint i=0;
    GLfloat A[]={ L,  L,  L, 1.0}; normalize(A);
    GLfloat B[]={ L,  L, -L, 1.0}; normalize(B);
    GLfloat C[]={-L,  L, -L, 1.0}; normalize(C);
    GLfloat D[]={-L,  L,  L, 1.0}; normalize(D);
    GLfloat E[]={ L, -L,  L, 1.0}; normalize(E);
    GLfloat F[]={ L, -L, -L, 1.0}; normalize(F);
    GLfloat G[]={-L, -L, -L, 1.0}; normalize(G);
    GLfloat H[]={-L, -L,  L, 1.0}; normalize(H);

    GLuint a0=i++; GLuint a1=i++; GLuint a2=i++;
    GLuint b0=i++; GLuint b1=i++; GLuint b2=i++;
    GLuint c0=i++; GLuint c1=i++; GLuint c2=i++;
    GLuint d0=i++; GLuint d1=i++; GLuint d2=i++;
    GLuint e0=i++; GLuint e1=i++; GLuint e2=i++;
    GLuint f0=i++; GLuint f1=i++; GLuint f2=i++;  
    GLuint g0=i++; GLuint g1=i++; GLuint g2=i++;     
    GLuint h0=i++; GLuint h1=i++; GLuint h2=i++;   
    
	GLfloat vertices[]=
	{A[0], A[1], A[2], A[3],   A[0], A[1], A[2],   A[3],A[0], A[1], A[2], A[3],
	 B[0], B[1], B[2], B[3],   B[0], B[1], B[2],   B[3],B[0], B[1], B[2], B[3],
	 C[0], C[1], C[2], C[3],   C[0], C[1], C[2],   C[3],C[0], C[1], C[2], C[3],
	 D[0], D[1], D[2], D[3],   D[0], D[1], D[2],   D[3],D[0], D[1], D[2], D[3],
	 E[0], E[1], E[2], E[3],   E[0], E[1], E[2],   E[3],E[0], E[1], E[2], E[3],
	 F[0], F[1], F[2], F[3],   F[0], F[1], F[2],   F[3],F[0], F[1], F[2], F[3],
	 G[0], G[1], G[2], G[3],   G[0], G[1], G[2],   G[3],G[0], G[1], G[2], G[3],
	 H[0], H[1], H[2], H[3],   H[0], H[1], H[2],   H[3],H[0], H[1], H[2], H[3]};

	GLuint indices[]={ e0, f0, b0,   b0, a0, e0,
				       g0, h0, d0,   d0, c0, g0,
				       f1, e1, h1,   h1, g1, f1,
				       a1, b1, c1,   c1, d1, a1,
				       h2, e2, a2,   a2, d2, h2,
				       f2, g2, c2,   c2, b2, f2}; 		       
				       
    GLfloat normals[object->nbVertices*3];
	setNormalsFlatTr(object->nbIndices, vertices, indices,  normals);		       
				       
    // Sends the data into buffers on the GPU
    object->sendPrimitives(vertices, indices);
    object->sendNormals(normals);
}





//______________________________________________________________________________
// Intersection fonctions



// Returns true if intersection between ray (from position pos with direction dir)
// and triangle ABC, if true : intersection point in result.
bool intersectRayTriangle(GLfloat * pos, GLfloat * dir, GLfloat * normal, GLfloat * A, GLfloat * B, GLfloat * C,
GLfloat * result)
{
    GLfloat t=(dotProduct(normal, A)
              -dotProduct(normal, pos))/dotProduct(normal, dir);
    if (t<0.0) return false;

    GLfloat pa[4]; GLfloat pb[4]; GLfloat pc[4];
    for (GLuint iCoord=0 ; iCoord<4 ; iCoord++)
    {
        pa[iCoord]=A[iCoord]-pos[iCoord];
        pb[iCoord]=B[iCoord]-pos[iCoord];
        pc[iCoord]=C[iCoord]-pos[iCoord];
    }
    
    // Test intersection against triangle ABC
    GLfloat u=scalarTriple(dir, pc, pb);
    if (u<0.0) return false;
    
    GLfloat v=scalarTriple(dir, pa, pc);
    if (v<0.0) return false;
    
    GLfloat w=scalarTriple(dir, pb, pa);
    if (w<0.0) return false;

    // Compute r, r=u*a+v*b+w*c, from barycentric coordinates (u, v, w)
    GLfloat denom=1.0/(u+v+w);
    u*=denom;
    v*=denom;
    w*=denom; // w=1.0f-u-v;
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        result[iCoord]=u*A[iCoord]+v*B[iCoord]+w*C[iCoord];

    return true;
}

// Returns true if intersection between ray (from position pos with direction dir) 
// and quad ABCD, if true : intersection point in result.
bool intersectRayQuad(GLfloat * pos, GLfloat * dir, GLfloat * normal, GLfloat * A, GLfloat * B, GLfloat * C, GLfloat * D, GLfloat * result)
{
    GLfloat t=(dotProduct(normal, A)
              -dotProduct(normal, pos))/dotProduct(normal, dir);
    if (t<0.0) return false;

    GLfloat pa[4]; GLfloat pb[4]; GLfloat pc[4]; GLfloat pd[4];
    for (GLuint iCoord=0 ; iCoord<4 ; iCoord++)
    {
        pa[iCoord]=A[iCoord]-pos[iCoord];
        pb[iCoord]=B[iCoord]-pos[iCoord];
        pc[iCoord]=C[iCoord]-pos[iCoord];
        pd[iCoord]=D[iCoord]-pos[iCoord];
    }
    // Determine which triangle to test against by testing against diagonal first
    GLfloat v=scalarTriple(dir, pa, pc);
    if (v>=0.0)
    {
        // Test intersection against triangle ABC
        GLfloat u=scalarTriple(dir, pc, pb);
        if (u<0.0) return false;
        
        GLfloat w=scalarTriple(dir, pb, pa);
        if (w<0.0) return false;

        // Compute r, r=u*a+v*b+w*c, from barycentric coordinates (u, v, w)
        GLfloat denom=1.0/(u+v+w);
        u*=denom;
        v*=denom;
        w*=denom; // w=1.0f-u-v;
        for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
            result[iCoord]=u*A[iCoord]+v*B[iCoord]+w*C[iCoord];
    }
    else
    {
        // Test intersection against triangle DAC
        GLfloat u=scalarTriple(dir, pd, pc);
        if (u<0.0) return false;

        float w=scalarTriple(dir, pa, pd);
        if (w<0.0) return false;
        
        v=-v;
        // Compute r, r=u*a+v*d+w*c, from barycentric coordinates (u, v, w)
        GLfloat denom=1.0/(u+v+w);
        u*=denom;
        v*=denom;
        w*=denom; // w=1.0f-u-v;
        for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
            result[iCoord]=u*A[iCoord]+v*D[iCoord]+w*C[iCoord];
    }
    return true;
}







//======================== X-tests ========================
bool axisTestX01(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=a*v0[1]-b*v0[2];
	GLfloat p2=a*v2[1]-b*v2[2];	
	
	GLfloat min, max;
    if (p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[1]+fb*boxHalfSize[2];
	if (min>rad || max<-rad) return false;
	return true;
}
bool axisTestX2(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=a*v0[1]-b*v0[2];
	GLfloat p1=a*v1[1]-b*v1[2];	
	
	GLfloat min, max;
    if (p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[1]+fb*boxHalfSize[2];
	if (min>rad || max<-rad) return false;
	return true;
}

//======================== Y-tests ========================
bool axisTestY02(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=-a*v0[0]+b*v0[2];
	GLfloat p2=-a*v2[0]+b*v2[2];	
	
	GLfloat min, max;
    if (p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[0]+fb*boxHalfSize[2];
	if (min>rad || max<-rad) return false;
	return true;
}
bool axisTestY1(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=-a*v0[0]+b*v0[2];
	GLfloat p1=-a*v1[0]+b*v1[2];	
	
	GLfloat min, max;
    if (p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[0]+fb*boxHalfSize[2];
	if (min>rad || max<-rad) return false;
	return true;
}

//======================== Z-tests ========================
bool axisTestZ12(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p1=a*v1[0]-b*v1[1];
	GLfloat p2=a*v2[0]-b*v2[1];
		
	GLfloat min, max;
    if (p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;}
    
	GLfloat rad=fa*boxHalfSize[0]+fb*boxHalfSize[1];
	if (min>rad || max<-rad) return false;
	return true;
}
bool axisTestZ0(GLfloat a, GLfloat b, GLfloat fa, GLfloat fb, GLfloat * boxHalfSize, GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
	GLfloat p0=a*v0[0]-b*v0[1];
	GLfloat p1=a*v1[0]-b*v1[1];
		
	GLfloat min, max;
    if (p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
    
	GLfloat rad=fa*boxHalfSize[0]+fb*boxHalfSize[1];
	if (min>rad || max<-rad) return false;
	return true;
}


void minMax(GLfloat x0, GLfloat x1, GLfloat x2, GLfloat * min, GLfloat * max)
{
    (*min)=x0; if (x1<(*min)) (*min)=x1; if (x2<(*min)) (*min)=x2;
    (*max)=x0; if (x1>(*max)) (*max)=x1; if (x2>(*max)) (*max)=x2;
}


bool planeBoxOverlap(GLfloat * normal, GLfloat * vert, GLfloat * maxbox)
{
    float vmin[3], vmax[3], v;
    for(GLuint iCoord=0 ; iCoord<3 ; iCoord++)
    {
        v=vert[iCoord];
        if(normal[iCoord]>0.0)
        {
            vmin[iCoord]=-maxbox[iCoord]-v;
            vmax[iCoord]= maxbox[iCoord]-v;
        }
        else
        {
            vmin[iCoord]= maxbox[iCoord]-v;
            vmax[iCoord]=-maxbox[iCoord]-v;
        }
    }
    if (dotProduct(normal, vmin)>0.0) return false;
    if (dotProduct(normal, vmax)>=0.0) return true;
    return false;
}


// Returns true if intersection of Axis Aligned (centred) Bounding Box with ABC triangle
// Uses separating axis theorem to test overlap between triangle and box
// Needs to test for overlap in these directions :
// 1) the {x,y,z}-directions (actually, since we use the AABB of the triangle
//    we do not even need to test these)
// 2) normal of the triangle
// 3) crossproduct(edge from tri, {x,y,z}-direction)
//    this gives 3x3=9 more tests
bool intersectAABBTriangle(GLfloat * boxHalfSize, GLfloat * normal, GLfloat *
A, GLfloat * B, GLfloat * C)
{
    GLfloat fex, fey, fez;
    GLfloat e0[3], e1[3], e2[3];
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
    {
        // Computes triangle edges
        e0[iCoord]=B[iCoord]-A[iCoord]; // tri edge 0
        e1[iCoord]=C[iCoord]-B[iCoord]; // tri edge 1
        e2[iCoord]=A[iCoord]-C[iCoord]; // tri edge 2
    }

    // 3)
    //  Tests the 9 tests first (this was faster)
    fex=fabsf(e0[0]); fey=fabsf(e0[1]); fez=fabsf(e0[2]);
    if (!axisTestX01(e0[2], e0[1], fez, fey, boxHalfSize, A, B, C)) return false;
    if (!axisTestY02(e0[2], e0[0], fez, fex, boxHalfSize, A, B, C)) return false;
    if (!axisTestZ12(e0[1], e0[0], fey, fex, boxHalfSize, A, B, C)) return false;

    fex=fabsf(e1[0]); fey=fabsf(e1[1]); fez=fabsf(e1[2]);
    if (!axisTestX01(e1[2], e1[1], fez, fey, boxHalfSize, A, B, C)) return false;
    if (!axisTestY02(e1[2], e1[0], fez, fex, boxHalfSize, A, B, C)) return false;
    if (!axisTestZ0 (e1[1], e1[0], fey, fex, boxHalfSize, A, B, C)) return false;

    fex=fabsf(e2[0]); fey=fabsf(e2[1]); fez=fabsf(e2[2]);
    if (!axisTestX2 (e2[2], e2[1], fez, fey, boxHalfSize, A, B, C)) return false;
    if (!axisTestY1 (e2[2], e2[0], fez, fex, boxHalfSize, A, B, C)) return false;
    if (!axisTestZ12(e2[1], e2[0], fey, fex, boxHalfSize, A, B, C)) return false;


    // 1)
    // First tests overlap in the {x,y,z}-directions
    // Finds min, max of the triangle each direction, and test for overlap in
    // That direction -- this is equivalent to testing a minimal AABB around
    // the triangle against the AABB
    GLfloat min, max;
    // Tests in the 3 directions
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
    {
        minMax(A[iCoord], B[iCoord], C[iCoord], &min, &max);
        if (min>boxHalfSize[iCoord] || max<-boxHalfSize[iCoord]) return false;
    }

    // 2)
    // Tests if the box intersects the plane of the triangle
    // Computes plane equation of triangle: normal*x+d=0
    if (!planeBoxOverlap(normal, A, boxHalfSize)) return false;

    return true;
}







// Returns code for intersection of Oriented Bounding Box with a plane and the part of space behind it
// returns 0 : fully in positive half-space
// returns 1 : intersecting plane
// returns 2 : fully in negative half-space
GLint intersectOBBHalfPlane(GLfloat * center, GLfloat * OBBModel, GLfloat * boxHalfSize, GLfloat * normal, GLfloat * pointOnPlane)
{
    // Computes the projection interval radius of OBB onto L(t)=c + t* n
    GLfloat r=0.0;
    for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        r+=boxHalfSize[iCoord]*fabsf(dotProduct(normal, &(OBBModel[iCoord*4+0])));

    // Computes distance of box center from plane
    GLfloat d=dotProduct(pointOnPlane, normal);
    GLfloat s=dotProduct(normal, center) - d;
           
    if (s>=r) return 0;  // Fully in posititve half-space
    if (s<=-r) return 2; // Fully in negative half-space
    else return 1;       // Intersecting plane
}


