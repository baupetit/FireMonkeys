#define SIZE ((N+2)*(N+2)*(N+2))

#include "solver.h"

#include <cmath>

inline void SWAP( float* &a , float* &b){
	float *tmp = a ;
	a = b ;
	b = tmp ;
}

Solver::Solver( int N ) : _N(N) {
	
	/* allocation */
	_d = new float[SIZE];
	_d0 = new float[SIZE];
	_u = new float[SIZE];
	_u0 = new float[SIZE];
	_v = new float[SIZE];
	_v0 = new float[SIZE];		
	_w = new float[SIZE];
	_w0 = new float[SIZE];		
	_srcd = new float[SIZE];
	_srcu = new float[SIZE];
	_srcv = new float[SIZE];
	_srcw = new float[SIZE];
	

	int i;

	for( i=0; i<SIZE ; i++ ) {
		_d[i] = 0.0f;
		_d0[i] = 0.0f;
		_u[i] = 0.0f;
		_u0[i] = 0.0f;
		_v[i] = 0.0f;
		_v0[i] = 0.0f;
		_w[i] = 0.0f;
		_w0[i] = 0.0f;
		_srcd[i] = 0.0f;
	}
}

Solver::~Solver(){
	delete [] _d;
	delete [] _d0;
	delete [] _u;
	delete [] _u0;
	delete [] _v;
	delete [] _v0;
	delete [] _w;
	delete [] _w0;
}

const float* Solver::getDensities() const {
	return _d;
}

const float* Solver::getVelocityU() const {
	return _u;
}
const float* Solver::getVelocityV() const {
	return _v;
}
const float* Solver::getVelocityW() const {
	return _w;
}


int Solver::getSize() const{
	return _N ;
}

void Solver::setDensity( int i , int j , int k , float dens ){
	int N = _N ;
	_srcd[IX(i,j,k)] = dens ;
}

void Solver::setVelocity( int i, int j , int k , float u, float v, float w ){
	int N = _N ;
	_srcu[IX(i,j,k)] = u ;
	_srcv[IX(i,j,k)] = v ;
	_srcw[IX(i,j,k)] = w ;
}

void addSource ( int N, float *x , float *s , float dt )
{
	int i;
	for ( i=0 ; i<SIZE ; i++ ) x[i] += dt*s[i];
}

void setBoundaries ( int N, int b, float *x )
{
    /*
	int i,j;

	for ( i=1 ; i<=N ; i++ ) {
		for( j=1 ; j<=N ; j++ ) {
			x[IX(0  ,i,j)] = b==1 ? -x[IX(1,i,j)] : x[IX(1,i,j)];
			x[IX(N+1,i,j)] = b==1 ? -x[IX(N,i,j)] : x[IX(N,i,j)];
			x[IX(i,0  ,j)] = b==2 ? -x[IX(i,1,j)] : x[IX(i,1,j)];
			x[IX(i,N+1,j)] = b==2 ? -x[IX(i,N,j)] : x[IX(i,N,j)];
			x[IX(i,j  ,0)] = b==3 ? -x[IX(i,j,1)] : x[IX(i,j,1)];
			x[IX(i,j  ,N+1)] = b==3 ? -x[IX(i,j,N)] : x[IX(i,j,N)];
		}
	}

	x[IX(0  ,0  ,0)] = (x[IX(1,0  ,0)]+x[IX(0  ,1,0)]+x[IX(0,0    ,1)])/3;
	x[IX(0  ,N+1,0)] = (x[IX(1,N+1,0)]+x[IX(0  ,N,0)]+x[IX(0,N+1  ,1)])/3;
	x[IX(N+1,0  ,0)] = (x[IX(N,0  ,0)]+x[IX(N+1,1,0)]+x[IX(N+1,0  ,1)])/3;
	x[IX(N+1,N+1,0)] = (x[IX(N,N+1,0)]+x[IX(N+1,N,0)]+x[IX(N+1,N+1,1)])/3;
	x[IX(0  ,0  ,N+1)] = (x[IX(1,0  ,N+1)]+x[IX(0  ,1,N+1)]+x[IX(0,0    ,N)])/3;
	x[IX(0  ,N+1,N+1)] = (x[IX(1,N+1,N+1)]+x[IX(0  ,N,N+1)]+x[IX(0,N+1  ,1)])/3;
	x[IX(N+1,0  ,N+1)] = (x[IX(N,0  ,N+1)]+x[IX(N+1,1,N+1)]+x[IX(N+1,0  ,N)])/3;
	x[IX(N+1,N+1,N+1)] = (x[IX(N,N+1,N+1)]+x[IX(N+1,N,N+1)]+x[IX(N+1,N+1,N)])/3;
	
	*/

}

void linearSolve ( int N, int b, float * x, float * x0, float a, float c )
{
	int i, j, k, l;

	for ( l=0 ; l<20 ; l++ ) {
		for( k=1; k<N+1; ++k ){ 
			for( j = 1; j<N+1 ; ++j) { 
				for( i=1 ; i<N+1 ; ++i ){
					x[IX(i,j,k)] = ( x0[IX(i,j,k)] + a*( x[IX(i-1,j,k)] +
									     x[IX(i+1,j,k)] +
									     x[IX(i,j-1,k)] +
									     x[IX(i,j+1,k)] +
									     x[IX(i,j,k-1)] +
									     x[IX(i,j,k+1)] ))/c;
				}
			}
		}
		setBoundaries ( N, b, x );
	}
}

void diffuse ( int N, int b, float * x, float * x0, float diff, float dt )
{
	float a=dt*diff*N*N;
	linearSolve ( N, b, x, x0, a, 1+6*a );
}

void advect ( int N, int b, float * d, float * d0, float * u, float * v, float *w, float dt )
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	float x, y, z, s0, t0, r0, s1, t1, r1, dt0;

	dt0 = dt*N;
	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				x = i-dt0*u[IX(i,j,k)]; 
				y = j-dt0*v[IX(i,j,k)];
				z = k-dt0*w[IX(i,j,k)];

				if (x<0.5f) x=0.5f; 
				if (x>N+0.5f) x=N+0.5f; 
				i0=(int)x; 
				i1=i0+1;
				
				if (y<0.5f) y=0.5f; 
				if (y>N+0.5f) y=N+0.5f; 
				j0=(int)y; 
				j1=j0+1;
				
				if (z<0.5f) z=0.5f; 
				if (z>N+0.5f) z=N+0.5f; 
				k0=(int)z; 
				k1=k0+1;
				
				
				s1 = (x-i0); s0 = 1-s1; 
				t1 = (y-j0); t0 = 1-t1; 
				r1 = (z-k0); r0 = 1-r1;
				
				d[IX(i,j,k)] = (s0*(t0*(r0*d0[IX(i0,j0,k0)] + r1*d0[IX(i0,j0,k1)]) + 
						    t1*(r0*d0[IX(i0,j1,k0)] + r1*d0[IX(i0,j1,k1)]))
						+
						s1*(t0*(r0*d0[IX(i1,j0,k0)] + r1*d0[IX(i1,j0,k1)]) + 
						    t1*(r0*d0[IX(i1,j1,k0)] + r1*d0[IX(i1,j1,k1)])));
			}
		}
	}
	setBoundaries ( N, b, d );
}

void project ( int N, float * u, float * v, float *w, float * p, float * div )
{
	int i, j, k;

	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				div[IX(i,j,k)] = -(u[IX(i+1,j,k)]-u[IX(i-1,j,k)]+
						   v[IX(i,j+1,k)]-v[IX(i,j-1,k)]+
						   w[IX(i,j,k+1)]-w[IX(i,j,k-1)] )/(3*N);
				p[IX(i,j,k)] = 0;
			}
		}
	}	
	setBoundaries ( N, 0, div ); setBoundaries ( N, 0, p );

	linearSolve ( N, 0, p, div, 1, 6 );

	 for( k=1; k<N+1; ++k ){ 
		 for( j = 1; j<N+1 ; ++j) { 
			 for( i=1 ; i<N+1 ; ++i ){
				 u[IX(i,j,k)] -= N*(p[IX(i+1,j,k)]-p[IX(i-1,j,k)])/3;
				 v[IX(i,j,k)] -= N*(p[IX(i,j+1,k)]-p[IX(i,j-1,k)])/3;
				 w[IX(i,j,k)] -= N*(p[IX(i,j,k+1)]-p[IX(i,j,k-1)])/3;
			 }
		 }
	 }
	setBoundaries ( N, 1, u ); 
	setBoundaries ( N, 2, v ); 
	setBoundaries( N, 3, w);
}

void Solver::densitiesStep ( float diff, float dt )
{	
	addSource ( _N, _d, _srcd, dt );
	SWAP ( _d0, _d ); diffuse ( _N, 0, _d, _d0, diff, dt );
	SWAP ( _d0, _d ); advect ( _N, 0, _d, _d0, _u, _v, _w, dt );
}

void Solver::velocitiesStep ( float visc, float dt )
{
	addSource ( _N, _u, _srcu, dt ); 
	addSource ( _N, _v, _srcv, dt );
	addSource ( _N, _w, _srcw, dt );
	SWAP ( _u0, _u ); diffuse ( _N, 1, _u, _u0, visc, dt );
	SWAP ( _v0, _v ); diffuse ( _N, 2, _v, _v0, visc, dt );
	SWAP ( _w0, _w ); diffuse ( _N, 3, _w, _w0, visc, dt );
	project ( _N, _u, _v, _w, _u0, _v0 );
	SWAP ( _u0, _u ); 
	SWAP ( _v0, _v );
	SWAP ( _w0, _w );
	advect ( _N, 1, _u, _u0, _u0, _v0, _w0, dt ); 
	advect ( _N, 2, _v, _v0, _u0, _v0, _w0, dt );
	advect ( _N, 3, _w, _w0, _u0, _v0, _w0, dt );
	project ( _N, _u, _v, _w, _u0, _v0 );
}

