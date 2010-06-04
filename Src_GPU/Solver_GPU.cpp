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


Solver_GPU::Solver_GPU( int width, int height, int depth ) 
	: _grille_width(width), _grille_height(height), _grille_depth(depth) {

	
	cout << "Nouveau Solver_GPU" << endl;
	
	// init
	SolverParam::initSolverParam();

	// Shader lineaire
	shader_linear_solve = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/linear_solve.frag");

    
	// Frame buffer
	buffer = new Framebuffer(width, height, depth);
    
	// Grilles
	_grille_temp = new Texture3D();
	_grille_feu_courante = new Texture3D();
	_grille_feu_dest = new Texture3D();
    
	srand ( time(NULL) );

    
	float coeff1 = rand()/(float)RAND_MAX;
	float coeff2 = rand()/(float)RAND_MAX;
    
    
    /*
	// Creation du champs de vitesse vide
	float *texture = new float[_grille_width*_grille_height*_grille_depth*4];
	float *ptr = texture;
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
	
	
	
	*/
	float *texture = new float[_grille_width*_grille_height*_grille_depth*4];
	float *ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth; i++){
				if(i<4){    
				*ptr = 1.0f;//i/(float)_grille_depth * coeff1;
				} else {*ptr = 0.0f;}
				ptr++;
				*ptr = 0.0;//j/(float)_grille_height * coeff2;
				ptr++;
				*ptr = 0.0;//k/(float)_grille_width;
				ptr++;
				*ptr = 1.0f;
				ptr++;
			}
		}
	} 
	
	/*
	
        
        /*
        
	float *texture = new float[_grille_width*_grille_height*_grille_depth*4];
	float *ptr = texture;
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
        
	ptr = texture;
	for (int k = 0; k < _grille_width; k++){
		for (int j = 0; j < _grille_height; j++){
			for (int i = 0; i < _grille_depth / 30; i++){    
				*ptr = 0.1;
				ptr++;
				*ptr = 0.4;
				ptr++;
				*ptr = 0.4;
				ptr++;
				*ptr = 1.0f;
				ptr++;
			}
		}
	} 
	
	
	
	*/
	
	
	
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
	_grille_feu_courante->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	_grille_feu_dest->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	_grille_temp->charger_matrice(NULL, _grille_width, _grille_height, _grille_depth);    
	
	
}

Solver_GPU::~Solver_GPU(){
	delete buffer;
	delete shader_linear_solve;    
	delete _grille_feu_courante;
	delete _grille_feu_dest;
	delete _grille_temp;
}

const GLuint Solver_GPU::getDensities() const {
	return _grille_feu_courante->get_texture_id();
}

const GLuint Solver_GPU::getDestDensities() const {
	return _grille_feu_dest->get_texture_id();
}

const GLuint Solver_GPU::getTemp() const {
	return _grille_temp->get_texture_id();
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

void Solver_GPU::linearSolve ( int b, float a1, float a2, float a3 ){
    float c1 = 1 + 6 * a1;
    float c2 = 1 + 6 * a2;
    float c3 = 1 + 6 * a3;
    
    
            
    shader_linear_solve->Bind_Program();          

    shader_linear_solve->lierFloat("taille_width",  _grille_width);
    shader_linear_solve->lierFloat("taille_height", _grille_height);
    shader_linear_solve->lierFloat("taille_depth",  _grille_depth);

    shader_linear_solve->lierTexture("texture_entree", _grille_feu_courante->get_texture_id(),0);

    
    Vecteur3D a = Vecteur3D(a1,a2,a3);
    Vecteur3D c = Vecteur3D(c1,c2,c3);
    
    
    shader_linear_solve->lierVecteur("a", a);
    shader_linear_solve->lierVecteur("c", c);
      
      
    for ( int i = 0; i < 20; i++){
        /*    
        cout << " Boucle i : " << i ;
        cout << " Source : " << (int) _grille_feu_courante->get_texture_id() ;
        cout << " Cible : " << (int) _grille_feu_dest->get_texture_id()  << endl;
        */
        

        /*
        shader_linear_solve->lierTexture("texture_sortie", _grille_feu_dest->get_texture_id(),1);
        buffer->traiterDessinDansBuffer(_grille_feu_dest->get_texture_id());
        */

        
        shader_linear_solve->lierTexture("texture_sortie", _grille_feu_dest->get_texture_id(),1);
        
    	buffer->traiterDessinDansBuffer(_grille_temp->get_texture_id());
    	
        swapGrilles(&_grille_feu_dest, &_grille_temp);
    
    }    
    
    
    shader_linear_solve->Unbind_Program();          
    
}

void Solver_GPU::diffuse ( float diff, 
                           float dt ){
    
    
    float beattleJuce = dt*diff*(SolverParam::getEchantillonage()*SolverParam::getEchantillonage());
    linearSolve(0, beattleJuce, beattleJuce, beattleJuce);
    
    
    swapGrilles(&_grille_feu_dest, &_grille_feu_courante);
    
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
    
    
	diffuse ( SolverParam::getDiffusionParamFire(),
		      dt );
    

    swapGrilles(&_grille_feu_dest, &_grille_feu_courante);

    
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
    
void Solver_GPU::swapGrilles(Texture3D** t1, Texture3D** t2){
    Texture3D* tmp;
    tmp = *t1;
    *t1 = *t2;
    *t2 = tmp;
}
 
