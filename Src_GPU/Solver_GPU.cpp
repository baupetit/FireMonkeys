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
    buffer_courant = 1;
    
    	
	
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

void addSource ( int w, int h, int d, Framebuffer& forign, Framebuffer& fdest, float dt ){

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

void Solver_GPU::diffuse ( Shader& calcul_shader, 
                           string nom_texture, 
                           Framebuffer& originBuffer,
                           Framebuffer& destBuffer,
                           float diff, 
                           float dt ){
                           
    GLuint	location = glGetUniformLocation ( calcul_shader.getProgramId(), nom_texture.c_str());
    glUniform1i(location,originBuffer.get_id_texture());
        
    // Positionnement des buffers 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destBuffer.get_id_buffer());
  
    // Calcul
    int layer_courant = 0;
    while ( _grille_depth - layer_courant > 8){        
        // On attache les layers
        /*
        originBuffer.bind_Buffer();
        originBuffer.attacher_layers_de_la_texture(layer_courant,8);
        originBuffer.unbind_Buffer();
        */
        
        destBuffer.bind_Buffer();
        destBuffer.attacher_layers_de_la_texture(layer_courant,8);
        
        // Calcul
        calcul_shader.Bind_Program();
        dessinerCarre(layer_courant,8);
        calcul_shader.Unbind_Program();
        
        destBuffer.unbind_Buffer();
        
        //dessinerCarre();
        
        // On détache
        originBuffer.detacher_texture();
        destBuffer.detacher_texture();
        
        
        dessinerCarre(layer_courant,8);
        
        // update
        layer_courant+=8;                
        
    } 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
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
    
    diffuse (  *shader_feu, 
               string("feu"), 
               getBufferFeuCourant(), 
               getBufferFeuDestination(),
               diff, 
               dt );
    
    swapBufferCourant(); 
    
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
        if (buffer_courant == 2)
            return *buffer_feu_1;
        else
            return *buffer_feu_2;
    }



    Framebuffer& Solver_GPU::getBufferFeuCourant() const {
        if (buffer_courant == 1)
            return *buffer_feu_1;
        else
            return *buffer_feu_2;
    }
    
    
    void Solver_GPU::swapBufferCourant(){
        if (buffer_courant == 1)
            buffer_courant = 2;
        else
            buffer_courant = 1;
    }
    
    void Solver_GPU::dessinerCarre(int layer_initial, int nb_layers){
    
	glLoadIdentity();
    glPushMatrix();
	glOrtho(0, 1, 1, 0, 0, 1);
	
	glColor4f(0.0f, 2.0f, 0.0f, 0.2f);
	
	Vecteur3D v0 = Vecteur3D(0.0, 0.0, 0.0);
	Vecteur3D v1 = Vecteur3D(0.0, _grille_height, 0.0);
	Vecteur3D v2 = Vecteur3D(_grille_width, _grille_height, 0.0);
	Vecteur3D v3 = Vecteur3D(_grille_width, 0.0, 0.0);
	
    Vecteur3D dirVect = Vecteur3D(0.0, 0.0, 1.0);
    Vecteur3D dirText = Vecteur3D(0.0, 0.0, 1.0);
    Vecteur3D coordVect;
    Vecteur3D coordText;

	glBegin(GL_TRIANGLES);	
	    for(int i = 0; i < nb_layers; i ++){
	        coordVect = v0 + i * dirVect;
	        coordText = v0 + i * dirText;
		    glTexCoord3d(coordText.x, coordText.y, coordText.y);
		    glVertex3d(coordVect.x, coordVect.y, coordVect.y);
	        coordVect = v1 + i * dirVect;
	        coordText = v1 + i * dirText;
		    glTexCoord3d(coordText.x, coordText.y, coordText.y);
		    glVertex3d(coordVect.x, coordVect.y, coordVect.y);
	        coordVect = v2 + i * dirVect;
	        coordText = v2 + i * dirText;
		    glTexCoord3d(coordText.x, coordText.y, coordText.y);
		    glVertex3d(coordVect.x, coordVect.y, coordVect.y);
		    
	        coordVect = v2 + i * dirVect;
	        coordText = v2 + i * dirText;
		    glTexCoord3d(coordText.x, coordText.y, coordText.y);
		    glVertex3d(coordVect.x, coordVect.y, coordVect.y);
	        coordVect = v3 + i * dirVect;
	        coordText = v3 + i * dirText;
		    glTexCoord3d(coordText.x, coordText.y, coordText.y);
		    glVertex3d(coordVect.x, coordVect.y, coordVect.y);
	        coordVect = v0 + i * dirVect;
	        coordText = v0 + i * dirText;
		    glTexCoord3d(coordText.x, coordText.y, coordText.y);
		    glVertex3d(coordVect.x, coordVect.y, coordVect.y);
		}
	glEnd();
	glPopMatrix();
    }
