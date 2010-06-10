#include "Sphere.h"

#define TINY_FLOAT 0.001

#include <cfloat>
#include <cmath>
#ifndef M_PI
#define M_PI 3.141592653589793238462643383279f
#endif

#include "MarchingCube.h"

#include <vector>
using namespace std;

static bool zero( float f ){
	if( fabs( f ) < TINY_FLOAT )
		return true;
	return false ;
}

Sphere::Sphere( Voxel defVox,
		float radius, Vecteur4D color, 
		int nb_plans, int nb_quarts  )
	:Object(defVox),radius(radius)
{
	this->color = color ;
	// compute AABB
	float spaceDiv  = SolverParam::getSpaceDiv();
	AABB = BoundingBox( Vecteur3D(-radius-spaceDiv ,-radius -spaceDiv ,-radius - spaceDiv),
			    Vecteur3D(radius + spaceDiv ,radius + spaceDiv ,radius + spaceDiv) );

//	generateDisplayList( nb_plans, nb_quarts );
	generateVoxels();
}

Sphere::~Sphere(){
}

/*
void Sphere::Afficher(float dt){


	//glColor4f( color.x, color.y, color.z, color.w );
	//glCallList( drawList );

    
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
*/
void Sphere::generateVoxels() {
	int nb_x,nb_y,nb_z ;
	float x_off, y_off, z_off;
	float spaceDiv  = SolverParam::getSpaceDiv();

	cout << "spaceDiv  : " << spaceDiv << endl;
	cout << "Upper     : " ; AABB.upperCorner.afficher(); cout << endl ;
	cout << "Lower     : " ; AABB.lowerCorner.afficher(); cout << endl ;

	/* Calculate number of cell in the grid */
	nb_x = (int)ceil( fabs( ( AABB.upperCorner.x - AABB.lowerCorner.x ) / spaceDiv ) );
	nb_y = (int)ceil( fabs( ( AABB.upperCorner.y - AABB.lowerCorner.y ) / spaceDiv ) );
	nb_z = (int)ceil( fabs( ( AABB.upperCorner.z - AABB.lowerCorner.z ) / spaceDiv ) );

	// Calculate the offset from the bounding box and the grid
	x_off = ( nb_x*spaceDiv + AABB.lowerCorner.x ) - AABB.upperCorner.x;
	y_off = ( nb_y*spaceDiv + AABB.lowerCorner.y ) - AABB.upperCorner.y;
	z_off = ( nb_z*spaceDiv + AABB.lowerCorner.z ) - AABB.upperCorner.z;

	// Increase the bounding box to line up with the grid
	if( !zero( x_off ) ) { 
		AABB.lowerCorner.x -= x_off/2.0f; 
		AABB.upperCorner.x += x_off/2.0f; 
	}
	if( !zero( y_off ) ) { 
		AABB.lowerCorner.y -= y_off/2.0f; 
		AABB.upperCorner.y += y_off/2.0f; 
	}
	if( !zero( z_off ) ) { 
		AABB.lowerCorner.z -= z_off/2.0f; 
		AABB.upperCorner.z += z_off/2.0f; 
	}
	
	grilleSize.x = nb_x ;
	grilleSize.y = nb_y ;
	grilleSize.z = nb_z ;

	// calculate the grid
	grille = new Voxel[nb_x*nb_y*nb_z];
	for( int k = 0 ; k < nb_z ; k++ ){
		for( int j = 0 ; j < nb_y ; j++ ){
			for( int i = 0 ; i < nb_x ; i++ ){
				Voxel &ref = grille[_Grille_Ind(i,j,k)];
				ref = defVox;
				ref.plein = false ;
				ref.frontiere = false ;
				Vecteur3I pos(i,j,k);
				ref.pos = pos ;
				if( isInside( cellToPoint( pos ) ) ){
					ref.plein = true ;
					//setVoisinBound( i,j,k );
				}
				setCornerCell( ref );
				setValuation( ref );
				Polygonise( ref );
			}
		}
	}
	for( int k = 0 ; k < nb_z ; k++ ){
		for( int j = 0 ; j < nb_y ; j++ ){
			for( int i = 0 ; i < nb_x ; i++ ){

					setVoisinBound( i,j,k );
				
				
			}
		}
	}
}

void Sphere::generateDisplayList(int nb_plans, int nb_quarts){
	drawList = glGenLists(1) ;
	vector<Vecteur3D> pred ;
	vector<Vecteur3D> cour ;
	
	for(float a= -M_PI/2 ; a < M_PI/2+M_PI/nb_plans ; a+= M_PI/nb_plans ){
		pred.push_back( Vecteur3D(radius*cosf(a)*cosf(-M_PI),
					  radius*cosf(a)*sinf(-M_PI),
					  radius*sinf(a) ) );
	}

	glNewList(drawList, GL_COMPILE );
	glBegin( GL_TRIANGLES );
	for(double b= -M_PI ; b< M_PI+M_PI/nb_quarts ; b+= M_PI/nb_quarts){
		int cpt = 0 ;
		for(double a= -M_PI/2 ; a < M_PI/2+M_PI/nb_plans ; a+= M_PI/nb_plans, cpt++ ){
			Vecteur3D p =  Vecteur3D(radius*cosf(a)*cosf(b),
						 radius*cosf(a)*sinf(b),
						 radius*sinf(a) );
			if( !cour.empty() ){
				glVertex3f( p.x, p.y, p.z);
				glVertex3f( pred[cpt-1].x, pred[cpt-1].y, pred[cpt-1].z);
				glVertex3f( cour[cpt-1].x, cour[cpt-1].y, cour[cpt-1].z);
				glVertex3f( pred[cpt-1].x, pred[cpt-1].y, pred[cpt-1].z);
				glVertex3f( p.x, p.y, p.z);		
				glVertex3f( pred[cpt].x, pred[cpt].y, pred[cpt].z);
			}
			cour.push_back( p );
		}
		pred.clear();
		pred.swap( cour );
	}
	glEnd();
	glEndList();
}
