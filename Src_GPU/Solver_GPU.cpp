#define SIZE ((N+2)*(N+2)*(N+2))
#ifndef NB_ITERATION_SOLVE
#define NB_ITERATION_SOLVE 20
#endif

#include "Solver_GPU.h"

#include "SolverParam.h"

#include <cmath>

#include <cstdlib>
#include <ctime>

#include <unistd.h>


#define PRECISION_RESOLUTION 20


Solver_GPU::Solver_GPU( int width, int height, int depth ) 
	: _grille_width(width), _grille_height(height), _grille_depth(depth) {

	// init des params
	SolverParam::initSolverParam();

	// Shader resolution lineaire
	shader_add_sources = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/add_sources.frag");

	shader_linear_solve = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/linear_solve.frag");
    
	shader_advect_cool = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/advect_cool.frag");
    
	    
	// Frame buffer
	buffer = new Framebuffer(width, height, depth);
    
	// Grilles ( textures )
	_grille_sources = new Texture3D();
	_grille_feu_courante = new Texture3D();
	_grille_feu_dest = new Texture3D();
	_grille_vitesse_courante = new Texture3D();
	_grille_vitesse_dest = new Texture3D();
    
    
	srand ( time(NULL) );    
	float coeff1 = rand()/(float)RAND_MAX;
	float coeff2 = rand()/(float)RAND_MAX;
	float coeff3 = rand()/(float)RAND_MAX;
    
    /* Init des textures */
	float *texture = new float[_grille_width*_grille_height*_grille_depth*4];
	float *ptr;
	// init de la texture feu courante
	ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){    
				*ptr = 0.0;
				ptr++;
				*ptr = 0.0;
				ptr++;
				*ptr = 0.0;
				ptr++;
				*ptr = 1.0f;
				ptr++;
			}
		}
	} 
	
	
    /*	
	ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){    
			    
			    if (    i <= _grille_depth/3 
			        ||  i >= 2*_grille_depth/3    
			        ||  j <= _grille_height/3 
			        ||  j >= 2*_grille_height/3 
			        ||  k <= _grille_width/3 
			        ||  k >= 2*_grille_width/3 
			        ){
			            ptr++;
				        ptr++;
				        ptr++;
				        ptr++;
			        }
			        else{
			            coeff1 = rand()/(float)RAND_MAX;
				        *ptr = i/(float)_grille_depth * coeff1;
				        ptr++;
				        *ptr = 0.0;//j/(float)_grille_height * coeff2;
				        ptr++;
				        *ptr = 0.0;//k/(float)_grille_width * coeff3;
				        ptr++;
				        *ptr = 1.0f;
				        ptr++;
				}
			}
		}
	} 
	*/
	
	_grille_feu_courante->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);
	
	// init de la texture dest
	
	
	_grille_feu_dest->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);
        

	// init de la texture feu courante
	ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){    
				if ( i > j &&  k > _grille_depth /2 && k < 2 * _grille_depth/2)
				{
				    *ptr = 0.142;
				    ptr++;
				    *ptr = 0.142;
				    ptr++;
				    *ptr = 0.142;
				    ptr++;
				    *ptr = 1.0f;
				    ptr++;
				}
				else
				{
				    *ptr = 0.0;
				    ptr++;
				    *ptr = 0.0;
				    ptr++;
				    *ptr = 0.0;
				    ptr++;
				    *ptr = 1.0f;
				    ptr++;
				}
			}
		}
	} 
		
	_grille_sources->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	
	_grille_vitesse_courante->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	
	_grille_vitesse_dest->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	
	
	// liberation
	delete(texture);
	
	
}

Solver_GPU::~Solver_GPU(){
	delete buffer;
	delete shader_linear_solve;    
	delete _grille_feu_courante;
	delete _grille_feu_dest;
	delete _grille_sources;
}



// SWAP
void Solver_GPU::swapGrilles(Texture3D** t1, Texture3D** t2){
    Texture3D* tmp;
    tmp = *t1;
    *t1 = *t2;
    *t2 = tmp;
}
 
 
 
 
 
 

// ACCESSEURS



const GLuint Solver_GPU::getDensities() const {
	return _grille_feu_courante->get_texture_id();
}

const GLuint Solver_GPU::getDestDensities() const {
	return _grille_feu_dest->get_texture_id();
}

const GLuint Solver_GPU::getSources() const {
	return _grille_sources->get_texture_id();
}





// DENSITI STEP

void Solver_GPU::addSource ( float dt ){
    // ajouter dt * source à sortie

    shader_advect_cool->Bind_Program();          

    shader_advect_cool->lierFloat("dt", dt);
        
    _grille_feu_courante->bindTexture(GL_TEXTURE0);
    _grille_sources->bindTexture(GL_TEXTURE1);
        
    shader_add_sources->lierLevel("texture_densite", 0);
    shader_add_sources->lierLevel("texture_sources", 1);
            	
    glActiveTexture(GL_TEXTURE0);
        
    buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_feu_courante);
        
    //swapGrilles(&_grille_feu_dest, &_grille_feu_courante);
        
    shader_add_sources->Unbind_Program();          
	
    
}


void Solver_GPU::linearSolve ( int b, float dt ){

    float arf = dt*_grille_width*_grille_width;

	float a1= arf * SolverParam::getDiffusionParamFire();
	float a2= arf * SolverParam::getDiffusionParamSmoke();
	float a3= arf * SolverParam::getDiffusionParamTemperature();

    Vecteur3D a = Vecteur3D(a1,a2,a3);
    Vecteur3D c = Vecteur3D(1.0, 1.0, 1.0) + 6 * a;
    
    shader_linear_solve->Bind_Program();          

    shader_linear_solve->lierFloat("taille_width",  _grille_width);
    shader_linear_solve->lierFloat("taille_height", _grille_height);
    shader_linear_solve->lierFloat("taille_depth",  _grille_depth);
    shader_linear_solve->lierVecteur("a", a);
    shader_linear_solve->lierVecteur("c", c);


    for ( int i = 0; i < PRECISION_RESOLUTION; i++){
    
        
        _grille_feu_courante->bindTexture(GL_TEXTURE0);
        _grille_feu_dest->bindTexture(GL_TEXTURE1);
        
        shader_linear_solve->lierLevel("texture_entree", 0);
        shader_linear_solve->lierLevel("texture_sortie", 1);
            	
    	glActiveTexture(GL_TEXTURE0);
        
        buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_feu_dest);
        
        swapGrilles(&_grille_feu_dest, &_grille_feu_courante);
    
    }    
    
    shader_linear_solve->Unbind_Program();          
    
}

void Solver_GPU::diffuse ( float dt ){
    linearSolve(0, dt);
}





void advect_cool ( int b, 
		           float * d, float * d0, float *f, float *f0, 
		           float *T, float *T0,
		           float * u, float * v, float *w, 
		           float cool, float consume, float dt )
{



    shader_add_sources->Bind_Program();          

    shader_add_sources->lierFloat("dt", dt);
        
    _grille_feu_courante->bindTexture(GL_TEXTURE0);
    _grille_sources->bindTexture(GL_TEXTURE1);
        
    shader_add_sources->lierLevel("texture_densite", 0);
    shader_add_sources->lierLevel("texture_sources", 1);
            	
    glActiveTexture(GL_TEXTURE0);
        
    buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_feu_courante);
        
    //swapGrilles(&_grille_feu_dest, &_grille_feu_courante);
        
    shader_add_sources->Unbind_Program();          
	






}



void project ( int N, float * u, float * v, float *w, float * p, float * div )
{
}

void vorticity_confinement( int N, float *u, float *v, float *w, 
			    float *u0, float *v0, float *w0, float *T0, 
			    float vc_eps, float dt)
{
}

void Solver_GPU::densitiesStepWithTemp ( float dt )
{	
    
    // Sources ( temp et dens )
    addSource(dt);
    // diffuse le feu, la fumee, la temperature    
	diffuse ( dt );
	// advect
	//
    
   
    
}



void Solver_GPU::velocitiesStepWithTemp ( float dt )
{
}


 
 
 
 
 
 
 
 
void addSourceCorrection ( int N, float *x , float *f, float *T, float *s , float sub, float fireToSmoke, float dt ){
}

void addBuoyancy( int N, float *T, float *v, float buoy, float dt){
}

void setBoundaries ( int N, int b, float *x ) {
}

void setBoundariesB0 ( int N, float *x ) {
}
