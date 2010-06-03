#define SIZE ((N+2)*(N+2)*(N+2))
#ifndef NB_ITERATION_SOLVE
#define NB_ITERATION_SOLVE 20
#endif

#include "Solver_GPU.h"

#include "SolverParam.h"

#include <cmath>

#include <cstdlib>
#include <ctime>


Solver_GPU::Solver_GPU( int width, int height, int depth ) 
	: _grille_width(width), _grille_height(height), _grille_depth(depth) {
	
	cout << "Nouveau Solver_GPU" << endl;

	// Shader d'advection du feu
	shader_advect_feu = new Shader("./Shaders/feu.vert","./Shaders/feu.frag");
    
	// PINGPONG
	pingpong = 1;
    
	// Frame buffer
	buffer = new Framebuffer(width, height, depth);
    
	// Grilles
	_grille_feu_1 = new Texture3D();
	_grille_feu_2 = new Texture3D();
    
	srand ( time(NULL) );

    
	float coeff1 = rand()/(float)RAND_MAX;
	float coeff2 = rand()/(float)RAND_MAX;
    
    
	// Creation du champs de vitesse vide
	float *texture = new float[_grille_width*_grille_height*_grille_depth*4];
	float *ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){    
				*ptr = i/(float)_grille_depth * coeff1;
				ptr++;
				*ptr = j/(float)_grille_height * coeff2;
				ptr++;
				*ptr = k/(float)_grille_width;
				ptr++;
				*ptr = 1.0f;
				ptr++;
			}
		}
	} 
        
	// INITIALISATION GRILLES
	_grille_feu_1->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	_grille_feu_2->charger_matrice(NULL,    _grille_width, _grille_height, _grille_depth);    
    
}

Solver_GPU::~Solver_GPU(){
	delete buffer;
	delete shader_advect_feu;    
	delete _grille_feu_1;
	delete _grille_feu_2;
}

const GLuint Solver_GPU::getDensities() const {
	if (pingpong == PING)
		return _grille_feu_1->get_texture_id();
	else
		return _grille_feu_2->get_texture_id();
}

const GLuint Solver_GPU::getDestDensities() const {
	if (pingpong == PONG)
		return _grille_feu_1->get_texture_id();
	else
		return _grille_feu_2->get_texture_id();
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
                           float diff, 
                           float dt ){
    
    
    
	calcul_shader.lierTexture("feu", getDestDensities());
    
	// on lie le shader            
	calcul_shader.Bind_Program();
    
    
    
	// traiter le calcul
	buffer->traiterDessinDansBuffer(getDensities(), getDestDensities());
    
	// on delie le shader            
	calcul_shader.Unbind_Program();
    
    
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

void Solver_GPU::densitiesStep ( float dt )
{	
    
    
	diffuse (  *shader_advect_feu, 
		   string("feu"), 
		   SolverParam::getDiffusionParamFire(),// diffusion
		   dt );
    
    
	swapGrillesCourantes(); 
    
    
}

void Solver_GPU::densitiesStepWithTemp ( float dt )
{	
}

void Solver_GPU::velocitiesStep ( float dt )
{
}

void Solver_GPU::velocitiesStepWithTemp ( float dt )
{
}
    
void Solver_GPU::swapGrillesCourantes(){
	if (pingpong == PING)
		pingpong = PONG;
	else
		pingpong = PING;
}
 
