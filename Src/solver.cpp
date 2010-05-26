#define SIZE ((N+2)*(N+2)*(N+2))
#define IX(i,j,k) ((i)+(N+2)*(j)+(N+2*N+2)*k)
#define SWAP(x0,x) {double * tmp=x0;x0=x;x=tmp;}
#define FOR_EACH_CELL for ( k=1 ; k<=N ; k++ ) { for ( j=1 ; j<=N ; j++ ) { for( i=1; i<N ; i++ ) {
#define END_FOR }}}

#include "solver.h"


Solver::Solver( int N ) : _N(N) {
	
	/* allocation */
	_d = new double[SIZE];
	_d0 = new double[SIZE];
	_u = new double[SIZE];
	_u0 = new double[SIZE];
	_v = new double[SIZE];
	_v0 = new double[SIZE];		
	_w = new double[SIZE];
	_w0 = new double[SIZE];		

	int i,j,k;

	FOR_EACH_CELL
		_d[IX(i,j,k)] = 0.0f;
        	_d0[IX(i,j,k)] = 0.0f;
	        _u[IX(i,j,k)] = 0.0f;
	        _u0[IX(i,j,k)] = 0.0f;
	        _v[IX(i,j,k)] = 0.0f;
	        _v0[IX(i,j,k)] = 0.0f;
	        _w[IX(i,j,k)] = 0.0f;
	        _w0[IX(i,j,k)] = 0.0f;
        END_FOR
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

void addSource ( int N, double *x , double *s , double dt )
{
	int i;
	for ( i=0 ; i<SIZE ; i++ ) x[i] += dt*s[i];
}

void setBoundaries ( int N, int b, double *x )
{
/*	
	int i;

	for ( i=1 ; i<=N ; i++ ) {
		x[IX(0  ,i)] = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
		x[IX(N+1,i)] = b==1 ? -x[IX(N,i)] : x[IX(N,i)];
		x[IX(i,0  )] = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
		x[IX(i,N+1)] = b==2 ? -x[IX(i,N)] : x[IX(i,N)];
	}
	x[IX(0  ,0  )] = 0.5f*(x[IX(1,0  )]+x[IX(0  ,1)]);
	x[IX(0  ,N+1)] = 0.5f*(x[IX(1,N+1)]+x[IX(0  ,N)]);
	x[IX(N+1,0  )] = 0.5f*(x[IX(N,0  )]+x[IX(N+1,1)]);
	x[IX(N+1,N+1)] = 0.5f*(x[IX(N,N+1)]+x[IX(N+1,N)]);
*/
}

void linearSolve ( int N, int b, double * x, double * x0, double a, double c )
{
	int i, j, k, l;

	for ( l=0 ; l<20 ; l++ ) {
		FOR_EACH_CELL
			x[IX(i,j,k)] = ( x0[IX(i,j,k)] + a*( x[IX(i-1,j,k)] +
							     x[IX(i+1,j,k)] +
							     x[IX(i,j-1,k)] +
							     x[IX(i,j+1,k)] +
							     x[IX(i,j,k-1)] +
							     x[IX(i,j,k+1)] ))/c;
		END_FOR
		setBoundaries ( N, b, x );
	}
}

void diffuse ( int N, int b, double * x, double * x0, double diff, double dt )
{
	double a=dt*diff*N*N*N;
	linearSolve ( N, b, x, x0, a, 1+4*a );
}

void advect ( int N, int b, double * d, double * d0, double * u, double * v, double *w, double dt )
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	double x, y, z, s0, t0, r0, s1, t1, r1, dt0;

	dt0 = dt*N;
	FOR_EACH_CELL
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
		k1=j0+1;


	        s1 = x-i0; s0 = 1-s1; 
		t1 = y-j0; t0 = 1-t1; 
		r1 = z-k0; r0 = 1-r1;

	        d[IX(i,j,k)] = s0*(t0*(r0*d0[IX(i0,j0,k0)] + r1*d0[IX(i0,j0,k1)]) + 
				 t1*(r0*d0[IX(i0,j1,k0)] + r1*d0[IX(i0,j1,k1)]))
			+
			     s1*(t0*(r0*d0[IX(i1,j0,k0)] + r1*d0[IX(i1,j0,k1)]) + 
				 t1*(r0*d0[IX(i1,j1,k0)] + r1*d0[IX(i1,j1,k1)]));
	END_FOR
	setBoundaries ( N, b, d );
}

void project ( int N, double * u, double * v, double *w, double * p, double * div )
{
	int i, j, k;

	FOR_EACH_CELL
		div[IX(i,j,k)] = -0.5f*(
			u[IX(i+1,j,k)]-u[IX(i-1,j,k)]+
			v[IX(i,j+1,k)]-v[IX(i,j-1,k)]+
			w[IX(i,j,k+1)]-v[IX(i,j,k-1)] )/N;
	        p[IX(i,j,k)] = 0;
	END_FOR	
	setBoundaries ( N, 0, div ); setBoundaries ( N, 0, p );

	linearSolve ( N, 0, p, div, 1, 4 );

	FOR_EACH_CELL
		u[IX(i,j,k)] -= 0.5f*N*(p[IX(i+1,j,k)]-p[IX(i-1,j,k)]);
	        v[IX(i,j,k)] -= 0.5f*N*(p[IX(i,j+1,k)]-p[IX(i,j-1,k)]);
		w[IX(i,j,k)] -= 0.5f*N*(p[IX(i,j,k+1)]-p[IX(i,j,k-1)]);
	END_FOR
	setBoundaries ( N, 1, u ); 
	setBoundaries ( N, 2, v ); 
	setBoundaries( N, 3, w);
}

void Solver::densitiesStep ( double diff, double dt )
{
	addSource ( _N, _d, _d0, dt );
	SWAP ( _d0, _d ); diffuse ( _N, 0, _d, _d0, diff, dt );
	SWAP ( _d0, _d ); advect ( _N, 0, _d, _d0, _u, _v, _w, dt );
}

void Solver::velocitiesStep ( double visc, double dt )
{
	addSource ( _N, _u, _u0, dt ); 
	addSource ( _N, _v, _v0, dt );
	addSource ( _N, _w, _w0, dt );
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

