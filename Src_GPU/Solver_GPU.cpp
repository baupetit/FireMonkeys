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


#define PRECISION_RESOLUTION 2


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
    
	shader_advect_speed = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/advect_speed.frag");
	    
	// Frame buffer
	buffer = new Framebuffer(width, height, depth);
    
	// Grilles ( textures )
	_grille_feu_sources = new Texture3D();
	_grille_feu_courante = new Texture3D();
	_grille_feu_dest = new Texture3D();
	_grille_vitesse_sources = new Texture3D();
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
	

	// init de la texture initiale
	_grille_feu_courante->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);
	// init de la texture dest
	_grille_feu_dest->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);
	

	// init de la source
	ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){    
				if ( i > _grille_depth/3 && i < 2*_grille_depth/3 &&
				     k < _grille_width /2  &&  k> _grille_height/3 && 
				     j < _grille_height/3 )
				{
				    *ptr = 0.0001;//rand()/(float)RAND_MAX;
				    ptr++;
				    *ptr = 0.0000;//rand()/(float)RAND_MAX;
				    ptr++;
				    *ptr = 0.1;//rand()/(float)RAND_MAX;
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
		
	_grille_feu_sources->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	
	
	ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){    
				if ( i > _grille_depth/3 && i < 2*_grille_depth/3 &&
				     k < _grille_width /2  &&  k> _grille_height/3 && 
				     j < _grille_height/3 )
				{
				    *ptr = ( rand()/(float)RAND_MAX - 0.5 );
				    ptr++;
				    *ptr = rand()/(float)RAND_MAX;
				    ptr++;
				    *ptr = ( rand()/(float)RAND_MAX - 0.5 );
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
	
	_grille_vitesse_sources->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	
	
	
	ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){    
			    /*
				*ptr = 0.01 * rand()/(float)RAND_MAX;
				ptr++;
				*ptr = 0.01 * rand()/(float)RAND_MAX;
				ptr++;
				*ptr = 0.01 * rand()/(float)RAND_MAX;
				ptr++;
				*ptr = 1.0f;
				ptr++;
				*/
				*ptr = 0.00 * rand()/(float)RAND_MAX;
				ptr++;
				*ptr = 0.19 * rand()/(float)RAND_MAX;
				ptr++;
				*ptr = 0.00 * rand()/(float)RAND_MAX;
				ptr++;
				*ptr = 1.0f;
				ptr++;
			}
		}
	} 
	
	
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
	delete _grille_feu_sources;    
	delete _grille_vitesse_courante;
	delete _grille_vitesse_dest;
	delete _grille_vitesse_sources;
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


const GLuint Solver_GPU::getSpeed() const {
	return _grille_vitesse_courante->get_texture_id();
}

const GLuint Solver_GPU::getSources() const {
	return _grille_feu_sources->get_texture_id();
}





// DENSITI STEP

void Solver_GPU::addSource ( float dt, Texture3D& textureCible, Texture3D& textureSource ){


    
    shader_add_sources->Bind_Program();          

    shader_add_sources->lierFloat("dt", dt);
        
    textureCible.bindTexture(GL_TEXTURE0);
    textureSource.bindTexture(GL_TEXTURE1);
        
    shader_add_sources->lierLevel("texture_densite", 0);
    shader_add_sources->lierLevel("texture_sources", 1);
            	
    glActiveTexture(GL_TEXTURE0);
        
    buffer->traiterDessinDansBuffer1ALAFOIS(textureCible);
        
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



void Solver_GPU::linearSolve_speed ( int b, float dt ){


    float arf = dt*_grille_width*_grille_width;

	float a1= arf * SolverParam::getViscosityParam();
	float a2= arf * SolverParam::getViscosityParam();
	float a3= arf * SolverParam::getViscosityParam();

    Vecteur3D a = Vecteur3D(a1,a2,a3);
    Vecteur3D c = Vecteur3D(1.0, 1.0, 1.0) + 6 * a;
    
    shader_linear_solve->Bind_Program();          

    shader_linear_solve->lierFloat("taille_width",  _grille_width);
    shader_linear_solve->lierFloat("taille_height", _grille_height);
    shader_linear_solve->lierFloat("taille_depth",  _grille_depth);
    shader_linear_solve->lierVecteur("a", a);
    shader_linear_solve->lierVecteur("c", c);


    for ( int i = 0; i < PRECISION_RESOLUTION; i++){
        
        _grille_vitesse_courante->bindTexture(GL_TEXTURE0);
        _grille_vitesse_dest->bindTexture(GL_TEXTURE1);
        
        shader_linear_solve->lierLevel("texture_entree", 0);
        shader_linear_solve->lierLevel("texture_sortie", 1);
            	
    	glActiveTexture(GL_TEXTURE0);
        
        buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_vitesse_dest);
        
        swapGrilles(&_grille_vitesse_dest, &_grille_vitesse_courante);
    
    }    
    
    shader_linear_solve->Unbind_Program();          
    
}
void Solver_GPU::diffuse_speed ( float dt ){
    linearSolve_speed(0, dt);
}




void Solver_GPU::advect_speed ( float dt )
{

    
    Vecteur3D dt0 = Vecteur3D(_grille_width, _grille_height, _grille_depth);
    dt0 = dt0 * dt;

    shader_advect_speed->Bind_Program();          

    shader_advect_speed -> lierFloat("dt", dt);
    shader_advect_speed-> lierVecteur("dt0", dt0);
    shader_advect_speed -> lierFloat("taille_width",  _grille_width);
    shader_advect_speed -> lierFloat("taille_height", _grille_height);
    shader_advect_speed -> lierFloat("taille_depth",  _grille_depth);
    
    _grille_vitesse_courante->bindTexture(GL_TEXTURE0);
    _grille_vitesse_courante->bindTexture(GL_TEXTURE1);
        
    shader_advect_speed->lierLevel("densite_entree", 0);
    shader_advect_speed->lierLevel("vitesse_entree", 1);
            	
    glActiveTexture(GL_TEXTURE0);
        
    buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_vitesse_dest);
        
    swapGrilles(&_grille_vitesse_dest, &_grille_vitesse_courante);
        
    shader_advect_speed->Unbind_Program();          
    
}



void Solver_GPU::advect_cool ( float dt )
{
    Vecteur3D dt0 = Vecteur3D(_grille_width, _grille_height, _grille_depth);
    dt0 = dt0 * dt;
 
    
    float consume = SolverParam::getConsumingParam();
    float firetosmoke = SolverParam::getFireToSmokeParam();
    float c0 = 1.0f - SolverParam::getCoolingParam() * dt;

    shader_advect_cool->Bind_Program();          

    shader_advect_cool -> lierFloat("dt", dt);
    shader_advect_cool-> lierVecteur("dt0", dt0);
    shader_advect_cool -> lierFloat("taille_width",  _grille_width);
    shader_advect_cool -> lierFloat("taille_height", _grille_height);
    shader_advect_cool -> lierFloat("taille_depth",  _grille_depth);
    shader_advect_cool -> lierFloat("consume", consume);
    shader_advect_cool -> lierFloat("c0", c0);
    shader_advect_cool -> lierFloat("fireToSmoke", firetosmoke);
    
    _grille_feu_courante->bindTexture(GL_TEXTURE0);
    _grille_vitesse_courante->bindTexture(GL_TEXTURE1);
        
    shader_advect_cool->lierLevel("densite_entree", 0);
    shader_advect_cool->lierLevel("vitesse_entree", 1);
            	
    glActiveTexture(GL_TEXTURE0);
        
    buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_feu_dest);
        
    swapGrilles(&_grille_feu_dest, &_grille_feu_courante);
        
    shader_advect_cool->Unbind_Program();          

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
    addSource( dt, *_grille_feu_courante, *_grille_feu_sources );
    // diffuse le feu, la fumee, la temperature    
	diffuse ( dt );
	// advect
	advect_cool( dt );
    
}



void Solver_GPU::velocitiesStepWithTemp ( float dt )
{
    // Sources
    addSource( dt, *_grille_vitesse_courante, *_grille_vitesse_sources );
    // Buoyancy
    //
    // Diffuse
	diffuse_speed ( dt);
	// Project
	//
	// Advect
	advect_speed ( dt);
	// Project
	//

}


 
 
 
 
 
 
 
 
void addSourceCorrection ( int N, float *x , float *f, float *T, float *s , float sub, float fireToSmoke, float dt ){
}

void addBuoyancy( int N, float *T, float *v, float buoy, float dt){
}

void setBoundaries ( int N, int b, float *x ) {
}

void setBoundariesB0 ( int N, float *x ) {
}
