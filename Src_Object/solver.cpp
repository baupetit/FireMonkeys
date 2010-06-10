#define SIZE ((N+2)*(N+2)*(N+2))
#ifndef NB_ITERATION_SOLVE
#define NB_ITERATION_SOLVE 10
#endif

#include "solver.h"
#include "SolverParam.h"
#include <cmath>

//#define MAX(a,b) (((a)>(b))?(a):(b))

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
	
	_filled = new Voxel[SIZE];

	int i;
	for( i=0; i<SIZE ; i++ ) {
		_d[i]=0.0f;_d0[i]=0.0f;_f[i]=0.0f;_f0[i]=0.0f;
		_T[i]=0.0f;_T0[i]=0.0f;
		_u[i]=0.0f;_u0[i]=0.0f;_v[i]=0.0f;_v0[i]=0.0f;
		_w[i]=0.0f;_w0[i]=0.0f;_srcd[i]=0.0f;_srcT[i]=0.0f;
		_srcu[i]=0.0f;_srcv[i]=0.0f;_srcw[i]=0.0f;
		//_filled[i] = Voxel();
	}

	float spaceDiv = SolverParam::getSpaceDiv();
	AABB = BoundingBox(Vecteur3D( -_N*spaceDiv/2, -_N*spaceDiv/2, -_N*spaceDiv/2 ) ,
			   Vecteur3D(  _N*spaceDiv/2,  _N*spaceDiv/2,  _N*spaceDiv/2 ) );

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
	
	delete [] _filled;
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
	_srcd[IX(i,j,k)] = dens ;
}

void Solver::setTemperature( int i, int j, int k, float temp ){
	_srcT[IX(i,j,k)] = temp ;
}

void Solver::setVelocity( int i, int j , int k , float u, float v, float w ){
	int N = _N ;
	_srcu[IX(i,j,k)] = u ;
	_srcv[IX(i,j,k)] = v ;
	_srcw[IX(i,j,k)] = w ;
}

void Solver::addSource ( int N, float *x , float *s , float dt )
{
	for( int i = 0 ; i < SIZE ; ++i ) 
		x[i] += dt*s[i];
}


void Solver::addSource3 ( int N, 
                         float *x1 , float *s1 ,
                         float *x2 , float *s2 ,
                         float *x3 , float *s3 ,
                         float dt )
{
	for( int i = 0 ; i < SIZE ; ++i ) 
	{
		x1[i] += dt*s1[i];
		x2[i] += dt*s2[i];
		x3[i] += dt*s3[i];
	}	
}

void Solver::addBuoyancy( int N, float *T, float *v, float buoy, float dt)
{
	for( int i = 0; i < SIZE ; i++ ) v[i] += T[i]*buoy*dt;	
}

void Solver::combustion( int N , float *d, float *f, float *T, 
			 float consume, float fireToSmoke, float dt )
{
	int i,j,k,ijk;
	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				ijk = IX(i,j,k);
				//T[ijk] += fmax(0, d[ijk] - consume*dt )*consume/fireToSmoke;

				float tmp = d[ijk];
				d[ijk] = fmax(0, d[ijk] - consume*dt );
				f[ijk] += (tmp-d[ijk])*fireToSmoke;
			}
		}
	}
}

void Solver::setBoundaries ( int N, int b, float *x )
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

void Solver::setBoundariesB0 ( int N, float *x )
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

void Solver::linearSolve ( int N, int b, float * x, float * x0, float a, float c )
{
	int i, j, k, l;
	//float cprime = c ;
	float vu, vd, vl, vr, vf, vb, vc;
	for ( l=0 ; l<NB_ITERATION_SOLVE ; l++ ) {
		for( k=1; k<N+1; ++k ){ 
			for( j = 1; j<N+1 ; ++j) { 
				for( i=1 ; i<N+1 ; ++i ){
					vc = x0[IX(i,j,k)];
					vl = x[IX(i-1,j,k)];
					vr = x[IX(i+1,j,k)];
					vu = x[IX(i,j+1,k)];
					vd = x[IX(i,j-1,k)];
					vf = x[IX(i,j,k+1)];
					vb = x[IX(i,j,k-1)];
					/*
					if( isSolidCell( i-1, j, k ) ){ vl = vc; }
					if( isSolidCell( i+1, j, k ) ){ vr = vc; }
					if( isSolidCell( i, j-1, k ) ){ vd = vc; }
					if( isSolidCell( i, j+1, k ) ){ vu = vc; }
					if( isSolidCell( i, j, k-1 ) ){ vb = vc; }
					if( isSolidCell( i, j, k+1 ) ){ vf = vc; }
					*/
					x[IX(i,j,k)] = ( vc + a*( vl+vr+vu+vd+vf+vb ))/c;
				}
			}
		}
		setBoundaries ( N, b, x );
	}
}

void Solver::diffuse ( int N, int b, float * x, float * x0, float diff, float dt )
{
	float a=dt*diff*N*N;
	linearSolve ( N, b, x, x0, a, 1+6*a );
}

void Solver::diffuseFireAndSmoke ( int N, int b1, int b2, int b3,
				   float * x, float * x0, 
				   float * f, float * f0, 
				   float * t, float * t0, 
				   float diffFire, float diffSmoke, float diffTemp, 
				   float dt )
{
	/* RIEN OBSOLETE */
}

void Solver::advect ( int N, int b, float * d, float * d0, float * u, float * v, float *w, float dt )
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	float x, y, z, s0, t0, r0, s1, t1, r1, dt0;
	float rhs, lhs ;

	float aa, ab, ac, ad, ba, bb, bc, bd;

	dt0 = dt*N;
	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				//if( isSolidCell( i,j,k ) ) d[IX(i,j,k)] = 0;
				//else {
					x = i-dt0*u[IX(i,j,k)] ; y = j-dt0*v[IX(i,j,k)]; z = k-dt0*w[IX(i,j,k)];
					
					if (x<0.5f) x=0.5f; if (x>N+0.5f) x=N+0.5f; i0=(int)x; i1=i0+1;			
					if (y<0.5f) y=0.5f; if (y>N+0.5f) y=N+0.5f; j0=(int)y; j1=j0+1;
					if (z<0.5f) z=0.5f; if (z>N+0.5f) z=N+0.5f; k0=(int)z; k1=k0+1;
					
					s1 = (x-i0); s0 = 1-s1; t1 = (y-j0); t0 = 1-t1; r1 = (z-k0); r0 = 1-r1;

					
					aa = d0[IX(i0,j0,k0)]; //if( isSolidCell( i0, j0, k0 ) ) aa = 0 ;
					ab = d0[IX(i0,j0,k1)]; //if( isSolidCell( i0, j0, k1 ) ) ab = 0 ;
					ac = d0[IX(i0,j1,k0)]; //if( isSolidCell( i0, j1, k0 ) ) ac = 0 ;
					ad = d0[IX(i0,j1,k1)]; //if( isSolidCell( i0, j1, k1 ) ) ad = 0 ;
					  
					ba = d0[IX(i1,j0,k0)]; //if( isSolidCell( i1, j0, k0 ) ) ba = 0 ;
					bb = d0[IX(i1,j0,k1)]; //if( isSolidCell( i1, j0, k1 ) ) bb = 0 ;
					bc = d0[IX(i1,j1,k0)]; //if( isSolidCell( i1, j1, k0 ) ) bc = 0 ;
					bd = d0[IX(i1,j1,k1)]; //if( isSolidCell( i1, j1, k1 ) ) bd = 0 ;
					
					rhs = (t0*(r0*aa + r1*ab) + 
					       t1*(r0*ac + r1*ad));
					lhs = (t0*(r0*ba + r1*bb) + 
					       t1*(r0*bc + r1*bd));
					
					d[IX(i,j,k)] = (s0*rhs + s1*lhs );
				//}
			}
		}
	}
	setBoundaries ( N, b, d );
} 

void Solver::advect_cool ( int N, int b, 
			   float * d, float * d0, 
			   float * u, float * v, float *w, 
			   float dt )
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	float x, y, z, s0, t0, r0, s1, t1, r1, dt0;
	float rhs, lhs, c0;
	float aa, ab, ac, ad, ba, bb, bc, bd;

	dt0 = dt*N;
	c0 = 1.0f - SolverParam::getCoolingParam()*dt;
	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				//if( isSolidCell( i,j,k ) ) d[IX(i,j,k)] = 0;
				//else {
					x = i-dt0*u[IX(i,j,k)] ; y = j-dt0*v[IX(i,j,k)]; z = k-dt0*w[IX(i,j,k)];
					
					if (x<0.5f) x=0.5f; if (x>N+0.5f) x=N+0.5f; i0=(int)x; i1=i0+1;			
					if (y<0.5f) y=0.5f; if (y>N+0.5f) y=N+0.5f; j0=(int)y; j1=j0+1;
					if (z<0.5f) z=0.5f; if (z>N+0.5f) z=N+0.5f; k0=(int)z; k1=k0+1;
					
					s1 = (x-i0); s0 = 1-s1; t1 = (y-j0); t0 = 1-t1; r1 = (z-k0); r0 = 1-r1;
					
					aa = d0[IX(i0,j0,k0)]; //if( isSolidCell( i0, j0, k0 ) ) aa = 0 ;
					ab = d0[IX(i0,j0,k1)]; //if( isSolidCell( i0, j0, k1 ) ) ab = 0 ;
					ac = d0[IX(i0,j1,k0)]; //if( isSolidCell( i0, j1, k0 ) ) ac = 0 ;
					ad = d0[IX(i0,j1,k1)]; //if( isSolidCell( i0, j1, k1 ) ) ad = 0 ;
					  
					ba = d0[IX(i1,j0,k0)]; //if( isSolidCell( i1, j0, k0 ) ) ba = 0 ;
					bb = d0[IX(i1,j0,k1)]; //if( isSolidCell( i1, j0, k1 ) ) bb = 0 ;
					bc = d0[IX(i1,j1,k0)]; //if( isSolidCell( i1, j1, k0 ) ) bc = 0 ;
					bd = d0[IX(i1,j1,k1)]; //if( isSolidCell( i1, j1, k1 ) ) bd = 0 ;
					
					rhs = (t0*(r0*aa + r1*ab) + 
					       t1*(r0*ac + r1*ad));
					lhs = (t0*(r0*ba + r1*bb) + 
					       t1*(r0*bc + r1*bd));
					
					d[IX(i,j,k)] = (s0*rhs + s1*lhs )*c0;
				//}
			}
		}
	}
	setBoundaries ( N, b, d );
}

void Solver::calcDiv( int N , float *u, float *v, float *w, float *div, float *p ){
	int i,j,k;

	float fw,bw,le,ri,up,dw;

	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				fw = w[IX(i,j,k+1)]; //if( isSolidCell( i,j,k+1 ) ) fw = _srcw[IX(i,j,k+1)];
				bw = w[IX(i,j,k-1)]; //if( isSolidCell( i,j,k-1 ) ) bw = _srcw[IX(i,j,k-1)];
				le = u[IX(i-1,j,k)]; //if( isSolidCell( i-1,j,k ) ) le = _srcu[IX(i-1,j,k)];
				ri = u[IX(i+1,j,k)]; //if( isSolidCell( i+1,j,k ) ) ri = _srcu[IX(i+1,j,k)];
				up = v[IX(i,j+1,k)]; //if( isSolidCell( i,j+1,k ) ) up = _srcv[IX(i,j+1,k)];
				dw = v[IX(i,j-1,k)]; //if( isSolidCell( i,j-1,k ) ) dw = _srcv[IX(i,j-1,k)];
				div[IX(i,j,k)] = -( le-ri + fw-bw + up-dw)/(2*N);
				p[IX(i,j,k)] = 0;
			}
		}
	}	
	setBoundaries ( N, 0, div ); setBoundaries ( N, 0, p );
}

void Solver::correctVel( int N, float *u, float * v, float *w, float * p){
	int i, j, k;
	
	float pfw,pbw,ple,pri,pup,pdw, pc;
	bool  cpb = false ,cvb= false ,chb = false  ;

	for( k=1; k<N+1; ++k ){ 
		for( j = 1; j<N+1 ; ++j) { 
			for( i=1 ; i<N+1 ; ++i ){
				//if( isSolidCell( i,j,k ) ) {
				//	u[IX(i,j,k)] = 0;
				//	v[IX(i,j,k)] = 0;
				//	w[IX(i,j,k)] = 0;
				//} else {
					pc = p[IX(i,j,k)];
					ple = p[IX(i-1,j,k)]; //if( isSolidCell( i-1, j,k ) ) { cpb = true ; ple = pc; } 
					pri = p[IX(i+1,j,k)]; //if( isSolidCell( i+1, j,k ) ) { cpb = true ; pri = pc; } 
					pup = p[IX(i,j+1,k)]; //if( isSolidCell( i, j+1,k ) ) { cvb = true ; pup = pc; } 
					pdw = p[IX(i,j-1,k)]; //if( isSolidCell( i, j-1,k ) ) { cvb = true ; pdw = pc; } 
					pbw = p[IX(i,j,k-1)]; //if( isSolidCell( i, j,k-1 ) ) { chb = true ; pbw = pc; } 
					pfw = p[IX(i,j,k+1)]; //if( isSolidCell( i, j,k+1 ) ) { chb = true ; pfw = pc; } 
					u[IX(i,j,k)] -= cpb?0:(ple - pri)/2 ; 
					v[IX(i,j,k)] -= cvb?0:(pup - pdw)/2 ; 
					w[IX(i,j,k)] -= chb?0:(pfw - pbw)/2 ; 
				//}
			}
		}
	}
	setBoundaries ( N, 1, u ); 
	setBoundaries ( N, 2, v ); 
	setBoundaries( N, 3, w);
}

void Solver::project ( int N, float * u, float * v, float *w, float * p, float * div )
{
	calcDiv( N, u, v, w, div, p);
	linearSolve ( N, 0, p, div, 1, 6 );
	correctVel( N, u, v, w, p);
}

void Solver::vorticity_confinement( int N, float *u, float *v, float *w, 
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
	SWAP ( _d0, _d ); diffuse ( _N, 0, _d, _d0, SolverParam::getDiffusionParamFire() , dt );
	SWAP ( _f0, _f ); diffuse ( _N, 0, _f, _f0, SolverParam::getDiffusionParamSmoke() , dt );
	SWAP ( _T0, _T ); diffuse ( _N, 0, _T, _T0, SolverParam::getDiffusionParamTemperature() , dt );
	
	SWAP ( _d0, _d ); advect ( _N, 0, _d, _d0, _u, _v, _w, dt );
	SWAP ( _f0, _f ); advect ( _N, 0, _f, _f0, _u, _v, _w, dt );
	SWAP ( _T0, _T ); advect_cool ( _N, 0, _T, _T0, _u, _v, _w, dt );
	combustion( _N, _d, _f, _T, SolverParam::getConsumingParam(),SolverParam::getFireToSmokeParam(), dt );
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
    /*
	addSource ( _N, _u, _srcu, dt ); 
	addSource ( _N, _v, _srcv, dt ); 
	addSource ( _N, _w, _srcw, dt );
	*/
	addSource3 ( _N, _u, _srcu,_v, _srcv, _w, _srcw, dt ); 
	addBuoyancy( _N, _T, _v, SolverParam::getBuoyancyParam(), dt);
	vorticity_confinement( _N, _u, _v, _w, 
	                       _u0, _v0, _w0, _T0, 
	                       SolverParam::getVorticityConfinementParam(), 
	                       dt);


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


void Solver::clearFilledInfo(){
	Voxel* ptr = _filled ;
	int N = _N;
	for( int i = 0; i < SIZE; ++i, ++ptr){
		_filled[i] = Voxel(); 
	}
}

   

void Solver::updateInfo( Object& o){
    // Si ça n'intersecte pas on ne considère rien
    if( ! intersect( AABB, o.getAABB() ) ){
		return;
	}
 
    Vecteur3I solverCell;
    Voxel *voxelObj = o.grille;
    
    if (voxelObj == NULL)
        return;
        
    float temp;
    float pyrolise;
     
    //cout << "updateInfo : objet " << (void*)&o << endl;
    
    for (int i = 0; i < o.grilleSize.x * o.grilleSize.y * o.grilleSize.z ; i++)
    {
        //cout << " i = " << i << endl;
        // transforme la coord du voxel de lobjet en coord du voxel solver
        //(*voxelObj).pos.afficher();
        solverCell = objectCellToSolverCell((*voxelObj).pos, o.getAABB().lowerCorner);
        
        if ( cellIsValid( solverCell) )
        {
            //////////////////
            // presence     //
            //////////////////
            if ((*voxelObj).plein)
            {
                // presence
                _filled[IX(solverCell.x, solverCell.y, solverCell.z)].plein = (*voxelObj).plein;
                // vitesse
                _u[IX(solverCell.x, solverCell.y, solverCell.z)] = o.vitesse.x;
                _v[IX(solverCell.x, solverCell.y, solverCell.z)] = o.vitesse.y;
                _w[IX(solverCell.x, solverCell.y, solverCell.z)] = o.vitesse.z;
            }
            //temp, combustible, vitesse 
            //uniquement sur les frontieres
            if ((*voxelObj).frontiere)
            {
                //////////////////
                // temperature  //
                //////////////////
                temp = _T[IX(solverCell.x, solverCell.y, solverCell.z)];
                temp = ( temp + (*voxelObj).temperature ) / 2.0;
                // maj du voxel et du tableau des temps
                (*voxelObj).temperature0 = temp;
                (*voxelObj).temperature = temp;
                _T[IX(solverCell.x, solverCell.y, solverCell.z)] = temp;
                
                //////////////////
                // combustible  //
                //////////////////
                //generation de combustible
                //et si la temp est assez grande pour la pyrolise
                if (temp > (*voxelObj).tempThreshold)
                {
                    // on enleve de la matiere
                    // et on recupere la qte cree
                    (*voxelObj).combustibleRestant -= (*voxelObj).tauxPerte;
                    if ( (*voxelObj).combustibleRestant <= 0)
                    {
                        // conservation de la matiere
                        // on ne peut creer plus de matiere qu'il n'y en avait
                        pyrolise = (*voxelObj).tauxPerte + (*voxelObj).combustibleRestant;
                        // il faut signaler que le voxel est vide
                        // et qu'il n'est plus une frontiere
                        
                        /////////////////////////////////////
                        //            TO DO                //
                        //            TO DO                //
                        //            TO DO                //
                        //            TO DO                //        
                        /////////////////////////////////////
                        
                    }
                    else
                    {
                        // generation de matiere
                        pyrolise = (*voxelObj).tauxConversion;
                    }
                    // on ajoute la matiere cree aux sources
                    _srcd[IX(solverCell.x, solverCell.y, solverCell.z)] += pyrolise * (*voxelObj).tauxConversion;
                }
                
                //////////////////
                // vitesse      //
                //////////////////
                // la repulsion ecrase la vitesse en frontiere
                _u[IX(solverCell.x, solverCell.y, solverCell.z)] = (*voxelObj).repulsion.x * 0.1 + o.vitesse.x;
                _v[IX(solverCell.x, solverCell.y, solverCell.z)] = (*voxelObj).repulsion.y * 0.1 + o.vitesse.y;
                _w[IX(solverCell.x, solverCell.y, solverCell.z)] = (*voxelObj).repulsion.z * 0.1 + o.vitesse.z;
            }    
            
        }
        
        // MAJ
        voxelObj++;
        
    }
    
}
