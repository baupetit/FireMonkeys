#include "Object.h"
#define NB_ITERATION_SOLVE 10
 


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

/* heat diffusion */
void Object::diffuserTemperature( float dt ){
}

/* herited methods */
void Object::Afficher_Face_Camera(Vecteur3D& positionCamera,
				  Vecteur3D& directionCamera,
				  float dt ) {
	Afficher( dt );
}


void Object::diffuserTemperature( float dt )
{
	int i, j, k, l , c;
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
	

	for ( l=0 ; l<NB_ITERATION_SOLVE ; l++ ) {
		for( k=1; k<N1+1; ++k ){ 
			for( j = 1; j<N2+1 ; ++j) { 
				for( i=1 ; i<N3+1 ; ++i ){
					if (grille[_Grille_Ind(i  ,j  ,k  )].plein && 
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
					} 
					else if (grille[_Grille_Ind(i  ,j  ,k  )].plein){
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
									(grille[_Grille_Ind(i+1,j,k)].conductance)*grille[_Grille_Ind(i+1,j  ,k  )].temperature;
							c += b*(grille[_Grille_Ind(i+1,j,k)].conductance);
						}
						if (grille[_Grille_Ind(i-1,j  ,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
									(grille[_Grille_Ind(i-1,j,k)].diffusion)*grille[_Grille_Ind(i-1,j  ,k  )].temperature;
							c += a*(grille[_Grille_Ind(i-1,j,k)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i-1,j,k)].conductance)*grille[_Grille_Ind(i-1,j  ,k  )].temperature;
							c += b*(grille[_Grille_Ind(i-1,j,k)].conductance);
						}
						if (grille[_Grille_Ind(i,j+1,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
									(grille[_Grille_Ind(i,j+1,k)].diffusion)*grille[_Grille_Ind(i,j+1,k  )].temperature;
							c += a*(grille[_Grille_Ind(i,j+1,k)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j+1,k)].conductance)*grille[_Grille_Ind(i,j+1,k  )].temperature;
							c += b*(grille[_Grille_Ind(i,j+1,k)].conductance);
						}
						if (grille[_Grille_Ind(i,j-1,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
									(grille[_Grille_Ind(i,j-1,k)].diffusion)*grille[_Grille_Ind(i,j-1  ,k  )].temperature;
							c += a*(grille[_Grille_Ind(i,j-1,k)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j-1,k)].conductance)*grille[_Grille_Ind(i,j-1  ,k  )].temperature;
							c += b*(grille[_Grille_Ind(i,j-1,k)].conductance);
						}
						if (grille[_Grille_Ind(i,j  ,k+1)].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
									(grille[_Grille_Ind(i,j,k+1)].diffusion)*grille[_Grille_Ind(i,j  ,k+1)].temperature;
							c += a*(grille[_Grille_Ind(i,j,k+1)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k+1)].conductance)*grille[_Grille_Ind(i,j  ,k+1)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k+1)].conductance);
						}
						if (grille[_Grille_Ind(i,j  ,k-1)].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += a*
									(grille[_Grille_Ind(i,j,k-1)].diffusion)*grille[_Grille_Ind(i,j  ,k-1)].temperature;
							c += a*(grille[_Grille_Ind(i,j,k-1)].diffusion);
						} else {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k-1)].conductance)*grille[_Grille_Ind(i,j  ,k-1)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k-1)].conductance);
						}
	
						grille[_Grille_Ind(i,j,k)].temperature /= c;

					//sinon on se concentre sur la frontiere.
					} else if (grille[_Grille_Ind(i  ,j  ,k  )].frontiere) {
						//on est a la frontiere on la met a jour avec le transfert thermique 
						//qui sort de l'objet, on ne considere donc que ses voisins plein.
						grille[_Grille_Ind(i,j,k)].temperature = grille[_Grille_Ind(i,j,k)].temperature0;
						c = 1;
						if (grille[_Grille_Ind(i+1,j  ,k  )].plein) {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i+1,j,k)].conductance)*grille[_Grille_Ind(i+1,j,k)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						} 

						if (grille[_Grille_Ind(i-1,j  ,k  )].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i-1,j,k)].conductance)*grille[_Grille_Ind(i-1,j,k)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						} 

						if (grille[_Grille_Ind(i  ,j+1,k  )].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j+1,k)].conductance)*grille[_Grille_Ind(i,j+1,k)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						} 

						if (grille[_Grille_Ind(i  ,j-1,k  )].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j-1,k)].conductance)*grille[_Grille_Ind(i,j-1,k)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						} 

						if (grille[_Grille_Ind(i  ,j  ,k+1)].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k+1)].conductance)*grille[_Grille_Ind(i,j,k+1)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						} 

						if (grille[_Grille_Ind(i  ,j  ,k-1)].plein)  {
							grille[_Grille_Ind(i,j,k)].temperature += b*
									(grille[_Grille_Ind(i,j,k-1)].conductance)*grille[_Grille_Ind(i,j,k-1)].temperature;
							c += b*(grille[_Grille_Ind(i,j,k)].conductance);
						} 

						grille[_Grille_Ind(i,j,k)].temperature /= c;

					}
				}
			}
		}
		//setBoundaries ( N, b, x );
	}

}


