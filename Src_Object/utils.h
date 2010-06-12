#ifndef UTILS_H
#define UTILS_H

/* utility classes */
#include <cmath>
#include <ostream>
using namespace std;

#define S2SQUARE(x) ((x)*(x))
#define S2CUBE(x)   ((x)*(x)*(x))
#define TINY_DOUBLE  0.001f 

inline bool zero( const double f ){ return fabs( f ) < TINY_DOUBLE ; }
inline bool negative( const double f ){ return f < -TINY_DOUBLE ; }
inline bool positive( const double f ){ return f > TINY_DOUBLE ; }

class D3Matrix;

/* 3DVector ( 3 component vectors ) */
class D3Vector {

public:
	double vec[3];
	
	D3Vector();
	D3Vector( const D3Vector &v );
	D3Vector( double x, double y, double z );
	
	D3Vector& operator+=(const D3Vector &v);
	D3Vector& operator-=(const D3Vector &v);
	D3Vector& operator*=(double r);
	
	friend D3Vector operator+(const D3Vector &lhs, const D3Vector &rhs);
	friend D3Vector operator-(const D3Vector &lhs, const D3Vector &rhs);
	friend D3Vector operator-(const D3Vector &lhs);
	friend D3Vector operator*(const D3Vector &lhs, double r);
	friend D3Vector operator*(double r, const D3Vector &rhs);
	friend D3Vector operator*(const D3Matrix &lhs, const D3Vector &rhs);
	friend ostream& operator<<(ostream& o, D3Vector& vec);

	inline friend bool operator==( const D3Vector &lhs, const D3Vector &rhs) {
		return  zero( lhs(0) - rhs(0) ) && 
			zero( lhs(0) - rhs(0) ) &&
			zero( lhs(0) - rhs(0) ) ;
	}

	/* element access */
	inline double operator()(int index) const {
		return vec[index] ;
	}

	inline double& operator()(int index) {
		return vec[index] ;
	}
	
	inline double operator[](int index) const {
		return vec[index] ;
	}
	
	inline double& operator[](int index) {
		return vec[index] ;
	}
	

	/* Norm */
	inline double norm() const { 
		return sqrtf( S2SQUARE(vec[0]) 
			      + S2SQUARE(vec[1]) 
			      + S2SQUARE(vec[2])) ; 
	}
	inline double squareNorm() const {
		return  S2SQUARE(vec[0]) 
			+ S2SQUARE(vec[1]) 
			+ S2SQUARE(vec[2]) ;
	}
	
	/* Normalize */
	inline D3Vector& normalize() { 
		double n = norm();
		if(n != 0) {
			vec[0] /= n;
			vec[1] /= n;
			vec[2] /= n;
		}
		return *this;
	}

	/* Scalar product */
	inline friend double dot_product( const D3Vector &a, 
					 const D3Vector &b ) {
		return a(0)*b(0) + a(1)*b(1) + a(2)*b(2) ;
	}

	/* Cross product */
	inline friend D3Vector cross_product( const D3Vector &a , 
					      const D3Vector &b ) {
		D3Vector t ;
		t(0) = a(1)*b(2) - a(2)*b(1) ;
		t(1) = a(2)*b(0) - a(0)*b(2) ;
		t(2) = a(0)*b(1) - a(1)*b(0) ;
		return 	t ;	
	}

};

/* 4DVector */
class D4Vector {
private :
	double vec[4];
public :
	/* element access */
	inline double operator()(int index) const {
		return vec[index] ;
	}

	inline double& operator()(int index) {
		return vec[index] ;
	}

	/* Norm */
	inline double norm() const { 
		return sqrtf( S2SQUARE(vec[0]) 
			      + S2SQUARE(vec[1]) 
			      + S2SQUARE(vec[2])
			      + S2SQUARE(vec[3])
			) ; 
	}

	/* Scalar product */
	inline static double dot_product( const D3Vector &a, 
					 const D3Vector &b ) {
		return a(0)*b(0) + a(1)*b(1) + a(2)*b(2) + a(3)*b(3) ;
	}

};

/* 3DMatrix ( 3x3 matrices ) */
class D3Matrix {
public :
	double mat[9];

	D3Matrix();

	/* element acces */
	inline double  operator()(int i, int j) const {
		return mat[i*3+j];		
	}

	inline double& operator()(int i, int j) {
		return mat[i*3+j];		
	}
	
	/* ID */
	inline static D3Matrix ID() {
		D3Matrix id ;
		for(int i = 0 ; i<3 ; ++i )
			id(i,i)=1.;
		return id;
	}
	
	/* Operators */	
	inline friend D3Matrix operator*(const D3Matrix &lhs, double r) {
		
		D3Matrix v;
		
		v.mat[0] = lhs.mat[0] * r;
		v.mat[1] = lhs.mat[1] * r;
		v.mat[2] = lhs.mat[2] * r;
		v.mat[3] = lhs.mat[3] * r;
		v.mat[4] = lhs.mat[4] * r;
		v.mat[5] = lhs.mat[5] * r;
		v.mat[6] = lhs.mat[6] * r;
		v.mat[7] = lhs.mat[7] * r;
		v.mat[8] = lhs.mat[8] * r;
		
		return v;
	}
	
	inline friend D3Matrix operator*(double r, const D3Matrix &rhs) {
		return operator*(rhs, r);
	}
	
	inline friend D3Matrix operator*(const D3Matrix &lhs, D3Matrix &rhs) {
		D3Matrix v;
		
		v.mat[0] = lhs.mat[0]*rhs.mat[0] + lhs.mat[1]*rhs.mat[3] + lhs.mat[2]*rhs.mat[6];
		v.mat[1] = lhs.mat[0]*rhs.mat[1] + lhs.mat[1]*rhs.mat[4] + lhs.mat[2]*rhs.mat[7]; 
		v.mat[2] = lhs.mat[0]*rhs.mat[2] + lhs.mat[1]*rhs.mat[5] + lhs.mat[2]*rhs.mat[8];
		
		v.mat[3] = lhs.mat[3]*rhs.mat[0] + lhs.mat[4]*rhs.mat[3] + lhs.mat[5]*rhs.mat[6];
		v.mat[4] = lhs.mat[3]*rhs.mat[1] + lhs.mat[4]*rhs.mat[4] + lhs.mat[5]*rhs.mat[7]; 
		v.mat[5] = lhs.mat[3]*rhs.mat[2] + lhs.mat[4]*rhs.mat[5] + lhs.mat[5]*rhs.mat[8];
		
		v.mat[6] = lhs.mat[6]*rhs.mat[0] + lhs.mat[7]*rhs.mat[3] + lhs.mat[8]*rhs.mat[6]; 
		v.mat[7] = lhs.mat[6]*rhs.mat[1] + lhs.mat[7]*rhs.mat[4] + lhs.mat[8]*rhs.mat[7]; 
		v.mat[8] = lhs.mat[6]*rhs.mat[2] + lhs.mat[7]*rhs.mat[5] + lhs.mat[8]*rhs.mat[8];
		
		return v;
	}
	
	inline D3Matrix& operator*=(double r) {
		mat[0] *= r;
		mat[1] *= r;
		mat[2] *= r;
		mat[3] *= r;
		mat[4] *= r;
		mat[5] *= r;
		mat[6] *= r;
		mat[7] *= r;
		mat[8] *= r;
		return *this;
	}
	
	inline friend D3Vector operator*(const D3Matrix &lhs, const D3Vector &rhs) {
		// On passe 12% du temps du programme dans cette methode !
		D3Vector v;
		double s=rhs.vec[0], t=rhs.vec[1], u=rhs.vec[2];
		const double *pointer = lhs.mat;
		v.vec[0] = (*pointer)*s + (*(++pointer))*t + (*(++pointer))*u;
		v.vec[1] = (*(++pointer))*s + (*(++pointer))*t + (*(++pointer))*u;
		v.vec[2] = (*(++pointer))*s + (*(++pointer))*t + (*(++pointer))*u;
		return v;
	}
	
	friend ostream& operator<<(ostream& o, D3Matrix& mat);
	
	/* return M transposed */
	inline D3Matrix transpose() const { 
		D3Matrix res ;
		int i,j,k,l;
		for( i=0, k=0; i<3 ; ++i , ++k )
			for( j=0, l=0; j<3 ; ++j , ++l )
				res(l,k) = (*this)(i,j);
		return res;
	}

	/* Inverse */
	inline D3Matrix inverse() {
		D3Matrix result ;
		
		double determinant = +(*this)(0,0)*((*this)(1,1)*(*this)(2,2)-(*this)(2,1)*(*this)(1,2))
		-(*this)(0,1)*((*this)(1,0)*(*this)(2,2)-(*this)(1,2)*(*this)(2,0))
		+(*this)(0,2)*((*this)(1,0)*(*this)(2,1)-(*this)(1,1)*(*this)(2,0));
		double invdet = 1./determinant;
		result(0,0) =  ((*this)(1,1)*(*this)(2,2)-(*this)(2,1)*(*this)(1,2))*invdet;
		result(1,0) = -((*this)(0,1)*(*this)(2,2)-(*this)(0,2)*(*this)(2,1))*invdet;
		result(2,0) =  ((*this)(0,1)*(*this)(1,2)-(*this)(0,2)*(*this)(1,1))*invdet;
		result(0,1) = -((*this)(1,0)*(*this)(2,2)-(*this)(1,2)*(*this)(2,0))*invdet;
		result(1,1) =  ((*this)(0,0)*(*this)(2,2)-(*this)(0,2)*(*this)(2,0))*invdet;
		result(2,1) = -((*this)(0,0)*(*this)(1,2)-(*this)(1,0)*(*this)(0,2))*invdet;
		result(0,2) =  ((*this)(1,0)*(*this)(2,1)-(*this)(2,0)*(*this)(1,1))*invdet;
		result(1,2) = -((*this)(0,0)*(*this)(2,1)-(*this)(2,0)*(*this)(0,1))*invdet;
		result(2,2) =  ((*this)(0,0)*(*this)(1,1)-(*this)(1,0)*(*this)(0,1))*invdet;
		
		return result ;	
	}
	
};

/* 4DMatrix ( 4x4 matrices ) */
class D4Matrix {
public :
	double mat[4][4];

	D4Matrix();

	/* element acces */
	inline double  operator()(int i, int j) const {
		return mat[i][j];		
	}

	inline double& operator()(int i, int j) {
		return mat[i][j];		
	}
	
	/* ID */
	inline static D4Matrix ID() {
		D4Matrix id ;
		for(int i = 0 ; i<4 ; ++i )
			id(i,i)=1.;
		return id;
	}
	
	/* Operators */
	friend D4Matrix operator*(const D4Matrix &lhs, D4Matrix &rhs);
	friend D4Matrix operator*(const D4Matrix &lhs, double r);
	friend D4Matrix operator*(double r, const D4Matrix &rhs);
	
	/* return M transposed */
	inline D4Matrix transpose() const { 
		D4Matrix res ;
		int i,j,k,l;
		for( i=0, k=0; i<4 ; ++i , ++k )
			for( j=0, l=0; j<4 ; ++j , ++l )
				res(l,k) = (*this)(i,j);
		return res;
	}

	
};

#endif
