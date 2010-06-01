#define SIZE ((N+2)*(N+2)*(N+2))
#ifndef NB_ITERATION_SOLVE
#define NB_ITERATION_SOLVE 20
#endif

#include "Solver_GPU.h"

#include <cmath>

inline void SWAP( float* &a , float* &b){
	float *tmp = a ;
	a = b ;
	b = tmp ;
}

Solver_GPU::Solver_GPU( int width, int height, int depth ) 
            : _grille_width(width), _grille_height(height), _grille_depth(depth) {
	
	cout << "Nouveau Solver_GPU" << endl;

    // Shader d'advection
    shader_feu = new Shader("./Shaders/feu.vert","./Shaders/feu.frag");
    
    // Frame buffer
    buffer_feu_1 = new Framebuffer();
    buffer_feu_2 = new Framebuffer();
    buffer_feu_courant = 1;
    
    	
	
    // Creation du champs de vitesse vide
    float *texture = new float[_grille_width*_grille_height*_grille_depth*4];
    float *ptr = texture;
    for (int k = 0; k < _grille_width; k++){
        for (int j = 0; j < _grille_height; j++){
            for (int i = 0; i < _grille_depth; i++){    
                *ptr = 1.0f;
                ptr++;
                *ptr = 1.0f;
                ptr++;
                *ptr = 0.0f;
                ptr++;
                *ptr = 0.5f;
                ptr++;
            }
        }
    } 
    
    
    // Quelques valeurs initiales
    ptr = texture;
    for (int k = 0; k < _grille_width/4; k++){
        for (int j = 0; j < _grille_height; j++){
            for (int i = 0; i < _grille_depth ; i++){   
                *ptr = 1.0f;
                ptr++;
                *ptr = 0.0f;
                ptr++;
                *ptr = 1.0f;
                ptr++;
                *ptr = 0.5f;
                ptr++;
            }
        }
    }
    
    // Association du tableau avec le buffer
    buffer_feu_1->initialiser_texture_buf(texture, _grille_width, _grille_height, _grille_depth);      
    buffer_feu_2->initialiser_texture_buf(texture, _grille_width, _grille_height, _grille_depth);      
    
    
    

}

Solver_GPU::~Solver_GPU(){
	delete shader_feu;
    delete buffer_feu_1;
    delete buffer_feu_2;
}

const GLuint Solver_GPU::getDensities() const {
    return getBufferFeuCourant().get_id_texture();
    
}

const GLuint Solver_GPU::getSmokes() const {
    return 0;
}

const GLuint Solver_GPU::getTemperatures() const {
    return 0;
}


int Solver_GPU::getWidth() const{
	return _grille_width ;
}
int Solver_GPU::getHeight() const{
	return _grille_height ;
}
int Solver_GPU::getDepth() const{
	return _grille_depth ;
}

void Solver_GPU::setDensity( int i , int j , int k , float dens ){
	
}

void Solver_GPU::setTemperature( int i, int j, int k, float temp ){
	
}

void Solver_GPU::setVelocity( int i, int j , int k , float u, float v, float w ){
}

void addSource ( int N, float *x , float *s , float dt ){
}

void addSourceCorrection ( int N, float *x , float *f, float *T, float *s , float sub, float fireToSmoke, float dt ){
}


void addBuoyancy( int N, float *T, float *v, float buoy, float dt){
}

void setBoundaries ( int N, int b, float *x ) {
}

void setBoundariesB0 ( int N, float *x ) {
}

void linearSolve ( int N, int b, float * x, float * x0, float a, float c ){
}

void diffuse ( int N, int b, float * x, float * x0, float diff, float dt ){
}

void diffuseFireAndSmoke ( int N, int b1, int b2, int b3,
			   float * x, float * x0, 
			   float * f, float * f0, 
			   float * t, float * t0, 
			   float diffFire, float diffSmoke, float diffTemp, 
			   float dt ){
}

void advect ( int N, int b, float * d, float * d0, float * u, float * v, float *w, float dt ){
} 

void advect_cool ( int N, int b, 
		   float * d, float * d0, float *f, float *f0, 
		   float *T, float *T0,
		   float * u, float * v, float *w, 
		   float cool, float consume, float dt )
{
}


void project ( int N, float * u, float * v, float *w, float * p, float * div )
{
}

void vorticity_confinement( int N, float *u, float *v, float *w, 
			    float *u0, float *v0, float *w0, float *T0, 
			    float vc_eps, float dt)
{
}

void Solver_GPU::densitiesStep ( float diff, float dt )
{	
    ////////////////////////////////////////////////////////////////////////////
    // Traitement du feu
    ////////////////////////////////////////////////////////////////////////////
 
    // liaison de la texture du buffer avec le shader associé
    GLuint	location = glGetUniformLocation ( shader_feu->getProgramId(), "feu");
    glUniform1i(location,getBufferFeuCourant().get_id_buffer());
        
    // Positionnement des buffers 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getBufferFeuDestination().get_id_buffer());
  
    // Calcul
    buffer_feu_layer_courant = 0;
    while ( _grille_depth - buffer_feu_layer_courant > 8){        
        // On attache les layers
        getBufferFeuCourant().attacher_layers_de_la_texture(buffer_feu_layer_courant,8);
        getBufferFeuDestination().attacher_layers_de_la_texture(buffer_feu_layer_courant,8);
        
        // Calcul
        shader_feu -> Bind_Program();
        dessinerCarre();
        shader_feu -> Unbind_Program();
        
        
        // update
        buffer_feu_layer_courant+=8;        
        
        
    } 
    //attacher_layers_de_la_texture(buffer_feu_layer_courant,_grille_depth - buffer_feu_layer_courant);
    
}

void Solver_GPU::densitiesStepWithTemp ( float diffFire,
				     float diffSmoke,
				     float diffTemp,
				     float cool, 
					 float consume,
				     float sub, 
				     float fireToSmoke, 
				     float dt )
{	
}


void Solver_GPU::velocitiesStep ( float visc, float dt )
{
}


void Solver_GPU::velocitiesStepWithTemp ( float visc, float buoy, float vc_eps, float dt )
{
}











    Framebuffer& Solver_GPU::getBufferFeuDestination() const {
        if (buffer_feu_courant == 2)
            return *buffer_feu_1;
        else
            return *buffer_feu_2;
    }



    Framebuffer& Solver_GPU::getBufferFeuCourant() const {
        if (buffer_feu_courant == 1)
            return *buffer_feu_1;
        else
            return *buffer_feu_2;
    }
    
    
    void Solver_GPU::swapBufferFeuCourant(){
        if (buffer_feu_courant == 1)
            buffer_feu_courant = 2;
        else
            buffer_feu_courant = 1;
    }
    
    void Solver_GPU::dessinerCarre(){
    GLfloat verts[4][3] = { { 0.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {1.0, 1.0, 0.5}, {1.0, 0.0, 0.5} };

	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2]);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2]);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2]);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2]);
		
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);		
	glEnd();
    }
