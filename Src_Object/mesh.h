/**
 * Projet G3D 
 *
 * Classe Mesh : represente une mesh
 */


#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
using namespace std;

#include "mglut.h"
#include "utils.h"

/** Structs **/
/* Triangle with index of its three vertices */
struct TRIANGLE {
	int v0, v1, v2 ;
} ;

/* Edge with index of its two vertices */
enum Edge_End { EdgeEnd1, EdgeEnd2 };
struct Edge {
	int v0, v1;
} ;

/* Cell with coordinates of its 8 corners */
/* and its own position */
struct Corner {
	int ci, cj, ck;
} ;

struct Cell {
	int ci, cj, ck ;
	Corner BDL, BDR, BUL, BUR; // back face
	Corner FDL, FDR, FUL, FUR; // front face
} ;

/* Class : Mesh                                */
/* brief : handles mesh and .obj file          */
/*                                             */
/*---------------------------------------------*/
class Mesh {
protected :
	/* Attributes */
	/* Mesh properties */
	int nb_vertices ;
	int nb_triangles ;

	/* vertices */
	vector<D3Vector> vertices ;
	/* vertices normals */
	vector<D3Vector> vnormals ;
	/* triangles normals */
	vector<D3Vector> tnormals ;
	/* triangles coordinates */
	vector<TRIANGLE> triangles ;
	/* vertices pseudo normals */
	vector<D3Vector> pseudoVNormals;
	/* edges pseudo normals */
	//vector<D3Vector> pseudoENormals;
	/* adjacents triangles */
	vector<TRIANGLE> adjacentTRIANGLEs;
	/* unique edges */
	vector<Edge> edges ;

	/* SDF related attributes */
	/* SDF offset */
	D3Vector offset;
	/* bounding box ( as upper corner and lower corner */
	D3Vector upperC, lowerC ;
	/* number of cells */
	int nb_x, nb_y, nb_z ;
	/* width of cells */
	double d_x, d_y, d_z ;
	/* values of Phi ( at cell corners ) */
	vector< vector< vector< double > > > phiGrid ;
	/* closest TRIANGLE at cell corner */
	vector< vector< vector< int > > >   closestTRIANGLEs ;
	/* greatest phi value */
	double maxPhi ;

	/* Center of mass & inertia matrix              */
	/* These are calculated using Mirtich algorithm */
	D3Vector center_of_mass;
	D3Matrix inertia_matrix;
	
	int A, B, C; // Alpha , Beta, Gamma ;
	/* projection integrals */
	double P1, Pa, Pb, Paa, Pab, Pbb, Paaa, Paab, Pabb, Pbbb ;
	/* face integrals */
	double Fa, Fb, Fc, Faa, Fbb, Fcc, Faaa, Fbbb, Fccc, Faab, Fbbc, Fcca ;
	/* volume integrals */
	double T0;
	double density;
	D3Vector  T1, T2, TP;
	/* face offset */
	vector<double> faceOffset;
	/* mass */
	double mass ;


	/* Material */
	D3Vector         color    ;
	D3Vector         diffuse  ;
	D3Vector         specular ;
	GLfloat          shininess ;

	/* Private Methods */
	void translateMeshToCOM();

public :
	/* Constructors                                */
	/*---------------------------------------------*/

	Mesh( D3Vector color      = D3Vector( 0.3f, 0.5f, 0.2f ),
	      D3Vector diffuse    = D3Vector( 0.3f, 0.5f, 0.2f ), 
	      D3Vector specular   = D3Vector( 0.3f, 0.5f, 0.2f ),
	      double   shininess  = 0.5f,
	      D3Vector offset     = D3Vector(0.4, 0.4, 0.4 ),
	      double   d_x        = 0.1f,
	      double   d_y        = 0.1f,
	      double   d_z        = 0.1f );
	Mesh( string &filename );  // load the mesh from a .obj file
	Mesh( string& filename, double mass, double density = 1.0f );

	/* Methods                                     */
	/*---------------------------------------------*/
	/* general init */
	void init( string&  filename, double mass, 
		   double   density      = 1.0f,
		   D3Vector color      = D3Vector( 0.3f, 0.5f, 0.2f ),
		   D3Vector diffuse    = D3Vector( 0.3f, 0.5f, 0.2f ), 
		   D3Vector specular   = D3Vector( 0.3f, 0.5f, 0.2f ),
		   double   shininess  = 0.5f,
		   D3Vector offset     = D3Vector(0.4, 0.4, 0.4 ),
		   double   d_x        = 0.1f,
		   double   d_y        = 0.1f,
		   double   d_z        = 0.1f );

	/* getters and setters */
	inline int      getNBTRIANGLEs() const;
	inline int      getNBVertices() const;
	
	inline const vector<D3Vector>& getVertices() const ;
	inline const vector<Edge>& getEdges() const ;
	inline D3Vector getEdgeEnd(const Edge& e  , Edge_End v ) const ;

	inline D3Vector getTRIANGLENormal( int t ) const ;

	inline D3Vector getCenterOfMass() const;
	inline D3Matrix getInertiaMatrix() const;

	inline double getMass() const ;
	inline void   setMass( double mass ) ;

	inline double getDensity() const ;
	inline void   setDensity( double density ) ;


	inline D3Vector getColor() const ;
	inline void     setColor( D3Vector color ) ;
	
	inline D3Vector getDiffuse() const ;
	inline void     setDiffuse( D3Vector diffuse ) ;
	
	inline D3Vector getSpecular() const ;
	inline void     setSpecular( D3Vector specular ) ;

	inline double   getShininess() const ;
	inline void     setShininess( double shininess ) ;

	inline D3Vector getUpperBound() const ;
	inline D3Vector getLowerBound() const ;
	
	/* real stuff */
	void     loadFromFile( string &filename); // ...
	
	void     calcNormals();         // compute face and vertices normals
	void     calcAdjacentTRIANGLE(); // compute the adjacent triangle
	void     calcUniqueEdges();     // compute unique edges
	void     calcPseudoVNormals();  // compute vertices pseudo normals
	void     calcPseudoENormals();  // compute edge pseudo normals
	void     calcSDF();             // compute the Mesh SDF
	void     calcBB();              // compute the grid Bounding Box

	void     display(bool phi = false) ; // display the mesh in the current
	                                     // OpenGL context
	
	/* SDF related method */
	/* ------------------ */
	/* Eval phi value at position p 
	   if interpolate is true, use trilinear interpolation,
	   else use closest corner distance */
	double    phiEval( D3Vector& p, bool interpolate = false );
	/* Evaluate the gradient of phi at point p */
	D3Vector gradPhi( D3Vector& p );
	/* Find the closest TRIANGLE to the point p 
	 * return triangle index
	 */
	int      closestTRIANGLE( D3Vector& p, double& dist, int& ce, int &cv, D3Vector& cd);
	/* Return true if the line segment p0-p1 intersects triangle t
	 * and set pi to point of intersection if true */
	bool	 intersectsTRIANGLE( D3Vector& p0, D3Vector& p1, 
				     int t, D3Vector &pi );
	/* Map a point to the closest cell of the domain
	 * return false if outside of the domain
	 */
	bool     mapPointToCell( D3Vector &p, Cell &c );
	
	/* Calculate the distance to triangle */
	double    calcDist( D3Vector& v0, 
			   D3Vector& v1, 
			   D3Vector& v2, 
			   D3Vector& p, 
			   int& v , int& e ,
			   D3Vector& d );
	/* Return true if the sign of phi if negative 
	 * i.e. the point is inside the Mesh */
	bool     calcPhiSign( int t , int v, int e, D3Vector& d );
	
	/* Calculate the Gradient at a given corner */
	D3Vector calcGradCorner( Corner& c );
	
	/* Return the lower corner position of a cell
	 */
	D3Vector lcCell( Cell& c );
	
	/* Map a cell to its corner indexes
	 */
	void     mapCellToCorner( Cell &c );

	/* Volume integration related method */
	/* --------------------------------- */
	/* calculate center of mass & inertia matrix */
	void      calcCOMAndIM();
	void      compVolumeIntegrals(); // calculate various integrals
	void      compFaceIntegrals( int t );
	void      compProjectionIntegrals( int t );
};

/* inline methods */
/******************/
int Mesh::getNBTRIANGLEs() const { return nb_triangles ; }

int Mesh::getNBVertices() const { return nb_vertices ; }

const vector<D3Vector>& Mesh::getVertices() const { return vertices; }

const vector<Edge>& Mesh::getEdges() const { return edges; }

D3Vector Mesh::getEdgeEnd( const Edge& e  , Edge_End v ) const { 
	if( v == EdgeEnd1 )return vertices[e.v0];
	else return vertices[e.v1] ;
}
D3Vector Mesh::getTRIANGLENormal( int t ) const {
	if( t < 0 || t >= nb_triangles )
		return D3Vector(0,0,0);
	return tnormals[t];
}
D3Vector Mesh::getCenterOfMass() const {
	return center_of_mass;
}
D3Matrix Mesh::getInertiaMatrix() const {
	return inertia_matrix;
}
double Mesh::getMass() const {
	return mass ;
}
double Mesh::getDensity() const {
	return density ;
}
void Mesh::setMass( double mass ) {
	this->mass = mass ;
}
void Mesh::setDensity( double density ) {
	this->density = density ;
}

D3Vector Mesh::getColor() const {
	return color ;
}
void     Mesh::setColor( D3Vector color ) {
	this->color = color ;
}
D3Vector Mesh::getDiffuse() const {
	return diffuse ;
}
void     Mesh::setDiffuse( D3Vector diffuse ) {
	this->diffuse = diffuse ;
}
D3Vector Mesh::getSpecular() const {
	return specular ;
}
void     Mesh::setSpecular( D3Vector specular ) {
	this->specular = specular ;
}
double   Mesh::getShininess() const {
	return shininess ;
}
void     Mesh::setShininess( double shininess ) {
	this->shininess = shininess ;
}
D3Vector Mesh::getUpperBound() const {
	return lowerC + offset;
}
D3Vector Mesh::getLowerBound() const {
	return upperC - offset;
}
#endif
