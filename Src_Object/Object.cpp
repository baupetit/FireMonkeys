#include "Object.h"
#define NB_ITERATION_SOLVE 10
 
#include "MarchingCube.h"

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
}

Vecteur3D Object::repulse (int i, int j, int k){
	//condition i j k est donnée pour une frontiere, on cherche tous les frontieres distantes de 1
	Vecteur3D vec = Vecteur3D(0,0,0);
	if (grille[_Grille_Ind(i  ,j  ,k  )].frontiere){
		//comparaison aux 9 devant	
		if (grille[_Grille_Ind(i  ,j  ,k-1)].plein)  vec += Vecteur3D(0,0,1); 		
		if (grille[_Grille_Ind(i  ,j-1,k-1)].plein)  vec += Vecteur3D(0,1,1);
		if (grille[_Grille_Ind(i  ,j+1,k-1)].plein)  vec += Vecteur3D(0,-1,1);
		if (grille[_Grille_Ind(i-1,j  ,k-1)].plein)  vec += Vecteur3D(1,0,1); 
		if (grille[_Grille_Ind(i-1,j-1,k-1)].plein)  vec += Vecteur3D(1,1,1);
		if (grille[_Grille_Ind(i-1,j+1,k-1)].plein)  vec += Vecteur3D(1,-1,1);
		if (grille[_Grille_Ind(i+1,j  ,k-1)].plein)  vec += Vecteur3D(-1,0,1);
		if (grille[_Grille_Ind(i+1,j-1,k-1)].plein)  vec += Vecteur3D(-1,1,1);
		if (grille[_Grille_Ind(i+1,j+1,k-1)].plein)  vec += Vecteur3D(-1,-1,1);

		//comparaison aux 9 derriere		
		if (grille[_Grille_Ind(i  ,j  ,k+1)].plein)  vec += Vecteur3D(0,0,-1); 		
		if (grille[_Grille_Ind(i  ,j-1,k+1)].plein)  vec += Vecteur3D(0,1,-1);
		if (grille[_Grille_Ind(i  ,j+1,k+1)].plein)  vec += Vecteur3D(0,-1,-1);
		if (grille[_Grille_Ind(i-1,j  ,k+1)].plein)  vec += Vecteur3D(1,0,-1); 
		if (grille[_Grille_Ind(i-1,j-1,k+1)].plein)  vec += Vecteur3D(1,1,-1);
		if (grille[_Grille_Ind(i-1,j+1,k+1)].plein)  vec += Vecteur3D(1,-1,-1);
		if (grille[_Grille_Ind(i+1,j  ,k+1)].plein)  vec += Vecteur3D(-1,0,-1);
		if (grille[_Grille_Ind(i+1,j-1,k+1)].plein)  vec += Vecteur3D(-1,1,-1);
		if (grille[_Grille_Ind(i+1,j+1,k+1)].plein)  vec += Vecteur3D(-1,-1,-1);

		//comparaison aux 8 du plan
		//if (grille[_Grille_Ind(i  ,j  ,k)].plein)  vec += Vecteur3D(0,0,-1); 		
		if (grille[_Grille_Ind(i  ,j-1,k)].plein)  vec += Vecteur3D(0,1,0);
		if (grille[_Grille_Ind(i  ,j+1,k)].plein)  vec += Vecteur3D(0,-1,0);
		if (grille[_Grille_Ind(i-1,j  ,k)].plein)  vec += Vecteur3D(1,0,0); 
		if (grille[_Grille_Ind(i-1,j-1,k)].plein)  vec += Vecteur3D(1,1,0);
		if (grille[_Grille_Ind(i-1,j+1,k)].plein)  vec += Vecteur3D(1,-1,0);
		if (grille[_Grille_Ind(i+1,j  ,k)].plein)  vec += Vecteur3D(-1,0,0);
		if (grille[_Grille_Ind(i+1,j-1,k)].plein)  vec += Vecteur3D(-1,1,0);
		if (grille[_Grille_Ind(i+1,j+1,k)].plein)  vec += Vecteur3D(-1,-1,0);	

		//pivotage de Pi/2
		Vecteur3D vec_vect_y = Vecteur3D(-vec.z,0.0,vec.x);
		float b = vec_vect_y.x*vec_vect_y.x+vec_vect_y.y*vec_vect_y.y+vec_vect_y.z*vec_vect_y.z ;
		if (vec_vect_y.x*vec_vect_y.x+vec_vect_y.y*vec_vect_y.y+vec_vect_y.z*vec_vect_y.z!=0 ){
			vec_vect_y.x = (vec_vect_y.x/(sqrt(b)));
			vec_vect_y.y = (vec_vect_y.y/(sqrt(b)));
			vec_vect_y.z = (vec_vect_y.z/(sqrt(b)));
		}
		float a = (vec.x*vec_vect_y.x+vec.y*vec_vect_y.y+vec.z*vec_vect_y.z);
		vec.x = a*vec_vect_y.x + vec_vect_y.y*vec.z - vec_vect_y.z*vec.y;
		vec.y = a*vec_vect_y.y + vec_vect_y.z*vec.x - vec_vect_y.x*vec.z;
		vec.z = a*vec_vect_y.z + vec_vect_y.x*vec.y - vec_vect_y.y*vec.x;

		if (vec.x==0 && vec.y==0 && vec.z==0) return vec;
		vec.x = (vec.x/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));
		vec.y = (vec.y/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));
		vec.z = (vec.z/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));
	}
	return vec;
}


void Object::voxelConsome( Voxel *v ){
	// quand un voxel est comsomme
	if( v->frontiere ){
		float spaceDiv = SolverParam::getSpaceDiv();
		//  - deplacement de la valeur de ses huits sommets ( par spaceDiv )
		//  - re-calcul de la triangulation localement (27 cases)
		//  - recherche de la frontiere localement

		for( int l =0; l < 8; ++l ){
			v->valuation[l] += spaceDiv;
		}
		Polygonise( *v );
		v->frontiere = false ;

		//  - transmission de l'information au 26 voisins
		// 9 deriere
		Voxel *voisin;
		// 1
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y-1,v->pos.z-1)];
		voisin->valuation[_FUR_] += spaceDiv;
		Polygonise( *voisin );				
		

		// 2
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y,v->pos.z-1)];
		voisin->valuation[_FUR_] += spaceDiv;
		voisin->valuation[_FDR_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}
		
		// 3
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y+1,v->pos.z-1)];
		voisin->valuation[_FDR_] += spaceDiv;
		Polygonise( *voisin );
		
		// 4
		voisin = &grille[_Grille_Ind(v->pos.x,v->pos.y-1,v->pos.z-1)];
		voisin->valuation[_FUR_] += spaceDiv;
		voisin->valuation[_FUL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 5
		voisin = &grille[_Grille_Ind(v->pos.x,v->pos.y,v->pos.z-1)];
		voisin->valuation[_FUR_] += spaceDiv;
		voisin->valuation[_FUL_] += spaceDiv;
		voisin->valuation[_FDR_] += spaceDiv;
		voisin->valuation[_FDL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 6
		voisin = &grille[_Grille_Ind(v->pos.x,v->pos.y+1,v->pos.z-1)];
		voisin->valuation[_FDR_] += spaceDiv;
		voisin->valuation[_FDL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		
		// 7
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y-1,v->pos.z-1)];
		voisin->valuation[_FUL_] += spaceDiv;
		Polygonise( *voisin );

		// 8
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y,v->pos.z-1)];
		voisin->valuation[_FUL_] += spaceDiv;
		voisin->valuation[_FDL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 9
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y+1,v->pos.z-1)];
		voisin->valuation[_FDL_] += spaceDiv;
		Polygonise( *voisin );

		
		// 8 meme plan
		// 1
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y-1,v->pos.z)];
		voisin->valuation[_FUR_] += spaceDiv;
		voisin->valuation[_BUR_] += spaceDiv;
		Polygonise( *voisin );

		// 2
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y,v->pos.z)];
		voisin->valuation[_FUR_] += spaceDiv;
		voisin->valuation[_BUR_] += spaceDiv;
		voisin->valuation[_FDR_] += spaceDiv;
		voisin->valuation[_BDR_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 3
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y+1,v->pos.z)];
		voisin->valuation[_FDR_] += spaceDiv;
		voisin->valuation[_BDR_] += spaceDiv;
		Polygonise( *voisin );

		// 4
		voisin = &grille[_Grille_Ind(v->pos.x,v->pos.y-1,v->pos.z)];
		voisin->valuation[_FUR_] += spaceDiv;
		voisin->valuation[_FUL_] += spaceDiv;
		voisin->valuation[_BUR_] += spaceDiv;
		voisin->valuation[_BUL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 6
		voisin = &grille[_Grille_Ind(v->pos.x,v->pos.y+1,v->pos.z)];
		voisin->valuation[_FDR_] += spaceDiv;
		voisin->valuation[_FDL_] += spaceDiv;
		voisin->valuation[_BDR_] += spaceDiv;
		voisin->valuation[_BDL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 7
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y-1,v->pos.z)];
		voisin->valuation[_FUL_] += spaceDiv;
		voisin->valuation[_BUL_] += spaceDiv;
		Polygonise( *voisin );

		// 8
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y,v->pos.z)];
		voisin->valuation[_FUL_] += spaceDiv;
		voisin->valuation[_BUL_] += spaceDiv;
		voisin->valuation[_FDL_] += spaceDiv;
		voisin->valuation[_BDL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 9
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y+1,v->pos.z)];
		voisin->valuation[_FDL_] += spaceDiv;
		voisin->valuation[_BDL_] += spaceDiv;
		Polygonise( *voisin );

		
		// 9 devant
		// 1
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y-1,v->pos.z+1)];
		voisin->valuation[_BUR_] += spaceDiv;
		Polygonise( *voisin );

		// 2
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y,v->pos.z+1)];
		voisin->valuation[_BUR_] += spaceDiv;
		voisin->valuation[_BDR_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 3
		voisin = &grille[_Grille_Ind(v->pos.x-1,v->pos.y+1,v->pos.z+1)];
		voisin->valuation[_BDR_] += spaceDiv;
		Polygonise( *voisin );

		// 4
		voisin = &grille[_Grille_Ind(v->pos.x,v->pos.y-1,v->pos.z+1)];
		voisin->valuation[_BUR_] += spaceDiv;
		voisin->valuation[_BUL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 5
		voisin = &grille[_Grille_Ind(v->pos.x,v->pos.y,v->pos.z+1)];
		voisin->valuation[_BUR_] += spaceDiv;
		voisin->valuation[_BUL_] += spaceDiv;
		voisin->valuation[_BDR_] += spaceDiv;
		voisin->valuation[_BDL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 6
		voisin = &grille[_Grille_Ind(v->pos.x,v->pos.y+1,v->pos.z+1)];
		voisin->valuation[_BDR_] += spaceDiv;
		voisin->valuation[_BDL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 7
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y-1,v->pos.z+1)];
		voisin->valuation[_BUL_] += spaceDiv;
		Polygonise( *voisin );

		// 8
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y,v->pos.z+1)];
		voisin->valuation[_BUL_] += spaceDiv;
		voisin->valuation[_BDL_] += spaceDiv;
		Polygonise( *voisin );
		if( voisin->plein ){
			voisin->plein = false ;
			voisin->frontiere = true ;
		}

		// 9
		voisin = &grille[_Grille_Ind(v->pos.x+1,v->pos.y+1,v->pos.z+1)];
		voisin->valuation[_BDL_] += spaceDiv;
		Polygonise( *voisin );
		
		cout << "voxelConsome" << endl;

		// mise a jour de la force de repulsion
		Vecteur3I pos = v-> pos ;
		for( int k = -1; k<2 ; ++k ){
			for( int j = -1; j<2 ; ++j ){
				for( int i = -1; i<2 ; ++i ){
					repulse( pos.x+i, pos.y+j, pos.z+k);
				}
			}
		}
	}
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
}
