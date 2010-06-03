#define SIZE ((N+2)*(N+2)*(N+2))
#ifndef NB_ITERATION_SOLVE
#define NB_ITERATION_SOLVE 20
#endif

#include "solver.h"
#include "SolverParam.h"
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
	_f = new float[SIZE];
	_f0 = new float[SIZE];
	_T = new float[SIZE];
	_T0 = new float[SIZE];
	_u = new float[SIZE];
	_u0 = new float[SIZE];
	_v = new float[SIZE];
	_v0 = new float[SIZE];		
	_w = new float[SIZE];
	_w0 = new float[SIZE];		
	_srcd = new float[SIZE];
	_srcT = new float[SIZE];
	_srcu = new float[SIZE];
	_srcv = new float[SIZE];
	_srcw = new float[SIZE];
	
	int i;
	for( i=0; i<SIZE ; i++ ) {
		_d[i]=0.0f;_d0[i]=0.0f;_f[i]=0.0f;_f0[i]=0.0f;
		_T[i]=0.0f;_T0[i]=0.0f;
		_u[i]=0.0f;_u0[i]=0.0f;_v[i]=0.0f;_v0[i]=0.0f;
		_w[i]=0.0f;_w0[i]=0.0f;_srcd[i]=0.0f;_srcT[i]=0.0f;
		_srcu[i]=0.0f;_srcv[i]=0.0f;_srcw[i]=0.0f;
	}

	SolverParam::initSolverParam() ;
}

Solver::~Solver(){
	delete [] _d;
	delete [] _d0;
	delete [] _f;
	delete [] _f0;
	delete [] _T;
	delete [] _T0;
	delete [] _u;
	delete [] _u0;
	delete [] _v;
	delete [] _v0;
	delete [] _w;
	delete [] _w0;
	delete [] _srcd;
	delete [] _srcu;
	delete [] _srcv;
	delete [] _srcw;
	delete [] _srcT;
}

const float* Solver::getDensities() const {
	return _d;
}

const float* Solver::getSmokes() const {
	return _f;
}

const float* Solver::getTemperatures() const {
	return _T;
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

void Solver::setTemperature( int i, int j, int k, float temp ){
	int N = _N ;
	_srcT[IX(i,j,k)] = temp ;
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

void addSourceCorrection ( int N, float *x , float *f, float *T, float *s , float sub, float fireToSmoke, float dt )
{
	int i;
	for ( i=0 ; i<SIZE ; i++ ){
		x[i] += dt*s[i];
	}
	
}


void addBuoyancy( int N, float *T, float *v, float buoy, float dt)
{
	int i;

	for (i=0; i<SIZE; i++)
		v[i] += T[i]*buoy*dt;
}

void setBoundaries ( int N, int b, float *x )
{
#ifdef _BOUNDARIES_SOLVE_
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
#endif
}

void setBoundariesB0 ( int N, float *x )
{
#ifdef _BOUNDARIES_SOLVE_
	int i,j;
	for ( i=1 ; i<=N ; i++ ) {
		for( j=1 ; j<=N ; j++ ) {
			x[IX(0  ,i,j)] = x[IX(1,i,j)];
			x[IX(N+1,i,j)] = x[IX(N,i,j)];
			x[IX(i,0  ,j)] = x[IX(i,1,j)];
			x[IX(i,N+1,j)] = x[IX(i,N,j)];
			x[IX(i,j  ,0)] = x[IX(i,j,1)];
			x[IX(i,j  ,N+1)] = x[IX(i,j,N)];
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
#endif
}

void linearSolve ( int N, int b, float * x, float * x0, float a, float c )
{
	int i, j, k, l;

	for ( l=0 ; l<NB_ITERATION_SOLVE ; l++ ) {
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

void diffuseFireAndSmoke ( int N, int b1, int b2, int b3,
			   float * x, float * x0, 
			   float * f, float * f0, 
			   float * t, float * t0, 
			   float diffFire, float diffSmoke, float diffTemp, 
			   float dt )
{
	float a1=dt*diffFire*N*N;
	float a2=dt*diffSmoke*N*N;
	float a3=dt*diffTemp*N*N;
	float c1=1+6*a1;
	float c2=1+6*a2;
	float c3=1+6*a3;
	
	//linearSolve ( N, b, x, x0, a, 1+6*a );
	int i, j, k, l, ijk;
	
	for ( l=0 ; l<NB_ITERATION_SOLVE ; l++ ) {
		for( k=1; k<N+1; ++k ){ 
			for( j = 1; j<N+1 ; ++j) { 
				for( i=1 ; i<N+1 ; ++i ){
					ijk = IX(i,j,k);
					// Fire
					x[ijk] = ( x0[ijk] + a1*( x[IX(i-1,j,k)] +
								  x[IX(i+1,j,k)] +
								  x[IX(i,j-1,k)] +
								  x[IX(i,j+1,k)] +
								  x[IX(i,j,k-1)] +
								  x[IX(i,j,k+1)] ))/c1;
					// Smoke
					f[ijk] = ( f0[ijk] + a2*( f[IX(i-1,j,k)] +
								  f[IX(i+1,j,k)] +
								  f[IX(i,j-1,k)] +
								  f[IX(i,j+1,k)] +
								  f[IX(i,j,k-1)] +
								  f[IX(i,j,k+1)] ))/c2;
					// Temp
					t[ijk] = ( t0[ijk] + a3*( t[IX(i-1,j,k)] +
								  t[IX(i+1,j,k)] +
								  t[IX(i,j-1,k)] +
								  t[IX(i,j+1,k)] +
								  t[IX(i,j,k-1)] +
								  t[IX(i,j,k+1)] ))/c3;
					
				}
			}
		}
		setBoundariesB0 ( N, x );
		setBoundariesB0 ( N, f );
		setBoundariesB0 ( N, t );
	}
}

void advect ( int N, int b, float * d, float * d0, float * u, float * v, float *w, float dt )
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	float x, y, z, s0, t0, r0, s1, t1, r1, dt0;
	float rhs, lhs ;

	dt0 = dt*N;
	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				x = i-dt0*u[IX(i,j,k)] ; y = j-dt0*v[IX(i,j,k)];	z = k-dt0*w[IX(i,j,k)];

				if (x<0.5f) x=0.5f; if (x>N+0.5f) x=N+0.5f; i0=(int)x; i1=i0+1;			
				if (y<0.5f) y=0.5f; if (y>N+0.5f) y=N+0.5f; j0=(int)y; j1=j0+1;				
				if (z<0.5f) z=0.5f; if (z>N+0.5f) z=N+0.5f; k0=(int)z; k1=k0+1;
				
				s1 = (x-i0); s0 = 1-s1; t1 = (y-j0); t0 = 1-t1; r1 = (z-k0); r0 = 1-r1;
				
				rhs = (t0*(r0*d0[IX(i0,j0,k0)] + r1*d0[IX(i0,j0,k1)]) + 
				       t1*(r0*d0[IX(i0,j1,k0)] + r1*d0[IX(i0,j1,k1)]));
				lhs = (t0*(r0*d0[IX(i1,j0,k0)] + r1*d0[IX(i1,j0,k1)]) + 
				       t1*(r0*d0[IX(i1,j1,k0)] + r1*d0[IX(i1,j1,k1)]));

				d[IX(i,j,k)] = (s0*rhs + s1*lhs );
			}
		}
	}
	setBoundaries ( N, b, d );
} 

void advect_cool ( int N, int b, 
		   float * d, float * d0, float *f, float *f0, 
		   float *T, float *T0,
		   float * u, float * v, float *w, 
		   float cool, float consume, float taux_conversion_fire_to_smoke,
		   float dt )
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	float x, y, z, s0, t0, r0, s1, t1, r1, dt0;
	float rhs, lhs, c0 ,diff_fire2smoke;

	dt0 = dt*N;
	c0 = 1.0f - cool*dt;
	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				x = i-dt0*u[IX(i,j,k)]; y = j-dt0*v[IX(i,j,k)];	z = k-dt0*w[IX(i,j,k)];

				if (x<0.5f) x=0.5f; if (x>N+0.5f) x=N+0.5f; i0=(int)x; i1=i0+1;			
				if (y<0.5f) y=0.5f; if (y>N+0.5f) y=N+0.5f; j0=(int)y; j1=j0+1;				
				if (z<0.5f) z=0.5f; if (z>N+0.5f) z=N+0.5f; k0=(int)z; k1=k0+1;
				
				s1 = (x-i0); s0 = 1-s1; t1 = (y-j0); t0 = 1-t1; r1 = (z-k0); r0 = 1-r1;
				
				rhs = (t0*(r0*d0[IX(i0,j0,k0)] + r1*d0[IX(i0,j0,k1)]) + 
				       t1*(r0*d0[IX(i0,j1,k0)] + r1*d0[IX(i0,j1,k1)]));
				lhs = (t0*(r0*d0[IX(i1,j0,k0)] + r1*d0[IX(i1,j0,k1)]) + 
				       t1*(r0*d0[IX(i1,j1,k0)] + r1*d0[IX(i1,j1,k1)]));

				
				rhs = (s0*rhs + s1*lhs) - consume / (10*T[IX(i,j,k)]) ;
				d[IX(i,j,k)] = ( rhs < 0 ) ? 0 : rhs ;
				diff_fire2smoke = d[IX(i,j,k)] - d0[IX(i,j,k)];
				
				
				rhs = (t0*(r0*f0[IX(i0,j0,k0)] + r1*f0[IX(i0,j0,k1)]) + 
				       t1*(r0*f0[IX(i0,j1,k0)] + r1*f0[IX(i0,j1,k1)]));
				lhs = (t0*(r0*f0[IX(i1,j0,k0)] + r1*f0[IX(i1,j0,k1)]) + 
				       t1*(r0*f0[IX(i1,j1,k0)] + r1*f0[IX(i1,j1,k1)]));

				f[IX(i,j,k)] = (diff_fire2smoke < 0 ) ? (s0*rhs + s1*lhs ) - diff_fire2smoke/taux_conversion_fire_to_smoke : (s0*rhs + s1*lhs );

				
				rhs = (t0*(r0*T0[IX(i0,j0,k0)] + r1*T0[IX(i0,j0,k1)]) + 
				       t1*(r0*T0[IX(i0,j1,k0)] + r1*T0[IX(i0,j1,k1)]));
				lhs = (t0*(r0*T0[IX(i1,j0,k0)] + r1*T0[IX(i1,j0,k1)]) + 
				       t1*(r0*T0[IX(i1,j1,k0)] + r1*T0[IX(i1,j1,k1)]));

				T[IX(i,j,k)] = (s0*rhs + s1*lhs )*c0 ;
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

void vorticity_confinement( int N, float *u, float *v, float *w, 
			    float *u0, float *v0, float *w0, float *T0, 
			    float vc_eps, float dt)
{
	int i,j,k,ijk;
	float *curlx = u0, *curly = v0, *curlz=w0, *curl=T0;		// temp buffers
	float dt0 = dt * vc_eps;
	float x,y,z;


	for (k=1; k<N+1; k++) {
		for (j=1; j<N+1; j++) {
			for (i=1; i<N+1; i++) {
				ijk = IX(i,j,k);
					// curlx = dw/dy - dv/dz
				x = curlx[ijk] = (w[IX(i,j+1,k)] - w[IX(i,j-1,k)]) * 0.5f -
					(v[IX(i,j,k+1)] - v[IX(i,j,k-1)]) * 0.5f;

					// curly = du/dz - dw/dx
				y = curly[ijk] = (u[IX(i,j,k+1)] - u[IX(i,j,k-1)]) * 0.5f -
					(w[IX(i+1,j,k)] - w[IX(i-1,j,k)]) * 0.5f;

					// curlz = dv/dx - du/dy
				z = curlz[ijk] = (v[IX(i+1,j,k)] - v[IX(i-1,j,k)]) * 0.5f -
					(u[IX(i,j+1,k)] - u[IX(i,j-1,k)]) * 0.5f;

					// curl = |curl|
				curl[ijk] = sqrtf(x*x+y*y+z*z);
			}
		}
	}

	for (k=1; k<N+1; k++) {
		for (j=1; j<N+1; j++) {
			for (i=1; i<N+1; i++) {
				ijk = IX(i,j,k);
				float Nx = (curl[IX(i+1,j,k)] - curl[IX(i-1,j,k)]) * 0.5f;
				float Ny = (curl[IX(i,j+1,k)] - curl[IX(i,j-1,k)]) * 0.5f;
				float Nz = (curl[IX(i,j,k+1)] - curl[IX(i,j,k-1)]) * 0.5f;
				float len1 = 1.0f/(sqrtf(Nx*Nx+Ny*Ny+Nz*Nz)+0.0000001f);
				Nx *= len1;
				Ny *= len1;
				Nz *= len1;
				u[ijk] += (Ny*curlz[ijk] - Nz*curly[ijk]) * dt0;
				v[ijk] += (Nz*curlx[ijk] - Nx*curlz[ijk]) * dt0;
				w[ijk] += (Nx*curly[ijk] - Ny*curlx[ijk]) * dt0;
			}
		}
	}
}

void Solver::densitiesStep ( float dt )
{	
	addSource ( _N, _d, _srcd, dt );
	SWAP ( _d0, _d ); diffuse ( _N, 0, _d, _d0, SolverParam::getDiffusionParamFire() , dt );
	SWAP ( _d0, _d ); advect ( _N, 0, _d, _d0, _u, _v, _w, dt );
}

void Solver::densitiesStepWithTemp ( float dt )
{	
	addSource ( _N, _d, _srcd, dt );
	addSource ( _N, _T, _srcT, dt );
	SWAP ( _d0, _d );SWAP ( _f0, _f );SWAP ( _T0, _T );
	diffuseFireAndSmoke(_N, 0, 0, 0,
			    _d, _d0, 
			    _f, _f0, 
			    _T, _T0, 
			    SolverParam::getDiffusionParamFire(), 
			    SolverParam::getDiffusionParamSmoke(), 
			    SolverParam::getDiffusionParamTemperature(), 
			    dt );
	SWAP ( _d0, _d ); SWAP ( _f0, _f ); SWAP ( _T0, _T ); 
	advect_cool ( _N, 0, 
		      _d, _d0, 
		      _f, _f0, 
		      _T, _T0, 
		      _u, _v, _w, 
		      SolverParam::getCoolingParam(), 
		      SolverParam::getConsumingParam(),
		      SolverParam::getFireToSmokeParam(), 
		      dt );
}


void Solver::velocitiesStep ( float dt )
{
	// Adding sources
	addSource ( _N, _u, _srcu, dt ); 
	addSource ( _N, _v, _srcv, dt ); 
	addSource ( _N, _w, _srcw, dt );

	// speed diffusig due to viscosity
	SWAP ( _u0, _u ); diffuse ( _N, 1, _u, _u0, SolverParam::getViscosityParam(), dt );
	SWAP ( _v0, _v ); diffuse ( _N, 2, _v, _v0, SolverParam::getViscosityParam(), dt );
	SWAP ( _w0, _w ); diffuse ( _N, 3, _w, _w0, SolverParam::getViscosityParam(), dt );
	project ( _N, _u, _v, _w, _u0, _v0 );
	
	// speed advection
	SWAP ( _u0, _u ); SWAP ( _v0, _v ); SWAP ( _w0, _w );
	advect ( _N, 1, _u, _u0, _u0, _v0, _w0, dt ); 
	advect ( _N, 2, _v, _v0, _u0, _v0, _w0, dt );
	advect ( _N, 3, _w, _w0, _u0, _v0, _w0, dt );
	project ( _N, _u, _v, _w, _u0, _v0 );
}


void Solver::velocitiesStepWithTemp ( float dt )
{
	// Adding sources
	addSource ( _N, _u, _srcu, dt ); 
	addSource ( _N, _v, _srcv, dt ); 
	addSource ( _N, _w, _srcw, dt );
	addBuoyancy( _N, _T, _v, SolverParam::getBuoyancyParam(), dt);
	vorticity_confinement( _N, _u, _v, _w, 
			       _u0, _v0, _w0, _T0, 
			       SolverParam::getVorticityConfinementParam(), dt);


	// speed diffusig due to viscosity
	SWAP ( _u0, _u ); diffuse ( _N, 1, _u, _u0, SolverParam::getViscosityParam(), dt );
	SWAP ( _v0, _v ); diffuse ( _N, 2, _v, _v0, SolverParam::getViscosityParam(), dt );
	SWAP ( _w0, _w ); diffuse ( _N, 3, _w, _w0, SolverParam::getViscosityParam(), dt );
	project ( _N, _u, _v, _w, _u0, _v0 );
	
	// speed advection
	SWAP ( _u0, _u ); SWAP ( _v0, _v ); SWAP ( _w0, _w );
	advect ( _N, 1, _u, _u0, _u0, _v0, _w0, dt ); 
	advect ( _N, 2, _v, _v0, _u0, _v0, _w0, dt );
	advect ( _N, 3, _w, _w0, _u0, _v0, _w0, dt );
	project ( _N, _u, _v, _w, _u0, _v0 );
}

