#include "ObjectMesh.h"
#include "MarchingCube.h"

ObjectMesh::ObjectMesh( Voxel defVox,
			string filename, Vecteur4D color )
	:Object(defVox)
{
	this->color = color;
	obj = new Mesh();
	obj->init( filename , 1.0, 1.0,
		   D3Vector( 0.3f, 0.5f, 0.2f ),D3Vector( 0.3f, 0.5f, 0.2f ),D3Vector( 0.3f, 0.5f, 0.2f ),0.5,
		   D3Vector( 2*SolverParam::getSpaceDiv(),  2*SolverParam::getSpaceDiv(),  2*SolverParam::getSpaceDiv() ),
		   SolverParam::getSpaceDiv(),SolverParam::getSpaceDiv(),SolverParam::getSpaceDiv()
		);

	D3Vector temp1 = obj->getLowerBound();
	D3Vector temp2 = obj->getUpperBound();
	AABB = BoundingBox( Vecteur3D(temp1(0),temp1(1),temp1(2)),
			    Vecteur3D(temp2(0),temp2(1),temp2(2)));
	generateVoxels();
}

ObjectMesh::~ObjectMesh(){
}

void ObjectMesh::generateVoxels(){
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
				Voxel *ref = &grille[_Grille_Ind(i,j,k)];
				*ref = defVox;
				ref->plein = false ;
				ref->frontiere = false ;
				Vecteur3I pos(i,j,k);
				ref->pos = pos ;
				if( isInside( cellToPoint( pos ) ) ){
					ref->plein = true ;
				}
				setCornerCell( *ref );
				setValuation( *ref );
				Polygonise( *ref );
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
