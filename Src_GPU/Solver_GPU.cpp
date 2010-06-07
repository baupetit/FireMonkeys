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


#define PRECISION_RESOLUTION 30


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
	    
	shader_buoy = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/buoy.frag");
	    
	    
	shader_div = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/div.frag");

	shader_linear_solve_pression = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/linear_solve_pression.frag");
    
	                                 
         	                                 
	shader_velocity_correction = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/shader_velocity_correction.frag");
	    
	    
	// Frame buffer
	buffer = new Framebuffer(width, height, depth);
    
	// Grilles ( textures )
	_grille_feu_sources = new Texture3D();
	_grille_feu_courante = new Texture3D();
	_grille_feu_dest = new Texture3D();
	_grille_vitesse_sources = new Texture3D();
	_grille_vitesse_courante = new Texture3D();
	_grille_vitesse_dest = new Texture3D();
	_grille_pression_courante = new Texture3D();
	_grille_pression_dest = new Texture3D();
    
    
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
	
	
	// init de la texture initiale
	_grille_pression_courante->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);
	// init de la texture dest
	_grille_pression_dest->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);
	


	// init de la source
	ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){    
				if ( i > 3*_grille_depth/7 && i < 4*_grille_depth/7 &&
				     k > 3*_grille_width /7  &&  k< 4*_grille_height/7 && 
				     j < _grille_height/10 )
				{
				    *ptr = 0.01;//0.001;//rand()/(float)RAND_MAX;
				    ptr++;
				    *ptr = 0.0000;//rand()/(float)RAND_MAX;
				    ptr++;
				    *ptr = 0.01;//rand()/(float)RAND_MAX;
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
				if ( j < _grille_height/5 )
				{
				    *ptr = 0.00 * ( rand()/(float)RAND_MAX - 0.5 );
				    ptr++;
				    *ptr = 0.00 * rand()/(float)RAND_MAX;
				    ptr++;
				    *ptr = 0.00 * ( rand()/(float)RAND_MAX - 0.5 );
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
				*ptr = 0.00;
				ptr++;
				*ptr = 0.00;
				ptr++;
				*ptr = 0.00;
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

const GLuint Solver_GPU::getPression() const {
	return _grille_pression_courante->get_texture_id();
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


void Solver_GPU::linearSolve ( int b, float dt,
                               Vecteur3D& a,
                               Texture3D* courante, Texture3D* dest){

    Vecteur3D c = Vecteur3D(1.0, 1.0, 1.0) + 6 * a;
    
    shader_linear_solve->Bind_Program();          

    shader_linear_solve->lierFloat("taille_width",  _grille_width);
    shader_linear_solve->lierFloat("taille_height", _grille_height);
    shader_linear_solve->lierFloat("taille_depth",  _grille_depth);
    shader_linear_solve->lierVecteur("a", a);
    shader_linear_solve->lierVecteur("c", c);


    for ( int i = 0; i < PRECISION_RESOLUTION; i++){
    
        
        courante->bindTexture(GL_TEXTURE0);
        dest->bindTexture(GL_TEXTURE1);
        

        shader_linear_solve->lierLevel("texture_entree", 0);
        shader_linear_solve->lierLevel("texture_sortie", 1);
            	
    	glActiveTexture(GL_TEXTURE0);
        
        buffer->traiterDessinDansBuffer1ALAFOIS(*dest);
        
        swapGrilles(&dest, &courante);
    
    }    
    
    shader_linear_solve->Unbind_Program();          
    
}

void Solver_GPU::diffuse ( float dt ){


    float arf = dt*_grille_width*_grille_width;

	float a1= arf * SolverParam::getDiffusionParamFire();
	float a2= arf * SolverParam::getDiffusionParamSmoke();
	float a3= arf * SolverParam::getDiffusionParamTemperature();

    Vecteur3D a = Vecteur3D(a1,a2,a3);
    
    linearSolve(0, dt, a, _grille_feu_courante, _grille_feu_dest);
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










void Solver_GPU::linearSolvePression ( int b, float dt ){

    float a = 1.0;
    float c = 6.0;
    
    shader_linear_solve_pression->Bind_Program();          

    shader_linear_solve_pression->lierFloat("taille_width",  _grille_width);
    shader_linear_solve_pression->lierFloat("taille_height", _grille_height);
    shader_linear_solve_pression->lierFloat("taille_depth",  _grille_depth);
    shader_linear_solve_pression->lierFloat("a", a);
    shader_linear_solve_pression->lierFloat("c", c);
 

    for ( int i = 0; i < PRECISION_RESOLUTION; i++){
        
        _grille_pression_courante->bindTexture(GL_TEXTURE0);
        _grille_pression_dest->bindTexture(GL_TEXTURE1);
        
        shader_linear_solve_pression->lierLevel("texture_entree", 0);
        shader_linear_solve_pression->lierLevel("texture_sortie", 1);
            	
    	glActiveTexture(GL_TEXTURE0);
        
        buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_pression_dest);
        
        swapGrilles(&_grille_pression_dest, &_grille_pression_courante);
    
    }    
    
    shader_linear_solve_pression->Unbind_Program();          
    
}




void Solver_GPU::velocityCorrection(){

    shader_velocity_correction->Bind_Program();          

        
    shader_velocity_correction->lierFloat("taille_width",  _grille_width);
    shader_velocity_correction->lierFloat("taille_height", _grille_height);
    shader_velocity_correction->lierFloat("taille_depth",  _grille_depth);
    
    _grille_vitesse_courante->bindTexture(GL_TEXTURE0);
    _grille_pression_courante->bindTexture(GL_TEXTURE1);
        
    shader_velocity_correction->lierLevel("texture_vitesse", 0);
    shader_velocity_correction->lierLevel("texture_pression", 1);
            	
    glActiveTexture(GL_TEXTURE0);
        
    buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_vitesse_courante);
        
    //swapGrilles(&_grille_feu_dest, &_grille_feu_courante);
        
    shader_velocity_correction->Unbind_Program();          
	


}



void Solver_GPU::diffuse_speed ( float dt ){


    float arf = dt*_grille_width*_grille_width;

	float a1= arf * SolverParam::getViscosityParam();
	float a2= arf * SolverParam::getViscosityParam();
	float a3= arf * SolverParam::getViscosityParam();

    Vecteur3D a = Vecteur3D(a1,a2,a3);
    
    linearSolve(0, dt, a, _grille_vitesse_courante, _grille_vitesse_dest);
    
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
        
    shader_advect_speed->lierLevel("vitesse_entree", 0);
            	
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


void Solver_GPU::addBuoyancy(float dt){
    
    float buoy = SolverParam::getBuoyancyParam();

    cout << " dt : " << dt << " buoy " << buoy << endl;
    shader_buoy->Bind_Program();          

    shader_buoy -> lierFloat("dt", dt);
    shader_buoy -> lierFloat("buoy", buoy);
    
    _grille_vitesse_courante->bindTexture(GL_TEXTURE0);
    _grille_feu_courante->bindTexture(GL_TEXTURE1);
        
    shader_buoy->lierLevel("texture_vitesse", 0);
    shader_buoy->lierLevel("texture_densite", 1);
            	
    glActiveTexture(GL_TEXTURE0);
        
    buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_vitesse_dest);
        
    swapGrilles(&_grille_vitesse_dest, &_grille_vitesse_courante);
        
    shader_buoy->Unbind_Program();          
}


void Solver_GPU::project ()
{
    // PASS 1
    // calcul de la divergence
    
    shader_div->Bind_Program();          

    shader_div->lierFloat("taille_width",  _grille_width);
    shader_div->lierFloat("taille_height", _grille_height);
    shader_div->lierFloat("taille_depth",  _grille_depth);
    shader_div->lierFloat("echantillonage",  _grille_width);

    _grille_vitesse_courante->bindTexture(GL_TEXTURE0);
    _grille_vitesse_courante->bindTexture(GL_TEXTURE1);
    shader_div->lierLevel("texture_pression", 0);
    shader_div->lierLevel("texture_vitesse", 1);
    
    glActiveTexture(GL_TEXTURE0);
        
    buffer->traiterDessinDansBuffer1ALAFOIS(*_grille_pression_courante);
    
    shader_div->Unbind_Program();
    
    
    // raz de la pression
    // PASS 2
    // calcul de la pression ( linear solve )
    
    linearSolvePression(0, 0);
    
    
    // PASS 3
    // correction de la vitesse
    
    //velocityCorrection();
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
    addSource ( dt, *_grille_vitesse_courante, *_grille_vitesse_sources );
    // Buoyancy
    addBuoyancy ( dt );
    // Vorticity confinement
    //
    // Diffuse
	diffuse_speed ( dt );
	// Project
	project();
	// Advect
	advect_speed ( dt );
	// Project
	project();

}


 
 
 
 
 
 
 
 
void addSourceCorrection ( int N, float *x , float *f, float *T, float *s , float sub, float fireToSmoke, float dt ){
}


void setBoundaries ( int N, int b, float *x ) {
}

void setBoundariesB0 ( int N, float *x ) {
}
