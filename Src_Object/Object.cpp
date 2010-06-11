#include "Object.h"
#define NB_ITERATION_SOLVE 10
 


#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

Object::Object(){
	defVox = Voxel();
	grille = 0;
}

Object::Object( Voxel defVox )
	:defVox(defVox)
{
	grille = 0;
}

/* destructor */
Object::~Object(){
	if( grille ) delete [] grille;
}


/* herited methods */
void Object::Afficher_Face_Camera(Vecteur3D& positionCamera,
				  Vecteur3D& directionCamera,
				  float dt ) {
	Afficher( dt );
}

void Object::Afficher( float dt ){
	int i,j,k,l ;

    
    
	glColor4f( color.x, color.y, color.z, color.w );
	glBegin( GL_TRIANGLES );
	for( k=0; k<grilleSize.z; ++k){
		for( j=0; j<grilleSize.y; ++j){
			for( i=0; i<grilleSize.x; ++i){
				Voxel v = grille[_Grille_Ind(i,j,k)];
				for( l = 0 ; l < v.nb_faces; ++l ){
					v.faces[l].drawTriangle();
				}
			}
		}
	}
	glEnd();

/*	
	for( int k = 0 ; k < grilleSize.z ; ++k ){
		for( int j = 0 ; j < grilleSize.y ; ++j ){
			for( int i = 0 ; i < grilleSize.x ; ++i ){
				Vecteur3D p = cellToPoint( Vecteur3I(i , j , k));
				Voxel val = grille[_Grille_Ind(i,j,k)];

				if( val.frontiere ){
					glBegin(GL_LINES);	
					glColor4f(0.0,0.0,0.0,1.0);
					p-=AABB.lowerCorner;
					glVertex3d(p.x,p.y,p.z);
					glVertex3d(p.x+0.5*val.repulsion.x,p.y+0.5*val.repulsion.y,p.z+0.5*val.repulsion.z);
					glEnd();
					glColor3f( val.temperature,0,0 );
					glVertex3f( p.x, p.y, p.z );
				}
			}
		}
	}
*/

	glPointSize( 4.0f );
	glDisable(GL_LIGHTING);
	
	glBegin(GL_POINTS);
	for( int k = 0 ; k < grilleSize.z ; ++k ){
		for( int j = 0 ; j < grilleSize.y ; ++j ){
			for( int i = 0 ; i < grilleSize.x ; ++i ){
				Vecteur3D p = cellToPoint( Vecteur3I(i , j , k));
				Voxel val = grille[_Grille_Ind(i,j,k)];
				
				if( val.plein ){
					//cout << val.temperature << endl;
					glColor3f( val.temperature,0,0 );
				    glVertex3f( p.x, p.y, p.z );
				}
				if( val.frontiere ){
					glColor3f( val.temperature,0,0 );
					glVertex3f( p.x, p.y, p.z );
				}

			}
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);

}

void Object::voxelConsome( Voxel *v ){
	// quand un voxel est comsomme
	//  - deplacement de la valeur de ses huits sommets ( par spaceDiv )
	//  - transmission de l'information au 26 voisins
	//  - re-calcul de la triangulation localement (27 cases)
	//  - recherche de la frontiere localement
                        cout << "voxelConsome" << endl;
}

/* heat diffusion */
void Object::diffuserTemperature( float dt )
{
	int i, j, k, l ;
	float div = SolverParam::getSpaceDiv();
	//coeff de diffusion thermique au sein du materiau
	//float a = dt*diff_materiau/(div*div);
	//float c = 1+6*a;
	//coeff de diffusion thermique au bord du matériau ( phi=hS(T1-T2) )	
	// h = conductance_thermique
	//float a_front = div*div * conductance_thermique;
	//taille de la grille
	int N1=grilleSize.x;
	int N2=grilleSize.y;
	int N3=grilleSize.z;
	float b = (div*div);
	float a = (dt/b);
	float c;
	float tmp;
	for( k=1; k<N1-1; ++k ){ 
		for( j = 1; j<N2-1 ; ++j) { 
			for( i=1 ; i<N3-1 ; ++i ){
				if (grille[_Grille_Ind(i,j,k)].plein){
					//cout << grille[_Grille_Ind(i,j,k)].temperature << endl;
				tmp = grille[_Grille_Ind(i,j,k)].temperature;		
				grille[_Grille_Ind(i,j,k)].temperature  = grille[_Grille_Ind(i,j,k)].temperature0;
				grille[_Grille_Ind(i,j,k)].temperature0 = tmp;
				}
				//cout << tmp << endl;
			}
		}
	}


	for ( l=0 ; l<NB_ITERATION_SOLVE ; l++ ) {
		for( k=1; k<N1-1; ++k ){ 
			for( j = 1; j<N2-1 ; ++j) { 
				for( i=1 ; i<N3-1 ; ++i ){
					 if (grille[_Grille_Ind(i  ,j  ,k  )].plein && (
						grille[_Grille_Ind(i+1,j  ,k  )].frontiere ||
						grille[_Grille_Ind(i-1,j  ,k  )].frontiere ||
						grille[_Grille_Ind(i  ,j+1,k  )].frontiere ||
						grille[_Grille_Ind(i  ,j-1,k  )].frontiere ||
						grille[_Grille_Ind(i  ,j  ,k+1)].frontiere ||
						grille[_Grille_Ind(i  ,j  ,k-1)].frontiere )){
						 //on a au moins 1 voxel frontiere proche
						 //il faut compter le nombre de voxel voisins qui sont plein: pour le c (1+nb_plein*a);
						grille[_Grille_Ind(i,j,k)].temperature = grille[_Grille_Ind(i,j,k)].temperature0;
						c = 1; 
						//on teste pour chaque voisin:					
						if (grille[_Grille_Ind(i+1,j  ,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
								(grille[_Grille_Ind(i+1,j,k)].diffusion)*grille[_Grille_Ind(i+1,j  ,k  )].temperature;
							c += a*(grille[_Grille_Ind(i+1,j,k)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k)].conductance)*grille[_Grille_Ind(i+1,j  ,k  )].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						}
						if (grille[_Grille_Ind(i-1,j  ,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
								(grille[_Grille_Ind(i-1,j,k)].diffusion)*grille[_Grille_Ind(i-1,j  ,k  )].temperature;
							c += a*(grille[_Grille_Ind(i-1,j,k)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k)].conductance)*grille[_Grille_Ind(i-1,j  ,k  )].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						}
						if (grille[_Grille_Ind(i,j+1,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
								(grille[_Grille_Ind(i,j+1,k)].diffusion)*grille[_Grille_Ind(i,j+1,k  )].temperature;
							c += a*(grille[_Grille_Ind(i,j+1,k)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k)].conductance)*grille[_Grille_Ind(i,j+1,k  )].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						}
						if (grille[_Grille_Ind(i,j-1,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
								(grille[_Grille_Ind(i,j-1,k)].diffusion)*grille[_Grille_Ind(i,j-1  ,k  )].temperature;
							c += a*(grille[_Grille_Ind(i,j-1,k)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k)].conductance)*grille[_Grille_Ind(i,j-1  ,k  )].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						}
						if (grille[_Grille_Ind(i,j  ,k+1)].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
								(grille[_Grille_Ind(i,j,k+1)].diffusion)*grille[_Grille_Ind(i,j  ,k+1)].temperature;
							c += a*(grille[_Grille_Ind(i,j,k+1)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k)].conductance)*grille[_Grille_Ind(i,j  ,k+1)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						}
						if (grille[_Grille_Ind(i,j  ,k-1)].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
								(grille[_Grille_Ind(i,j,k-1)].diffusion)*grille[_Grille_Ind(i,j  ,k-1)].temperature;
							c += a*(grille[_Grille_Ind(i,j,k-1)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k)].conductance)*grille[_Grille_Ind(i,j  ,k-1)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						}
	
						grille[_Grille_Ind(i,j,k)].temperature /= c;
						
					//sinon on se concentre sur la frontiere.
					}					
					else if (grille[_Grille_Ind(i  ,j  ,k  )].plein && 
					    grille[_Grille_Ind(i+1,j  ,k  )].plein &&
						grille[_Grille_Ind(i-1,j  ,k  )].plein &&
						grille[_Grille_Ind(i  ,j+1,k  )].plein &&
						grille[_Grille_Ind(i  ,j-1,k  )].plein &&
						grille[_Grille_Ind(i  ,j  ,k+1)].plein &&
						grille[_Grille_Ind(i  ,j  ,k-1)].plein ){
						//le cas ou on est dans la matiere avec que des voisins matiere
						//conduction thermique classique avc un coefficient de diffusion 
						//diff propre au matériau
						c = 1 +  a*(grille[_Grille_Ind(i+1,j,k)].diffusion +
												 grille[_Grille_Ind(i-1,j,k)].diffusion +
												 grille[_Grille_Ind(i,j+1,k)].diffusion +
												 grille[_Grille_Ind(i,j-1,k)].diffusion +
												 grille[_Grille_Ind(i,j,k+1)].diffusion +
												 grille[_Grille_Ind(i,j,k-1)].diffusion );
						grille[_Grille_Ind(i,j,k)].temperature = ( grille[_Grille_Ind(i,j,k)].temperature0 + a*(
									  	grille[_Grille_Ind(i+1,j,k)].diffusion* grille[_Grille_Ind(i+1,j,k)].temperature +
									  	grille[_Grille_Ind(i-1,j,k)].diffusion* grille[_Grille_Ind(i-1,j,k)].temperature +
									 	grille[_Grille_Ind(i,j+1,k)].diffusion* grille[_Grille_Ind(i,j+1,k)].temperature +
										grille[_Grille_Ind(i,j-1,k)].diffusion* grille[_Grille_Ind(i,j-1,k)].temperature +
										grille[_Grille_Ind(i,j,k+1)].diffusion* grille[_Grille_Ind(i,j,k+1)].temperature +
										grille[_Grille_Ind(i,j,k-1)].diffusion* grille[_Grille_Ind(i,j,k-1)].temperature ) )/c;
					//cout << "a= " << a << endl;
					//cout << grille[_Grille_Ind(i,j,k)].temperature << endl;	
					} else if (grille[_Grille_Ind(i  ,j  ,k  )].frontiere) {
						//on est a la frontiere on la met a jour avec le transfert thermique 
						//qui sort de l'objet, on ne considere donc que ses voisins plein.
						grille[_Grille_Ind(i,j,k)].temperature = grille[_Grille_Ind(i,j,k)].temperature0;
						
						c = 1;
						if (grille[_Grille_Ind(i+1,j  ,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i+1,j,k)].conductance)*grille[_Grille_Ind(i+1,j,k)].temperature;
							c += b*(grille[_Grille_Ind(i+1,j,k)].conductance);
						} 

						if (grille[_Grille_Ind(i-1,j  ,k  )].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i-1,j,k)].conductance)*grille[_Grille_Ind(i-1,j,k)].temperature;
							c += b*(grille[_Grille_Ind(i-1,j,k)].conductance);
						} 

						if (grille[_Grille_Ind(i  ,j+1,k  )].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j+1,k)].conductance)*grille[_Grille_Ind(i,j+1,k)].temperature;
							c += b*(grille[_Grille_Ind(i,j+1,k)].conductance);
						} 

						if (grille[_Grille_Ind(i  ,j-1,k  )].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j-1,k)].conductance)*grille[_Grille_Ind(i,j-1,k)].temperature;
							c += b*(grille[_Grille_Ind(i,j-1,k)].conductance);
						} 

						if (grille[_Grille_Ind(i  ,j  ,k+1)].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k+1)].conductance)*grille[_Grille_Ind(i,j,k+1)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k+1)].conductance);
						} 

						if (grille[_Grille_Ind(i  ,j  ,k-1)].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k-1)].conductance)*grille[_Grille_Ind(i,j,k-1)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k-1)].conductance);
						} 

						grille[_Grille_Ind(i,j,k)].temperature /= c;
						

					} 
				}
			}
		}
		//setBoundaries ( N, b, x );
	}

/*	

	for( k=1; k<N1-1; ++k ){ 
		for( j = 1; j<N2-1 ; ++j) { 
			for( i=1 ; i<N3-1 ; ++i ){
				if (grille[_Grille_Ind(i,j,k)].plein)
					//cout << grille[_Grille_Ind(i,j,k)].temperature << endl;
				tmp = grille[_Grille_Ind(i,j,k)].temperature;		
				grille[_Grille_Ind(i,j,k)].temperature  = grille[_Grille_Ind(i,j,k)].temperature0;
				grille[_Grille_Ind(i,j,k)].temperature0 = tmp;
								
				//cout << tmp << endl;
			}
		}
	}
*/
}
