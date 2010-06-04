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

	// init des params
	SolverParam::initSolverParam();

	// Shader resolution lineaire
	shader_linear_solve = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/linear_solve.frag");
    
	// Frame buffer
	buffer = new Framebuffer(width, height, depth);
    
	// Grilles ( textures )
	_grille_temp = new Texture3D();
	_grille_feu_courante = new Texture3D();
	_grille_feu_dest = new Texture3D();
    
    
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
	
	
	_grille_feu_courante->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);
	
	// init de la texture dest
	
	
	_grille_feu_dest->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);
        
	
	_grille_temp->charger_matrice(texture, _grille_width, _grille_height, _grille_depth);    
	
	// liberation
	delete(texture);
	
	
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
    
    
            
    shader_linear_solve->Bind_Program();          

    shader_linear_solve->lierFloat("taille_width",  _grille_width);
    shader_linear_solve->lierFloat("taille_height", _grille_height);
    shader_linear_solve->lierFloat("taille_depth",  _grille_depth);

    //shader_linear_solve->lierTexture("texture_entree", _grille_feu_courante->get_texture_id(),0);

    
    Vecteur3D a = Vecteur3D(a1,a2,a3);
    a = Vecteur3D( 0.04, 0.039, 0.69);

    //a = a * 100000000;    
    
    Vecteur3D c = Vecteur3D(1.0, 1.0, 1.0) + 6 * a;
    
    
    
    
    
    shader_linear_solve->lierVecteur("a", a);
    shader_linear_solve->lierVecteur("c", c);


    for ( int i = 0; i < 20; i++){
        
        cout << " ================ "   << endl;
        
        cout << " Boucle i : " << i  << endl;
        cout << " Source : " << (int) _grille_feu_courante->get_texture_id() << endl;
        cout << " Cible : " << (int) _grille_feu_dest->get_texture_id()  << endl;
        cout << " Temp : " << (int) _grille_temp->get_texture_id()  << endl;
        
        
        GLuint id0 = _grille_feu_courante->get_texture_id();
        GLuint id1 = _grille_feu_dest->get_texture_id();
        GLuint id2 = _grille_temp->get_texture_id();
        
        glEnable(GL_TEXTURE_3D);
        glActiveTexture(GL_TEXTURE0);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_3D,id0);   
        
        glActiveTexture(GL_TEXTURE1);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_3D,id1);   
        
        
        glActiveTexture(GL_TEXTURE2);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_3D,id2);   
        
        
        GLuint	location;
        location = glGetUniformLocation ( shader_linear_solve->getProgramId(), "texture_entree");
        glUniform1i(location, 0);
        
        location = glGetUniformLocation ( shader_linear_solve->getProgramId(), "texture_sortie");
        glUniform1i(location, 1);
        
    	
    	
    	glActiveTexture(GL_TEXTURE0);
        
        buffer->traiterDessinDansBuffer(id1);
        
        swapGrilles(&_grille_feu_dest, &_grille_feu_courante);
    
    }    
    
    
    shader_linear_solve->Unbind_Program();          
    
}

void Solver_GPU::diffuse ( float diff, 
                           float dt ){
    
    
    
    float beattleJuce = dt*diff*(1/SolverParam::getEchantillonage()*1/SolverParam::getEchantillonage());
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
 
