#include "utils.h"

D3Vector::D3Vector(){}

D3Vector::D3Vector( double x, double y, double z ){
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

D3Vector::D3Vector( const D3Vector &v ){
	vec[0] = v(0);
	vec[1] = v(1);
	vec[2] = v(2);
}

D3Matrix::D3Matrix(){
	for(int i=0; i<3; ++i )
		for( int j =0; j<3 ; ++j)
			(*this)(i,j) = 0;
}

D4Matrix::D4Matrix(){
	for(int i=0; i<4; ++i )
		for( int j =0; j<4 ; ++j)
			(*this)(i,j) = 0;
}

D3Vector& D3Vector::operator+=(const D3Vector &v) {
	vec[0] += v.vec[0];
	vec[1] += v.vec[1];
	vec[2] += v.vec[2];
	return *this;
}

D3Vector& D3Vector::operator-=(const D3Vector &v) {
	vec[0] -= v.vec[0];
	vec[1] -= v.vec[1];
	vec[2] -= v.vec[2];
	return *this;
}

D3Vector& D3Vector::operator*=(double r) {
	vec[0] *= r;
	vec[1] *= r;
	vec[2] *= r;
	return *this;
}

D3Vector operator+(const D3Vector &lhs, const D3Vector &rhs) {
	D3Vector v;
	v.vec[0] = lhs.vec[0] + rhs.vec[0];
	v.vec[1] = lhs.vec[1] + rhs.vec[1];
	v.vec[2] = lhs.vec[2] + rhs.vec[2];
	return v;
}

D3Vector operator-(const D3Vector &lhs, const D3Vector &rhs) {
	D3Vector v;
	v.vec[0] = lhs.vec[0] - rhs.vec[0];
	v.vec[1] = lhs.vec[1] - rhs.vec[1];
	v.vec[2] = lhs.vec[2] - rhs.vec[2];
	return v;
}

D3Vector operator-(const D3Vector &lhs){
	D3Vector v;
	v.vec[0] = -lhs.vec[0];
	v.vec[1] = -lhs.vec[1];
	v.vec[2] = -lhs.vec[2];
	return v;
}

D3Vector operator*(const D3Vector &lhs, double r) {
	D3Vector v;
	v.vec[0] = lhs.vec[0] * r;
	v.vec[1] = lhs.vec[1] * r;
	v.vec[2] = lhs.vec[2] * r;
	return v;
}

D3Vector operator*(double r, const D3Vector &rhs) {
	return operator*(rhs, r);
}

ostream& operator<<(ostream& o, D3Vector& vec) {
	o << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")" << endl;
	return o;
}

/* D3Matrix */

ostream& operator<<(ostream& o, D3Matrix &mat) {
	o << "/" << mat(0,0) << ", " << mat(0,1) << ", " << mat(0,2) << "\\" << endl;
	o << "|" << mat(1,0) << ", " << mat(1,1) << ", " << mat(1,2) << "|" << endl;
	o << "\\" << mat(2,0) << ", " << mat(2,1) << ", " << mat(2,2) << "/" << endl;
	return o;
}

/* D4Matrix */
D4Matrix operator*(const D4Matrix &lhs, double r) {
	
	D4Matrix v;
	
	v.mat[0][0] = lhs.mat[0][0] * r;
	v.mat[0][1] = lhs.mat[0][1] * r;
	v.mat[0][2] = lhs.mat[0][2] * r;
	v.mat[0][3] = lhs.mat[0][3] * r;
	
	v.mat[1][0] = lhs.mat[1][0] * r;
	v.mat[1][1] = lhs.mat[1][1] * r;
	v.mat[1][2] = lhs.mat[1][2] * r;
	v.mat[1][3] = lhs.mat[1][3] * r;
	
	v.mat[2][0] = lhs.mat[2][0] * r;
	v.mat[2][1] = lhs.mat[2][1] * r;
	v.mat[2][2] = lhs.mat[2][2] * r;
	v.mat[2][3] = lhs.mat[2][3] * r;
	
	v.mat[3][0] = lhs.mat[3][0] * r;
	v.mat[3][1] = lhs.mat[3][1] * r;
	v.mat[3][2] = lhs.mat[3][2] * r;
	v.mat[3][3] = lhs.mat[3][3] * r;
	
	return v;
}

D4Matrix operator*(double r, const D4Matrix &rhs) {
	return operator*(rhs, r);
}

D4Matrix operator*(const D4Matrix &lhs, D4Matrix &rhs) {
	D4Matrix v;
	
	v.mat[0][0] = lhs.mat[0][0]*rhs.mat[0][0] + lhs.mat[0][1]*rhs.mat[1][0] + lhs.mat[0][2]*rhs.mat[2][0] + lhs.mat[0][3]*rhs.mat[3][0];
	v.mat[0][1] = lhs.mat[0][0]*rhs.mat[0][1] + lhs.mat[0][1]*rhs.mat[1][1] + lhs.mat[0][2]*rhs.mat[2][1] + lhs.mat[0][3]*rhs.mat[3][1];
	v.mat[0][2] = lhs.mat[0][0]*rhs.mat[0][2] + lhs.mat[0][1]*rhs.mat[1][2] + lhs.mat[0][2]*rhs.mat[2][2] + lhs.mat[0][3]*rhs.mat[3][2];
	v.mat[0][3] = lhs.mat[0][0]*rhs.mat[0][3] + lhs.mat[0][1]*rhs.mat[1][3] + lhs.mat[0][2]*rhs.mat[2][3] + lhs.mat[0][3]*rhs.mat[3][3];
	
	v.mat[1][0] = lhs.mat[1][0]*rhs.mat[0][0] + lhs.mat[1][1]*rhs.mat[1][0] + lhs.mat[1][2]*rhs.mat[2][0] + lhs.mat[1][3]*rhs.mat[3][0];
	v.mat[1][1] = lhs.mat[1][0]*rhs.mat[0][1] + lhs.mat[1][1]*rhs.mat[1][1] + lhs.mat[1][2]*rhs.mat[2][1] + lhs.mat[1][3]*rhs.mat[3][1];
	v.mat[1][2] = lhs.mat[1][0]*rhs.mat[0][2] + lhs.mat[1][1]*rhs.mat[1][2] + lhs.mat[1][2]*rhs.mat[2][2] + lhs.mat[1][3]*rhs.mat[3][2];
	v.mat[1][3] = lhs.mat[1][0]*rhs.mat[0][3] + lhs.mat[1][1]*rhs.mat[1][3] + lhs.mat[1][2]*rhs.mat[2][3] + lhs.mat[1][3]*rhs.mat[3][3];
	
	v.mat[2][0] = lhs.mat[2][0]*rhs.mat[0][0] + lhs.mat[2][1]*rhs.mat[1][0] + lhs.mat[2][2]*rhs.mat[2][0] + lhs.mat[2][3]*rhs.mat[3][0];
	v.mat[2][1] = lhs.mat[2][0]*rhs.mat[0][1] + lhs.mat[2][1]*rhs.mat[1][1] + lhs.mat[2][2]*rhs.mat[2][1] + lhs.mat[2][3]*rhs.mat[3][1]; 
	v.mat[2][2] = lhs.mat[2][0]*rhs.mat[0][2] + lhs.mat[2][1]*rhs.mat[1][2] + lhs.mat[2][2]*rhs.mat[2][2] + lhs.mat[2][3]*rhs.mat[3][2];
	v.mat[2][3] = lhs.mat[2][0]*rhs.mat[0][3] + lhs.mat[2][1]*rhs.mat[1][3] + lhs.mat[2][2]*rhs.mat[2][3] + lhs.mat[2][3]*rhs.mat[3][3];
	
	v.mat[3][0] = lhs.mat[3][0]*rhs.mat[0][0] + lhs.mat[3][1]*rhs.mat[1][0] + lhs.mat[3][2]*rhs.mat[2][0] + lhs.mat[3][3]*rhs.mat[3][0];
	v.mat[3][1] = lhs.mat[3][0]*rhs.mat[0][1] + lhs.mat[3][1]*rhs.mat[1][1] + lhs.mat[3][2]*rhs.mat[2][1] + lhs.mat[3][3]*rhs.mat[3][1];
	v.mat[3][2] = lhs.mat[3][0]*rhs.mat[0][2] + lhs.mat[3][1]*rhs.mat[1][2] + lhs.mat[3][2]*rhs.mat[2][2] + lhs.mat[3][3]*rhs.mat[3][2];
	v.mat[3][3] = lhs.mat[3][0]*rhs.mat[0][3] + lhs.mat[3][1]*rhs.mat[1][3] + lhs.mat[3][2]*rhs.mat[2][3] + lhs.mat[3][3]*rhs.mat[3][3];
	
	return v;
}
