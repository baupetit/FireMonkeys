#include "mesh.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cfloat>

#include <list>

#include <iostream>

using namespace std;



#define BUFFSIZE 256

/********************************************************/
/* Constructors                                         */
/********************************************************/
Mesh::Mesh( D3Vector color      ,
	    D3Vector diffuse    ,
	    D3Vector specular   ,
	    double   shininess  ,
	    D3Vector offset     ,
	    double   d_x        ,
	    double   d_y        ,
	    double   d_z         ) 
	:offset(offset),d_x(d_x),d_y(d_y),d_z(d_z),
	 color( color ), diffuse( diffuse ), specular( specular ),
	 shininess( shininess )
{
}


Mesh::Mesh( string &filename ){
	loadFromFile(filename);
}

Mesh::Mesh( string& filename, double mass, double density ) 
	:  density( density ),mass( mass )
{
	loadFromFile(filename);
}

/* general init */
void Mesh::init( string&  filename, double mass, 
		 double   density    ,
		 D3Vector color      ,
		 D3Vector diffuse    ,
		 D3Vector specular   ,
		 double   shininess  ,
		 D3Vector offset     ,
		 double   d_x        ,
		 double   d_y        ,
		 double   d_z        )
{
	setMass( mass );
	setDensity( density );
	setColor( color );
	setDiffuse( diffuse );
	setSpecular( specular );
	setShininess( shininess );
	this->offset = offset ;
	this->d_x = d_x ;
	this->d_y = d_y ;
	this->d_z = d_z ;
	
	// load Mesh
	loadFromFile( filename );

	// compute center of mass etc
	calcCOMAndIM();

	// translate Mesh to COM
	translateMeshToCOM();
	
	// recompute COM and IM : this shouldn't be done but whatever
	// calcCOMAndIM();
	
	// recompute BB
	calcBB();
	// calc Phi
	calcSDF();
}

void Mesh::translateMeshToCOM(){
	vector<D3Vector>::iterator it;
	for( it = vertices.begin(); it != vertices.end() ; it++ ){
		*it -= center_of_mass ;
	}
}

/********************************************************/
/* SDF  Function                                        */
/********************************************************/
void     Mesh::mapCellToCorner( Cell &c ){

	c.FDL.ci = c.ci   ; c.FDL.cj = c.cj   ; c.FDL.ck = c.ck ; // left down forward
	c.FDR.ci = c.ci+1 ; c.FDR.cj = c.cj   ; c.FDR.ck = c.ck ; // right down forward
	c.FUL.ci = c.ci   ; c.FUL.cj = c.cj+1 ; c.FUL.ck = c.ck ; // left up forward
	c.FUR.ci = c.ci+1 ; c.FUR.cj = c.cj+1 ; c.FUR.ck = c.ck ; // right up forward

	c.BDL.ci = c.ci   ; c.BDL.cj = c.cj   ; c.BDL.ck = c.ck+1 ; // left back down
	c.BDR.ci = c.ci+1 ; c.BDR.cj = c.cj   ; c.BDR.ck = c.ck+1 ; // right back down
	c.BUL.ci = c.ci   ; c.BUL.cj = c.cj+1 ; c.BUL.ck = c.ck+1 ; // left back up
	c.BUR.ci = c.ci+1 ; c.BUR.cj = c.cj+1 ; c.BUR.ck = c.ck+1 ; // right back up



}

D3Vector Mesh::lcCell( Cell& c ){
	D3Vector lc( lowerC );
	lc(0) += c.ci*d_x;
	lc(1) += c.cj*d_y;
	lc(2) += c.ck*d_z;	
	return lc;
}

bool Mesh::mapPointToCell( D3Vector &p, Cell &c ){
	if( isnan( p(0) ) || isnan( p(1) ) || isnan( p(2) ) )
		exit(4);

	// test si le point est dans le domaine
	if( p(0) < lowerC(0) ||
	    p(1) < lowerC(1) ||
	    p(2) < lowerC(2) ||
	    p(0) > upperC(0) ||
	    p(1) > upperC(1) ||
	    p(2) > upperC(2) 
		)
		return false ;

	//sinon
	c.ci = floor( ( p(0) - lowerC(0) ) / d_x );
	c.cj = floor( ( p(1) - lowerC(1) ) / d_y );
	c.ck = floor( ( p(2) - lowerC(2) ) / d_z );
	
	return true;
}

double    Mesh::phiEval( D3Vector& p, bool interpolate ){
	D3Vector             w, clc ;
	Cell                 c ;

       	// if outside of domain return maxPhi
	if( mapPointToCell( p , c ) ){
		
		mapCellToCorner( c );
		
		if( interpolate ) {
			clc = lcCell( c );
			
			// Translate the cube to the origin and scale to the unit cube
			w(0) = ( p(0) - clc(0) ) / d_x;
			w(1) = ( p(1) - clc(1) ) / d_y;
			w(2) = ( p(2) - clc(2) ) / d_z;
			
			return
				(1.0f-w(0))*(1.0f-w(1))*(1.0f-w(2))*phiGrid[c.FDL.ci][c.FDL.cj][c.FDL.ck] + // Left, Down, Forward
				w(0)       *(1.0f-w(1))*(1.0f-w(2))*phiGrid[c.FDR.ci][c.FDR.cj][c.FDR.ck] + // Right, Down, Forward
				(1.0f-w(0))*w(1)       *(1.0f-w(2))*phiGrid[c.FUL.ci][c.FUL.cj][c.FUL.ck] + // Left, Up, Forward
				w(0)       *w(1)       *(1.0f-w(2))*phiGrid[c.FUR.ci][c.FUR.cj][c.FUR.ck] + // Right, Up, Forward
				(1.0f-w(0))*(1.0f-w(1))*w(2)       *phiGrid[c.BDL.ci][c.BDL.cj][c.BDL.ck] + // Left, Down, Back
				w(0)       *(1.0f-w(1))*w(2)       *phiGrid[c.BDR.ci][c.BDR.cj][c.BDR.ck] + // Right, Down, Back
				(1.0f-w(0))*w(1)       *w(2)       *phiGrid[c.BUL.ci][c.BUL.cj][c.BUL.ck] + // Left, Up, Back
				w(0)       *w(1)       *w(2)       *phiGrid[c.BUR.ci][c.BUR.cj][c.BUR.ck] ; // Right, Up, Back
		} else {
			double dist;
			int   e,v;
			D3Vector d;
			int   t = closestTRIANGLE( p , dist, e , v , d );
			if( dist != 0.0f ) 
				if( calcPhiSign( t, v, e, d) ) 
					dist = -dist;
			return dist ;
		}
	}
	return maxPhi;
}

D3Vector Mesh::gradPhi( D3Vector& p ){
	D3Vector grad(0.,0.,0.);
	D3Vector clc, w, gradC[8];
	
	Cell     c;
	// if outside of the domain return (0,0,0)
	if( mapPointToCell( p, c ) ){
		// use trilinear interpolation
		clc = lcCell( c );
		
		// get corner grad values
		gradC[ 0 ] = calcGradCorner( c.FDL );
		gradC[ 1 ] = calcGradCorner( c.FDR );
		gradC[ 2 ] = calcGradCorner( c.FUL );
		gradC[ 3 ] = calcGradCorner( c.FUR );
		gradC[ 4 ] = calcGradCorner( c.BDL );
		gradC[ 5 ] = calcGradCorner( c.BDR );
		gradC[ 6 ] = calcGradCorner( c.BUL );
		gradC[ 7 ] = calcGradCorner( c.BUR );
		
		// calc the grad
		w(0) = ( p(0) - clc(0) ) / d_x ;
		w(1) = ( p(1) - clc(1) ) / d_y ;
		w(2) = ( p(2) - clc(2) ) / d_z ;
		
		// ( grad in the x axis 
		grad(0) = 
			( 1.0f - w(0) ) * ( 1.0f -w(1) ) * ( 1.0f - w(2) ) * gradC[0](0) + 
			w(0)            * ( 1.0f -w(1) ) * ( 1.0f - w(2) ) * gradC[1](0) + 
			( 1.0f - w(0) ) * w(1)           * ( 1.0f - w(2) ) * gradC[2](0) + 
			w(0)            * w(1)           * ( 1.0f - w(2) ) * gradC[3](0) + 
			( 1.0f - w(0) ) * ( 1.0f -w(1) ) * w(2)            * gradC[4](0) + 
			w(0)            * ( 1.0f -w(1) ) * w(2)            * gradC[5](0) + 
			( 1.0f - w(0) ) * w(1)           * w(2)            * gradC[6](0) + 
			w(0)            * w(1)           * w(2)            * gradC[7](0);  

		// ( grad in the y axis 
		grad(1) = 
			( 1.0f - w(0) ) * ( 1.0f -w(1) ) * ( 1.0f - w(2) ) * gradC[0](1) + 
			w(0)            * ( 1.0f -w(1) ) * ( 1.0f - w(2) ) * gradC[1](1) + 
			( 1.0f - w(0) ) * w(1)           * ( 1.0f - w(2) ) * gradC[2](1) + 
			w(0)            * w(1)           * ( 1.0f - w(2) ) * gradC[3](1) + 
			( 1.0f - w(0) ) * ( 1.0f -w(1) ) * w(2)            * gradC[4](1) + 
			w(0)            * ( 1.0f -w(1) ) * w(2)            * gradC[5](1) + 
			( 1.0f - w(0) ) * w(1)           * w(2)            * gradC[6](1) + 
			w(0)            * w(1)           * w(2)            * gradC[7](1);  

		// ( grad in the y axis 
		grad(2) = 
			( 1.0f - w(0) ) * ( 1.0f -w(1) ) * ( 1.0f - w(2) ) * gradC[0](2) + 
			w(0)            * ( 1.0f -w(1) ) * ( 1.0f - w(2) ) * gradC[1](2) + 
			( 1.0f - w(0) ) * w(1)           * ( 1.0f - w(2) ) * gradC[2](2) + 
			w(0)            * w(1)           * ( 1.0f - w(2) ) * gradC[3](2) + 
			( 1.0f - w(0) ) * ( 1.0f -w(1) ) * w(2)            * gradC[4](2) + 
			w(0)            * ( 1.0f -w(1) ) * w(2)            * gradC[5](2) + 
			( 1.0f - w(0) ) * w(1)           * w(2)            * gradC[6](2) + 
			w(0)            * w(1)           * w(2)            * gradC[7](2);  

	}
	return grad;
}

int      Mesh::closestTRIANGLE( D3Vector& p, double& dist, int& ce, int &cv, D3Vector& cd){
	Cell                c;
	double              dist2, minDist ;
	int                 closestT;
	int                 v0,v1,v2;
	D3Vector            d;
	list<int>           tList;
	list<int>::iterator it;

	minDist = DBL_MAX ;

	// if p is outside of the domain return invalid triangle
	if( !mapPointToCell( p , c ) )
		return -1;
	
	// getCornerindices
	mapCellToCorner( c );

	// get the list of involved triangles
	tList.push_back( closestTRIANGLEs[c.BDL.ci][c.BDL.cj][c.BDL.ck] );
	tList.push_back( closestTRIANGLEs[c.BDR.ci][c.BDR.cj][c.BDR.ck] );
	tList.push_back( closestTRIANGLEs[c.BUL.ci][c.BUL.cj][c.BUL.ck] );
	tList.push_back( closestTRIANGLEs[c.BUR.ci][c.BUR.cj][c.BUR.ck] );

	tList.push_back( closestTRIANGLEs[c.FDL.ci][c.FDL.cj][c.FDL.ck] );
	tList.push_back( closestTRIANGLEs[c.FDR.ci][c.FDR.cj][c.FDR.ck] );
	tList.push_back( closestTRIANGLEs[c.FUL.ci][c.FUL.cj][c.FUL.ck] );
	tList.push_back( closestTRIANGLEs[c.FUR.ci][c.FUR.cj][c.FUR.ck] );
	
	// get reed of redundant triangles
	tList.unique();

	// look for the closest
	for( it = tList.begin() ; it != tList.end() ; it++ ){
		int e = -1, v = -1;
		v0 = triangles[*it].v0 ;
		v1 = triangles[*it].v1 ;
		v2 = triangles[*it].v2 ;

		dist2 = calcDist( vertices[v0],
				  vertices[v1],
				  vertices[v2],
				  p, v, e, d );
		
		if( dist2 < minDist ){
			minDist  = dist2 ;
			dist = dist2;
			cv = v;
			ce = e;
			cd = d;
			closestT = *it  ;
		}
	}
	return closestT;
}

bool	 Mesh::intersectsTRIANGLE( D3Vector& p0, D3Vector& p1, 
				   int t, D3Vector &pi ){
	int			v0 , v1, v2;
	D3Vector		n, d, u, v, w;
	double			angle, r, uu, uv, vv, wu, wv, D, a, b;

	/* init */
	v0 = triangles[t].v0 ;
	v1 = triangles[t].v1 ;
	v2 = triangles[t].v2 ;

	n = tnormals[t];
	
	d = p1 - p0 ;
	
	angle = dot_product( n, d );

	// segment is parallel to triangle plane
	if( zero( angle ) ) return false ;

	r = -dot_product( n, p0-vertices[v0] ) /angle;
	// intersection is on the line but not within segment
	if( r < 0.0f || r > 1.0f ) return false ;
	
	pi = p0 +r*d ;

	// check if the point of intersection is inside the triangle
	// area
	u = vertices[v1] - vertices[v0] ;
	v = vertices[v2] - vertices[v0] ;
	w = pi - vertices[v0] ;
	
	uu = u.squareNorm();
	uv = dot_product( u , v );
	vv = v.squareNorm();
	wu = dot_product( w , u );
	wv = dot_product( w , v );
	D  = uv*uv - uu*vv ;
	
	a = ( uv*wv - vv*wu ) / D ;
	if( a < 0.0f || a > 1.0f ) return false ;
	b = ( uv*wu - uu*wv ) / D ;
	if( b < 0.0f || (a + b) > 1.0f ) return false ;

	return true;
}

double    Mesh::calcDist( D3Vector& v0, 
			  D3Vector& v1, 
			  D3Vector& v2, 
			  D3Vector& p, 
			  int& v , int& e ,
			  D3Vector& d ){
	
	D3Vector diff    ;
	D3Vector e0      ; 
	D3Vector e1      ;
	double    A00     ;
	double    A01     ;
	double    A11     ;
	double    B0      ;
	double    B1      ;
	double    C       ;
	double    DET     ;
	double    S       ;
	double    T       ;
	double    sqrDist ;
	int      lv, le  ;
	
	/* init */
	diff = v0 - p ;
	e0   = v1 - v0 ;
	e1   = v2 - v0 ;
	A00  = e0.squareNorm() ;
	A01  = dot_product( e0 , e1 ) ;
	A11  = e1.squareNorm() ;
	B0   = dot_product( diff , e0 ) ;
	B1   = dot_product( diff , e1 ) ;
	C    = diff.squareNorm() ;
	DET  = fabs( A00*A11 - A01*A01 ) ;
	S    = A01*B1 - A11*B0 ;
	T    = A01*B0 - A00*B1 ;
	lv   = -1 ;
	le   = -1 ;
	
	if ( S + T <= DET )
	{
		if ( S < 0.0f )
		{
			if ( T < 0.0f )  // Region 4
			{
				if ( B0 < 0.0f )
				{
					T = 0.0f;
					if ( -B0 >= A00 )
					{
						S = 1.0f;
						sqrDist = A00 + 2.0f * B0 + C;
						lv = 1;
					}
					else
					{
						S = -B0/A00;
						sqrDist = B0 * S + C;
						le = 0;
					}
				}
				else
				{
					S = 0.0f;
					if ( B1 >= 0.0f )
					{
						T = 0.0f;
						sqrDist = C;
						lv = 0;
					}
					else if ( -B1 >= A11 )
					{
						T = 1.0f;
						sqrDist = A11 + 2.0f * B1 + C;
						lv = 2;
					}
					else
					{
						T = -B1 / A11;
						sqrDist = B1 * T + C;
						le = 1;
					}
				}
			}
			else  // Region 3
			{
				S = 0.0f;
				if ( B1 >= 0.0f )
				{
					T = 0.0f;
					sqrDist = C;
					lv = 0;
				}
				else if ( -B1 >= A11 )
				{
					T = 1.0f;
					sqrDist = A11 + 2.0f * B1 + C;
					lv = 2;
				}
				else
				{
					T = -B1 / A11;
					sqrDist = B1 * T + C;
					le = 1;
				}
			}
		}
		else if ( T < 0.0f )  // Region 5
		{
			T = 0.0f;
			if ( B0 >= 0.0f )
			{
				S = 0.0f;
				sqrDist = C;
				lv = 0;
			}
			else if ( -B0 >= A00 )
			{
				S = 1.0f;
				sqrDist = A00 + 2.0f * B0 + C;
				lv = 1;
			}
			else
			{
				S = -B0 / A00;
				sqrDist = B0 * S + C;
				le = 0;
			}
		}
		else  // Region 0
		{
			// Minimum at interior lv
			double invDet = 1.0f / DET;
			S *= invDet;
			T *= invDet;
			sqrDist = S * ( A00 * S + A01 * T + 2.0f * B0) +
				T * ( A01 * S + A11 * T + 2.0f * B1 ) + C;
		}
	}
	else
	{
		double tmp0, tmp1, numer, denom;

		if ( S < 0.0f )  // Region 2
		{
			tmp0 = A01 + B0;
			tmp1 = A11 + B1;
			if ( tmp1 > tmp0 )
			{
				numer = tmp1 - tmp0;
				denom = A00 - 2.0f * A01 + A11;
				if ( numer >= denom )
				{
					S = 1.0f;
					T = 0.0f;
					sqrDist = A00 + 2.0f * B0 + C;
					lv = 1;
				}
				else
				{
					S = numer / denom;
					T = 1.0f - S;
					sqrDist = S * ( A00 * S + A01 * T + 2.0f * B0 ) +
						T * ( A01 * S + A11 * T + 2.0f * B1 ) + C;
					le = 2;
				}
			}
			else
			{
				S = 0.0f;
				if ( tmp1 <= 0.0f )
				{
					T = 1.0f;
					sqrDist = A11 + 2.0f * B1 + C;
					lv = 2;
				}
				else if ( B1 >= 0.0f )
				{
					T = 0.0f;
					sqrDist = C;
					lv = 0;
				}
				else
				{
					T = -B1 / A11;
					sqrDist = B1 * T + C;
					le = 1;
				}
			}
		}
		else if ( T < 0.0f )  // Region 6
		{
			tmp0 = A01 + B1;
			tmp1 = A00 + B0;
			if ( tmp1 > tmp0 )
			{
				numer = tmp1 - tmp0;
				denom = A00 - 2.0f * A01 + A11;
				if ( numer >= denom )
				{
					T = 1.0f;
					S = 0.0f;
					sqrDist = A11 + 2.0f * B1 + C;
					lv = 2;
				}
				else
				{
					T = numer / denom;
					S = 1.0f - T;
					sqrDist = S * ( A00 * S + A01 * T + 2.0f * B0 ) +
						T * ( A01 * S + A11 * T + 2.0f * B1 ) + C;
					le = 2;
				}
			}
			else
			{
				T = 0.0f;
				if ( tmp1 <= 0.0f )
				{
					S = 1.0f;
					sqrDist = A00 + 2.0f * B0 + C;
					lv = 1;
				}
				else if ( B0 >= 0.0f )
				{
					S = 0.0f;
					sqrDist = C;
					lv = 0;
				}
				else
				{
					S = -B0 / A00;
					sqrDist = B0 * S + C;
					le = 0;
				}
			}
		}
		else  // Region 1
		{
			numer = A11 + B1 - A01 - B0;
			if ( numer <= 0.0f )
			{
				S = 0.0f;
				T = 1.0f;
				sqrDist = A11 + 2.0f * B1 + C;
				lv = 2;
			}
			else
			{
				denom = A00 - 2.0f * A01 + A11;
				if ( numer >= denom )
				{
					S = 1.0f;
					T = 0.0f;
					sqrDist = A00 + 2.0f * B0 + C;
					lv = 1;
				}
				else
				{
					S = numer / denom;
					T = 1.0f - S;
					sqrDist = S * ( A00 * S + A01 * T + 2.0f * B0 ) +
						T * ( A01 * S + A11 * T + 2.0f * B1 ) + C;
					le = 2;
				}
			}
		}
	}
	// take account of numerical round off
	if( sqrDist < 0.0f ){
		sqrDist = 0.0f ;
	}

	d = p - ( v0 + S * e0 + T * e1 );
	v = lv;
	e = le;

	return sqrt( sqrDist );
}

D3Vector Mesh::calcGradCorner( Corner& c ){
	D3Vector grad(0.,0.,0.);
	
	// if the corner is too close to the border stop
	if( ( c.ci + 2 ) > nb_x || 
	    ( c.cj + 2 ) > nb_y || 
	    ( c.ck + 2 ) > nb_z ||
	    c.ci < 2 || 
	    c.cj < 2 || 
	    c.ck < 2 ) return grad;

	// now we are sure not to step outside of the grid
	grad(0) = ( -phiGrid[c.ci+2][c.cj][c.ck] +
		    8 * ( phiGrid[c.ci+1][c.cj][c.ck] 
			  - phiGrid[c.ci-1][c.cj][c.ck] ) +
		    phiGrid[c.ci-2][c.cj][c.ck] ) / (d_x*12);

	grad(1) = ( -phiGrid[c.ci][c.cj+2][c.ck] +
		    8 * ( phiGrid[c.ci][c.cj+1][c.ck] 
			  - phiGrid[c.ci][c.cj-1][c.ck] ) +
		    phiGrid[c.ci][c.cj-2][c.ck] ) / (d_y*12);

	grad(2) = ( -phiGrid[c.ci][c.cj][c.ck+2] +
		    8 * ( phiGrid[c.ci][c.cj][c.ck+1] 
			  - phiGrid[c.ci][c.cj][c.ck-1] ) +
		    phiGrid[c.ci][c.cj][c.ck-2] ) / (d_z*12);
	

	return grad ;
}

bool     Mesh::calcPhiSign( int t , int v, int e, D3Vector& d ){

	if( v==-1 && e==-1 ){
		return ( 0.0f > dot_product( tnormals[t] , d ) ) ;
	}
	else if( e==-1 ){
		int vert;
		if( v==0 ){
			vert = triangles[t].v0 ;
		} else if ( v==1 ){
			vert = triangles[t].v1 ;
		} else {
			vert = triangles[t].v2 ;
		}
		return ( 0.0f > dot_product( pseudoVNormals[vert] , d ) ) ;
	} 
	else {
		int edge;
		if( e==0 ){
			edge = adjacentTRIANGLEs[t].v0 ;
		} else if ( e==1 ){
			edge = adjacentTRIANGLEs[t].v1 ;
		} else {
			edge = adjacentTRIANGLEs[t].v2 ;
		}
		D3Vector n = M_PI*tnormals[ edge ] + M_PI*tnormals[ t ];
		n.normalize();
		return ( 0.0f > dot_product( n , d ) );		
	}
	return false;
}

/********************************************************/
/* Computing Function                                   */
/********************************************************/
void     Mesh::calcBB(){
	int i;
	D3Vector tmp = 2.*D3Vector(d_x,d_y,d_z) + offset;
	
	lowerC = D3Vector( DBL_MAX , DBL_MAX , DBL_MAX );
	upperC = D3Vector( -DBL_MAX , -DBL_MAX , -DBL_MAX );

	// calculate the mesh BB
	for( i=0 ; i<nb_vertices ; ++i ){
		if( vertices[i](0) < lowerC(0) ){
			lowerC(0) = vertices[i](0);
		} 
		if( vertices[i](0) > upperC(0) ){
			upperC(0) = vertices[i](0);
		}
		if( vertices[i](1) < lowerC(1) ){
			lowerC(1) = vertices[i](1);
		} 
		if( vertices[i](1) > upperC(1) ){
			upperC(1) = vertices[i](1);
		}
		if( vertices[i](2) < lowerC(2) ){
			lowerC(2) = vertices[i](2);
		} 
		if( vertices[i](2) > upperC(2) ){
			upperC(2) = vertices[i](2);
		}
	}
	 
	// add offset to get thr grid BB
	upperC += tmp;
	lowerC -= tmp;
}

void Mesh::calcSDF(){
	double    x_off, y_off, z_off, dist, minDist;
	int      ci, cj, ck, i, v0, v1, v2, closestV;
	int      closestT=0, closestE=0;
	D3Vector closestD, p, d;

	calcAdjacentTRIANGLE();
	calcUniqueEdges();

	calcPseudoVNormals() ;

	maxPhi = -DBL_MAX ;
	
	/* Calculate number of cell inb the grid */
	nb_x = (int)ceil( fabs( ( upperC(0) - lowerC(0) ) / d_x ) );
	nb_y = (int)ceil( fabs( ( upperC(1) - lowerC(1) ) / d_y ) );
	nb_z = (int)ceil( fabs( ( upperC(2) - lowerC(2) ) / d_z ) );

	// Calculate the offset from the bounding box and the grid
	x_off = ( nb_x*d_x + lowerC(0) ) - upperC(0);
	y_off = ( nb_y*d_y + lowerC(1) ) - upperC(1);
	z_off = ( nb_z*d_z + lowerC(2) ) - upperC(2);

	// Increase the bounding box to line up with the grid
	if( !zero( x_off ) ) { 
		lowerC(0) -= x_off/2.0f; 
		upperC(0) += x_off/2.0f; 
	}
	if( !zero( y_off ) ) { 
		lowerC(1) -= y_off/2.0f; 
		upperC(1) += y_off/2.0f; 
	}
	if( !zero( z_off ) ) { 
		lowerC(2) -= z_off/2.0f; 
		upperC(2) += z_off/2.0f; 
	}
	
	phiGrid.clear();
	closestTRIANGLEs.clear();

	/* init grid */
	closestTRIANGLEs.resize(nb_x+1);
	phiGrid.resize(nb_x+1);
	for( i=0; i<nb_x+1; ++i){
		closestTRIANGLEs[i].resize(nb_y+1);
		phiGrid[i].resize(nb_y+1);
		for( int j=0; j<nb_y+1; ++j){
			closestTRIANGLEs[i][j].resize(nb_z+1);
			phiGrid[i][j].resize(nb_z+1);
		}
	}
	cout << " taille de la grille " << phiGrid.size() << "x"<< phiGrid[0].size()<< "x" << phiGrid[0][0].size() << endl ;
	
	/* calculate grid value */
	for( ck = 0 ; ck < nb_z+1 ; ck++ ){
		for( cj = 0 ; cj < nb_y+1; cj++ ){
			for( ci = 0 ; ci < nb_x+1 ; ci++ ){
				int v =-1 , e =-1 ;
				minDist = DBL_MAX ; // init min dist to a maximum
				
				p(0) = ci*d_x + lowerC(0) ;
				p(1) = cj*d_y + lowerC(1) ;
				p(2) = ck*d_z + lowerC(2) ;
				
				// seek closest triangle
				for( i = 0 ; i<nb_triangles ; ++i ){
					v0 = triangles[i].v0 ;
					v1 = triangles[i].v1 ;
					v2 = triangles[i].v2 ;
					
					dist = calcDist( vertices[v0],
							 vertices[v1],
							 vertices[v2],
							 p , v , e , d );
					
					if( dist < minDist ) 
					{
						minDist = dist;
						closestV = v;
						closestE = e;
						closestD = d;
						closestT = i;
					}
				}
				
				// check sign
				if( minDist != 0.0f )
					if( calcPhiSign( closestT, closestV, closestE, closestD ) ) 
						minDist = -minDist;
				
				
				//cout << " Phi value at [" << ci << "][" << cj << "][" << ck << "] :" << minDist << endl ;
				phiGrid[ci][cj][ck] = minDist ;
				closestTRIANGLEs[ci][cj][ck] = closestT ;
								
				if( minDist > maxPhi )
					maxPhi = minDist ;
			}
		}
	}
}

void Mesh::calcNormals() {
	int i;
	int v0,v1,v2;
	D3Vector a,b,c;
	
	tnormals.clear();
	tnormals.resize(nb_triangles);

	faceOffset.clear();
	faceOffset.resize(nb_triangles);

	vnormals.clear();
	vnormals.resize(nb_vertices);
	
	// calculate normals
	for( i=0; i<nb_triangles; ++i){
		v0 = triangles[i].v0;
		v1 = triangles[i].v1;
		v2 = triangles[i].v2;
		
		a = vertices[v1] - vertices[v0] ;
		b = vertices[v2] - vertices[v0] ;
		c = cross_product( a, b );
		
		c.normalize();
		
		// add th normal to the face
		tnormals[i]=c;
		// add the normal to each vertice
		vnormals[v0]+=c;
		vnormals[v1]+=c;
		vnormals[v2]+=c;

		faceOffset[i]= - (c(0) * vertices[v0](0))
			- (c(1) * vertices[v0](1))
			- (c(2) * vertices[v0](2)) ;				
	}
	// normalize vertice normals 
	for( i=0 ; i<nb_vertices; ++i ){
		vnormals[i].normalize();
	}
}

void Mesh::calcAdjacentTRIANGLE(){
	int i,j ;
	TRIANGLE a, b;
	int match, aEdge, bEdge;

	adjacentTRIANGLEs.clear();
	adjacentTRIANGLEs.resize(nb_triangles);
	
	for( i = 0 ; i < nb_triangles ; ++i ){
		a = triangles[i] ;
		for( j = i+1 ; j< nb_triangles ; ++j ){
			match = 0 ;
			aEdge = bEdge = -1 ;
			b = triangles[j];
			
			// test sur le premier point 
			if( a.v0 == b.v0 ) match++;
			else if( a.v0 == b.v1 ) { bEdge += 1 ; match++; }
			else if( a.v0 == b.v2 ) { bEdge += 2 ; match++; }
				
			// test sur le second point
			if( a.v1 == b.v0 ) { aEdge += 1 ; match++; }
			else if( a.v1 == b.v1 ) { aEdge += 1 ; bEdge += 1 ; match++; }
			else if( a.v1 == b.v2 ) { aEdge += 1 ; bEdge += 2 ; match++; }

			// test sur le 3eme point
			if( match < 2 ) // on test que si on a pas deja trouver
			{
				if( a.v2 == b.v0 ) { aEdge += 2 ;match++; }
				else if( a.v2 == b.v1 ) { aEdge += 2 ; bEdge += 1 ; match++; }
				else if( a.v2 == b.v2 ) { aEdge += 2 ; bEdge += 2 ; match++; }
			}
				
			// si ca marche on s'en souvient
			if( match == 2 ){
				if( aEdge == 0 )
					adjacentTRIANGLEs[i].v0 = j ;
				else if ( aEdge == 1 )
					adjacentTRIANGLEs[i].v1 = j ;
				else
					adjacentTRIANGLEs[i].v2 = j ;
				
				if( bEdge == 0 )
					adjacentTRIANGLEs[j].v0 = i ;
				else if ( bEdge == 1 )
					adjacentTRIANGLEs[j].v1 = i ;
				else
					adjacentTRIANGLEs[j].v2 = i ;

			}
		}
	}
	
}

void Mesh::calcUniqueEdges() {
	vector<bool> unvisited ;
	int i, nbEdge;
	int e, ti ;
	
	unvisited.clear();
	unvisited.resize(nb_triangles * 3, true);
	edges.clear();
	
	nbEdge = nb_triangles * 3;
	for( i=0 ; i < nbEdge ; ++i ){
		if( unvisited[i] ){
			e = i%3 ; // index of the Edge in the triangle
			ti = i/3 ; // index of the triangle
			
			Edge ed;
			
			if( e == 0 ){
				ed.v0 = triangles[ti].v0 ;
				ed.v1 = triangles[ti].v1 ;
				unvisited[ adjacentTRIANGLEs[ti].v0 ] = false ;
			} else if ( e==1 ){
				ed.v0 = triangles[ti].v0 ;
				ed.v1 = triangles[ti].v2 ;
				unvisited[ adjacentTRIANGLEs[ti].v1 ] = false ;
			} else {
				ed.v0 = triangles[ti].v1 ;
				ed.v1 = triangles[ti].v2 ;
				unvisited[ adjacentTRIANGLEs[ti].v2 ] = false ;
			}
			
			edges.push_back( ed );
		}
	}
	
	unvisited.clear();
}

void Mesh::calcPseudoVNormals(){
	D3Vector n1, n2, n0, n;
	double    dotp;
	int      v0, v1, v2;
	int      i ;

	pseudoVNormals.clear();
	pseudoVNormals.resize( nb_vertices, D3Vector(0.,0.,0.) );
	

	for( i = 0 ; i < nb_triangles ; i++ ){
		v0 = triangles[ i ].v0 ;
		v1 = triangles[ i ].v1 ;
		v2 = triangles[ i ].v2 ;

		n0 = vertices[v0] - vertices[v1] ;
		n1 = vertices[v0] - vertices[v2] ;
		n2 = vertices[v1] - vertices[v2] ;

		n0.normalize() ; 
		n1.normalize() ; 
		n2.normalize() ; 

		n = tnormals[ i ] ;

		dotp = dot_product( n0, n1 );
		pseudoVNormals[ v0 ] += acos( dotp )*n;
		dotp = dot_product( -n0, n2 );
		pseudoVNormals[ v1 ] += acos( dotp )*n;
		dotp = dot_product( n1, n2 );
		pseudoVNormals[ v2 ] += acos( dotp )*n;
		
	}
	
	for( i=0 ; i < nb_vertices ; i++ )
		pseudoVNormals[i].normalize();
	
}


/********************************************************/
/* Volume Integration Stuff ( from Mirtich work & code  */
/********************************************************/
void Mesh::compProjectionIntegrals( int t ){
	double a0, a1, da;
	double b0, b1, db;
	double a0_2, a0_3, a0_4, b0_2, b0_3, b0_4;
	double a1_2, a1_3, b1_2, b1_3;
	double C1, Ca, Caa, Caaa, Cb, Cbb, Cbbb;
	double Cab, Kab, Caab, Kaab, Cabb, Kabb;

	P1 = Pa = Pb = Paa = Pab = Pbb = Paaa = Paab = Pabb = Pbbb = 0.0;

	/* vertice 1 */
	a0 = vertices[triangles[t].v0](A);
	b0 = vertices[triangles[t].v0](B);
	a1 = vertices[triangles[t].v1](A);
	b1 = vertices[triangles[t].v1](B);
	da = a1 - a0;
	db = b1 - b0;
	a0_2 = a0 * a0; a0_3 = a0_2 * a0; a0_4 = a0_3 * a0;
	b0_2 = b0 * b0; b0_3 = b0_2 * b0; b0_4 = b0_3 * b0;
	a1_2 = a1 * a1; a1_3 = a1_2 * a1; 
	b1_2 = b1 * b1; b1_3 = b1_2 * b1;
	
	C1 = a1 + a0;
	Ca = a1*C1 + a0_2; Caa = a1*Ca + a0_3; Caaa = a1*Caa + a0_4;
	Cb = b1*(b1 + b0) + b0_2; Cbb = b1*Cb + b0_3; Cbbb = b1*Cbb + b0_4;
	Cab = 3*a1_2 + 2*a1*a0 + a0_2; Kab = a1_2 + 2*a1*a0 + 3*a0_2;
	Caab = a0*Cab + 4*a1_3; Kaab = a1*Kab + 4*a0_3;
	Cabb = 4*b1_3 + 3*b1_2*b0 + 2*b1*b0_2 + b0_3;
	Kabb = b1_3 + 2*b1_2*b0 + 3*b1*b0_2 + 4*b0_3;
	
	P1 += db*C1;
	Pa += db*Ca;
	Paa += db*Caa;
	Paaa += db*Caaa;
	Pb += da*Cb;
	Pbb += da*Cbb;
	Pbbb += da*Cbbb;
	Pab += db*(b1*Cab + b0*Kab);
	Paab += db*(b1*Caab + b0*Kaab);
	Pabb += da*(a1*Cabb + a0*Kabb);
	
	/* vertice 2 */
	a0 = vertices[triangles[t].v1](A);
	b0 = vertices[triangles[t].v1](B);
	a1 = vertices[triangles[t].v2](A);
	b1 = vertices[triangles[t].v2](B);
	da = a1 - a0;
	db = b1 - b0;
	a0_2 = a0 * a0; a0_3 = a0_2 * a0; a0_4 = a0_3 * a0;
	b0_2 = b0 * b0; b0_3 = b0_2 * b0; b0_4 = b0_3 * b0;
	a1_2 = a1 * a1; a1_3 = a1_2 * a1; 
	b1_2 = b1 * b1; b1_3 = b1_2 * b1;
	
	C1 = a1 + a0;
	Ca = a1*C1 + a0_2; Caa = a1*Ca + a0_3; Caaa = a1*Caa + a0_4;
	Cb = b1*(b1 + b0) + b0_2; Cbb = b1*Cb + b0_3; Cbbb = b1*Cbb + b0_4;
	Cab = 3*a1_2 + 2*a1*a0 + a0_2; Kab = a1_2 + 2*a1*a0 + 3*a0_2;
	Caab = a0*Cab + 4*a1_3; Kaab = a1*Kab + 4*a0_3;
	Cabb = 4*b1_3 + 3*b1_2*b0 + 2*b1*b0_2 + b0_3;
	Kabb = b1_3 + 2*b1_2*b0 + 3*b1*b0_2 + 4*b0_3;
	
	P1 += db*C1;
	Pa += db*Ca;
	Paa += db*Caa;
	Paaa += db*Caaa;
	Pb += da*Cb;
	Pbb += da*Cbb;
	Pbbb += da*Cbbb;
	Pab += db*(b1*Cab + b0*Kab);
	Paab += db*(b1*Caab + b0*Kaab);
	Pabb += da*(a1*Cabb + a0*Kabb);

	/* vertice 3 */
	a0 = vertices[triangles[t].v2](A);
	b0 = vertices[triangles[t].v2](B);
	a1 = vertices[triangles[t].v0](A);
	b1 = vertices[triangles[t].v0](B);
	da = a1 - a0;
	db = b1 - b0;
	a0_2 = a0 * a0; a0_3 = a0_2 * a0; a0_4 = a0_3 * a0;
	b0_2 = b0 * b0; b0_3 = b0_2 * b0; b0_4 = b0_3 * b0;
	a1_2 = a1 * a1; a1_3 = a1_2 * a1; 
	b1_2 = b1 * b1; b1_3 = b1_2 * b1;
	
	C1 = a1 + a0;
	Ca = a1*C1 + a0_2; Caa = a1*Ca + a0_3; Caaa = a1*Caa + a0_4;
	Cb = b1*(b1 + b0) + b0_2; Cbb = b1*Cb + b0_3; Cbbb = b1*Cbb + b0_4;
	Cab = 3*a1_2 + 2*a1*a0 + a0_2; Kab = a1_2 + 2*a1*a0 + 3*a0_2;
	Caab = a0*Cab + 4*a1_3; Kaab = a1*Kab + 4*a0_3;
	Cabb = 4*b1_3 + 3*b1_2*b0 + 2*b1*b0_2 + b0_3;
	Kabb = b1_3 + 2*b1_2*b0 + 3*b1*b0_2 + 4*b0_3;
	
	P1 += db*C1;
	Pa += db*Ca;
	Paa += db*Caa;
	Paaa += db*Caaa;
	Pb += da*Cb;
	Pbb += da*Cbb;
	Pbbb += da*Cbbb;
	Pab += db*(b1*Cab + b0*Kab);
	Paab += db*(b1*Caab + b0*Kaab);
	Pabb += da*(a1*Cabb + a0*Kabb);


	P1 /= 2.0;
	Pa /= 6.0;
	Paa /= 12.0;
	Paaa /= 20.0;
	Pb /= -6.0;
	Pbb /= -12.0;
	Pbbb /= -20.0;
	Pab /= 24.0;
	Paab /= 60.0;
	Pabb /= -60.0;
}

void Mesh::compFaceIntegrals( int t ){
	D3Vector n ;
	double w ;
	double k1, k2, k3, k4;
	
	compProjectionIntegrals( t );
	
	w = faceOffset[t];
	n = tnormals[t];

	k1 = 1 / n(C); k2 = k1 * k1; k3 = k2 * k1; k4 = k3 * k1;
	
	Fa = k1 * Pa;
	Fb = k1 * Pb;
	Fc = -k2 * (n(A)*Pa + n(B)*Pb + w*P1);
	
	Faa = k1 * Paa;
	Fbb = k1 * Pbb;
	Fcc = k3 * (S2SQUARE(n(A))*Paa + 2*n(A)*n(B)*Pab + S2SQUARE(n(B))*Pbb
		    + w*(2*(n(A)*Pa + n(B)*Pb) + w*P1));
	
	Faaa = k1 * Paaa;
	Fbbb = k1 * Pbbb;
	Fccc = -k4 * (S2CUBE(n(A))*Paaa + 3*S2SQUARE(n(A))*n(B)*Paab 
		      + 3*n(A)*S2SQUARE(n(B))*Pabb + S2CUBE(n(B))*Pbbb
		      + 3*w*(S2SQUARE(n(A))*Paa + 2*n(A)*n(B)*Pab + S2SQUARE(n(B))*Pbb)
		      + w*w*(3*(n(A)*Pa + n(B)*Pb) + w*P1));
	
	Faab = k1 * Paab;
	Fbbc = -k2 * (n(A)*Pabb + n(B)*Pbbb + w*Pbb);
	Fcca = k3 * (S2SQUARE(n(A))*Paaa + 2*n(A)*n(B)*Paab + S2SQUARE(n(B))*Pabb
		     + w*(2*(n(A)*Paa + n(B)*Pab) + w*Pa));
}

void Mesh::compVolumeIntegrals() {
	double nx, ny, nz;
	int i;
	
	T0 = T1(0) = T1(1) = T1(2) 
		= T2(0) = T2(1) = T2(2) 
		= TP(0) = TP(1) = TP(2) = 0;
	
	for (i = 0; i < nb_triangles; i++) {
		
		nx = fabs(tnormals[i](0));
		ny = fabs(tnormals[i](1));
		nz = fabs(tnormals[i](2));
		
		if (nx > ny && nx > nz) C = 0;
		else C = (ny > nz) ? 1 : 2;
		A = (C + 1) % 3;
		B = (A + 1) % 3;
		
		compFaceIntegrals( i );
		
		T0 += tnormals[i](0) * ((A == 0) ? Fa : ((B == 0) ? Fb : Fc));
		
		T1(A) += tnormals[i](A) * Faa;
		T1(B) += tnormals[i](B) * Fbb;
		T1(C) += tnormals[i](C) * Fcc;
		T2(A) += tnormals[i](A) * Faaa;
		T2(B) += tnormals[i](B) * Fbbb;
		T2(C) += tnormals[i](C) * Fccc;
		TP(A) += tnormals[i](A) * Faab;
		TP(B) += tnormals[i](B) * Fbbc;
		TP(C) += tnormals[i](C) * Fcca;
	}
	
	T1(0) /= 2; T1(1) /= 2; T1(2) /= 2;
	T2(0) /= 3; T2(1) /= 3; T2(2) /= 3;
	TP(0) /= 2; TP(1) /= 2; TP(2) /= 2;
}

void Mesh::calcCOMAndIM(){
	compVolumeIntegrals();
	
	/* compute center of mass */
	center_of_mass(0) =  T1(0) / T0 ;
	center_of_mass(1) =  T1(1) / T0 ;
	center_of_mass(2) =  T1(2) / T0 ;

	/* assume density = 1 */
	//setDensity( 1.0 );
	
	setMass(fabs( density * T0 ));	

	/* compute inertia tensor */
	inertia_matrix(0,0) = density * (T2(1) + T2(2));
	inertia_matrix(1,1) = density * (T2(2) + T2(0));
	inertia_matrix(2,2) = density * (T2(0) + T2(1));
	inertia_matrix(0,1) = inertia_matrix(1,0) = - density * TP(0);
	inertia_matrix(1,2) = inertia_matrix(2,1) = - density * TP(1);
	inertia_matrix(2,0) = inertia_matrix(0,2) = - density * TP(2);
	
	/* translate inertia tensor to center of mass */
	/*
	  inertia_matrix(0,0) -= mass * (center_of_mass(1)*center_of_mass(1) + center_of_mass(2)*center_of_mass(2));
	  inertia_matrix(1,1) -= mass * (center_of_mass(2)*center_of_mass(2) + center_of_mass(0)*center_of_mass(0));
	  inertia_matrix(2,2) -= mass * (center_of_mass(0)*center_of_mass(0) + center_of_mass(1)*center_of_mass(1));
	  inertia_matrix(0,1) = inertia_matrix(1,0) += mass * center_of_mass(0) * center_of_mass(1); 
	  inertia_matrix(1,2) = inertia_matrix(2,1) += mass * center_of_mass(1) * center_of_mass(2); 
	  inertia_matrix(2,0) = inertia_matrix(0,2) += mass * center_of_mass(2) * center_of_mass(0); 
	*/
	/* and we're done :) */
}


/********************************************************/
/* Mesh Loading                                         */
/********************************************************/

void Mesh::loadFromFile( string& filename ){
	FILE      *mfile;
	char      objbuff[BUFFSIZE] ;
	int       val ;

	nb_vertices = 0;
	nb_triangles = 0;

	mfile = fopen( filename.c_str() , "r" ) ;
	if( mfile == NULL ) { fprintf(stderr,"MESH unable to read file"); exit(1);}
	
	/* read vertices and faces */
	while( true ){
		val = fscanf(mfile, "%s", objbuff );
		if( val == EOF ) break ;

		if( strcmp( objbuff , "v" ) == 0 ){
			// found a vertex
			D3Vector v ;
			float f1,f2,f3 ;
			fscanf(mfile," %f %f %f\n", &f1, &f2, &f3 );
			v(0) = f1 ; v(1) = f2 ; v(2) = f3 ;
			//printf("Vertex : %f %f %f\n",v.v0,v.v1,v.v2 );
			vertices.push_back( v );
			nb_vertices ++ ;
		} 
		else if ( strcmp( objbuff , "f" ) == 0 ) {
			// found face
			int t0, t1, t2;
			TRIANGLE t ;
			fscanf( mfile , "%d/%d %d/%d %d/%d\n", &t.v0 , &t0, &t.v1, &t1, &t.v2 , &t2 );
			//printf("TRIANGLE %d %d %d\n", t.v0, t.v1, t.v2 );
			t.v0 -= 1;
			t.v1 -= 1;
			t.v2 -= 1;
			
			triangles.push_back( t );
			nb_triangles ++ ;			
		}			
	}
	fclose( mfile );

	cout << " Vertices  : " << nb_vertices << endl;
	cout << " TRIANGLEs : " << nb_triangles << endl;

	calcNormals();
	calcBB();
}

/********************************************************/
/* Display Function                                     */
/********************************************************/

void Mesh::display(bool phi) {
	
	if( !phi ){
		
	
		int v0, v1, v2;
		
		GLfloat			mat_diffuse[] = {diffuse(0), 
							 diffuse(1), 
							 diffuse(2), 
							 1.0 };
		GLfloat			mat_specular[] = {specular(0), 
							  specular(1), 
							  specular(2), 
							  1.0};
		
		glColor4f( color(0), color(1), color(2), 1.0f );
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
		
		glBegin( GL_TRIANGLES );
		for( int i = 0 ; i < nb_triangles ; ++i ){
			v0 = triangles[i].v0;
			v1 = triangles[i].v1;
			v2 = triangles[i].v2;
			
			//glNormal3f( tnormals[i](0) , tnormals[i](1), tnormals[i](2) );
			
			glNormal3f( vnormals[v0](0) , vnormals[v0](1), vnormals[v0](2) );
			glVertex3f( vertices[v0](0) , vertices[v0](1), vertices[v0](2) );
			glNormal3f( vnormals[v1](0) , vnormals[v1](1), vnormals[v1](2) );
			glVertex3f( vertices[v1](0) , vertices[v1](1), vertices[v1](2) );
			glNormal3f( vnormals[v2](0) , vnormals[v2](1), vnormals[v2](2) );
			glVertex3f( vertices[v2](0) , vertices[v2](1), vertices[v2](2) );
		}
		glEnd();
	

	} else {
	
	

		int i, v0, v1 ;

		glColor4f( color(0), color(1), color(2), 1.0f );
		glBegin(GL_LINES);
		for( i = 0; i < (int)edges.size(); i++ )
		{
			v0 = edges[ i ].v0;
			v1 = edges[ i ].v1;
		
			glVertex3f(vertices[v1](0), vertices[v1](1), vertices[v1](2));
			glVertex3f(vertices[v0](0), vertices[v0](1), vertices[v0](2));
		
		}
		glEnd();


		int ci,cj,ck;
		glPointSize( 2.0f );
		glBegin( GL_POINTS );
		// Draw phi values at corners
		for( ck = 0; ck < nb_z+1; ck++ )
			for( cj = 0; cj < nb_y+1; cj++ )
				for( ci = 0; ci < nb_x+1; ci++ )
				{
					//Cell c ; c.ci = ci ; c.cj = cj ; c.ck = ck ; 
					//mapCellToCorner( c );
					//D3Vector p = lcCell( c );
					double val = phiGrid[ci][cj][ck] ;
					//double val = phiEval( p );
					if( negative( val )  ){
						glColor3f( 1.0f, 0.0f, 0.0f );
						glVertex3f( ci*d_x + lowerC(0), cj*d_y + lowerC(1), ck*d_z + lowerC(2) );
					}
					//if( positive( val ) ){
						//glColor3f( 0.0f, 1.0f, 0.0f );
						//glVertex3f( ci*d_x + lowerC(0), cj*d_y + lowerC(1), ck*d_z + lowerC(2) );
					//}
					if( zero( val ) ){
						glColor3f( 0.0f, 0.0f, 1.0f );
						glVertex3f( ci*d_x + lowerC(0), cj*d_y + lowerC(1), ck*d_z + lowerC(2) );
					}
					
					
					
					
				}
		glEnd();
	}
}
