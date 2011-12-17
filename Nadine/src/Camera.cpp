// Camera.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#include "Camera.hpp"
#include "Tools.hpp"

#include <iostream>
#include <cmath>



// Default constructor
Camera::Camera()
{
    this->init();
}

// Cleans memory for Camera
Camera::~Camera()
{

}


// Inits parameters for the camera
void Camera::init()
{
    // View Data
    // Camera position and orientation
    this->c[0]=0.0;  this->c[1]=0.0; this->c[2]=1.0; // Camera position    
    this->x[0]=1.0;  this->x[1]=0.0; this->x[2]=0.0; normalize(this->x); // x axis
    this->y[0]=0.0;  this->y[1]=1.0; this->y[2]=0.0; normalize(this->y); // y axis
    this->z[0]=0.0;  this->z[1]=0.0; this->z[2]=1.0; normalize(this->z); // z axis
    setToIdentity(this->view);
    this->updateView();
    
    // Projection data
    // Projection type : perspective:true / orthographic:false
    this->perspectiveProjection=false;
    // Projection frustum data
    GLfloat l=1.0;
    this->left=-l; 
    this->right=l;
    this->bottom=-l;
    this->top=l; 
    this->near=0.4;/*0.1 à l'origine*/
    this->far=100;/*100 à l'origine*/
    setToIdentity(this->projection);
    updateProjection();
}


// Stores the data necassary to evaluate the projection matrix
void Camera::setProjectionData(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
    this->left=left;
    this->right=right;
    this->bottom=bottom;
    this->top=top;
    this->near=near;
    this->far=far;
}

void Camera::setPerspectiveFromAngle(GLfloat fovy, GLfloat aspectRatio) {
    this->top = this->near * tan(fovy/2.0);
    this->bottom = -this->top;
    this->left = this->bottom * aspectRatio;
    this->right = this->top * aspectRatio;
}

// Turns camera projection from perspective to ortho or inverse
void Camera::switchCameraProjection()
{
    std::cout<<"Camera projection : ";
	if (this->perspectiveProjection)
    {
        std::cout<<"orthographic"<<std::endl;
		this->perspectiveProjection=false;
    }
	else
    {
        std::cout<<"perspective"<<std::endl;
		this->perspectiveProjection=true;
    }

    // Changes the matrix accordingly
    updateProjection();
}


// Updates view
void Camera::updateView()
{
    // Rotation to be aligned with correct camera axis
    GLfloat RcInv[]={this->x[0], this->y[0], this->z[0], 0.0,
                     this->x[1], this->y[1], this->z[1], 0.0,
                     this->x[2], this->y[2], this->z[2], 0.0,
                     0.0,          0.0,      0.0,        1.0};

    // Translation to be at the right distance from the scene
    GLfloat TcInv[]={1.0,           0.0,           0.0,           0.0,
                     0.0,           1.0,           0.0,           0.0,
                     0.0,           0.0,           1.0,           0.0,
                     -this->c[0],   -this->c[1],   -this->c[2],   1.0};

    // Initializes
    setToIdentity(this->view);
    // Rotates
    multMatrixBtoMatrixA(this->view, RcInv); 
    // Translates
    multMatrixBtoMatrixA(this->view, TcInv);
}


// Updates the projection matrix from the data
void Camera::updateProjection()
{  
    GLfloat l=this->left;
    GLfloat r=this->right;
    GLfloat b=this->bottom;
    GLfloat t=this->top;
    GLfloat n=this->near;
    GLfloat f=this->far;

    if (this->perspectiveProjection) // Perspective projection
    {
        GLfloat P[]={ (2.0*n)/(r-l), 0.0,           0.0,              0.0,
                      0.0,           (2.0*n)/(t-b), 0.0,              0.0,
                      (r+l)/(r-l),   (t+b)/(t-b),   -(f+n)/(f-n),    -1.0,
                      0.0,           0.0,           -(2.0*f*n)/(f-n), 0.0}; // Perspective projection
        for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
            this->projection[iMatrixCoord]=P[iMatrixCoord];
    }
    else // Orthographic projection
    { 
        GLfloat P[]={ 2.0/(r-l),   0.0,         0.0,         0.0,
                      0.0,         2.0/(t-b),   0.0,         0.0,
                      0.0,         0.0,         -2.0/(f-n),  0.0,
                    -(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1.0}; // Orthographic projection
        for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
            this->projection[iMatrixCoord]=P[iMatrixCoord];
    }
}


void Camera::lookAt(GLfloat * c, GLfloat * aim, GLfloat * up)
{
    for (GLuint i=0 ; i<3; i++)
    {
        this->c[i]=c[i]; // centre de la camera
        this->y[i]=up[i]; // vecteur up
        this->z[i]=c[i]-aim[i]; // créer le vecteur z
    }

    GLfloat zOpp[]={-this->z[0], -this->z[1], -this->z[2]};

    vectorProduct(this->y, this->z, this->x);

    vectorProduct(this->x, zOpp, this->y);

    // On normalise
    normalize(this->x);
    normalize(this->y);
    normalize(this->z);

    // Construire la nouvelle matrice view
    updateView();
}

