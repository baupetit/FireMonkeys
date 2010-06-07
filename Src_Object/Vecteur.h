#ifndef VECTEUR_H
#define VECTEUR_H

#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#include <iostream>
using namespace std;

template <typename T>
class Vecteur2{
public:
	T x;
	T y;
	
	Vecteur2() : x(0), y(0){}
	Vecteur2(T xx, T yy) : x(xx), y(yy){}
	Vecteur2(const Vecteur2<T>& vec) : x(vec.x), y(vec.y){}
	
	Vecteur2<T>& operator= (Vecteur2<T>& vec){
		Vecteur2<T> *res = new Vecteur2<T>(vec.x, vec.y);
		return *res;
	}
};

template <typename T>
class Vecteur3 : public Vecteur2<T>{
public:
	T z;
    
	Vecteur3() : z(0){}
	
	Vecteur3(T xx, T yy, T zz) : Vecteur2<T>(xx,yy), z(zz){}
	
	Vecteur3(const Vecteur3<T>& vec) : Vecteur2<T>(vec.x,vec.y), z(vec.z){}
    
	Vecteur3<T>& operator= (const Vecteur3<T>& vec){
		this->x=vec.x;
		this->y=vec.y;
		this->z=vec.z;
		return *this;
	}
    
	void normaliser(){
		float norme = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
		this->x = this->x/norme;
		this->y = this->y/norme;
		this->z = this->z/norme;
	}
    
	friend Vecteur3<T> produitVectoriel(const Vecteur3<T>& v1, const Vecteur3<T>& v2){
		Vecteur3<T> res;
		res.x = v1.y * v2.z - v1.z * v2.y;
		res.y = v1.z * v2.x - v1.x * v2.z;
		res.z = v1.x * v2.y - v1.y * v2.x;
		return res;
	}
	
	friend float produitScalaire(const Vecteur3<T>& v1, const Vecteur3<T>& v2){
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
    	
	friend Vecteur3<T> operator+ (const Vecteur3<T>& v1, const Vecteur3<T>& v2){
		Vecteur3<T> res(v1);
		res+=v2;
		return res;
	}
    
	friend Vecteur3<T> operator- (const Vecteur3<T>& v1, const Vecteur3<T>& v2){
		Vecteur3<T> res(v1);
		res-=v2;
		return res;
	}
    
	friend Vecteur3<T> operator- (const Vecteur3<T>& v1){
		Vecteur3<T> res(0,0,0);
		res-=v1;
		return res;
	}
    
	friend Vecteur3<T> operator+ (const Vecteur3<T>& v1){
		Vecteur3<T> res(0,0,0);
		res+=v1;
		return res;
	}
    
	Vecteur3<T>& operator+= (const Vecteur3<T>& v2){
		this->x = this->x + v2.x;
		this->y = this->y + v2.y;
		this->z = this->z + v2.z;
		return *this;
	}
    
        
	Vecteur3<T>& operator-= (const Vecteur3<T>& v2){
		this->x = this->x - v2.x;
		this->y = this->y - v2.y;
		this->z = this->z - v2.z;
		return *this;
	}
    
	friend Vecteur3<T> operator*(const Vecteur3<T>& v1, float coeff){
		Vecteur3<T> res(v1);
		res.x*=coeff;
		res.y*=coeff;
		res.z*=coeff;
		return res;
	} 
    
	friend Vecteur3<T> operator*(float coeff, const Vecteur3<T>& v1){
		return v1*coeff;
	}    
    
	friend Vecteur3<T> rotationAutourDunVecteur(const Vecteur3<T>& v1, const Vecteur3<T>& v2, float angle){
		Vecteur3<T> res;
		res = v1*cosf(angle)+(1-cosf(angle))*(produitScalaire(v1,v2))*v2+sinf(angle)*produitVectoriel(v2,v1);
		return res;
	}
    
	void afficher(){
		cout << "x : "<< this->x <<", y : "<< this->y <<", z : "<< this->z <<endl;
	}
    
	void rotationAutourAxeX(float angleRad){
		// x = x;
		float y2, z2;
		y2 = this->y * cosf(angleRad) - this->z * sinf(angleRad);
		z2 = this->y * sinf(angleRad) + this->z * cosf(angleRad);
		this->y = y2;
		this->z = z2;
	}
    
	void rotationAutourAxeY(float angleRad){
		float x2, z2;
		x2 = this->x * cosf(angleRad) - this->z * sinf(angleRad);
		z2 = this->x * sinf(angleRad) + this->z * cosf(angleRad);
		this->x = x2;
		this->z = z2;
	}
    
	void rotationAutourAxeZ(float angleRad){
		float x2, y2;
		x2 = this->x * cosf(angleRad) - this->y * sinf(angleRad);
		y2 = this->x * sinf(angleRad) + this->y * cosf(angleRad);
		this->x = x2;
		this->y = y2;
		// z = z;
	}
	
	friend float norme(Vecteur3<T>& v){
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}
};

template <typename T>
class Vecteur4 : public Vecteur3<T> {
public:
	T w;

	Vecteur4<T>() : w(0){}
	Vecteur4<T>(T xx, T yy, T zz, T ww) : Vecteur3<T>(xx,yy,zz), w(ww) {}
	Vecteur4<T>(const Vecteur4<T>& vec) : Vecteur3<T>(vec.x,vec.y,vec.z), w(vec.w){}
	
};

typedef Vecteur3<float> Vecteur3D;
typedef Vecteur3<int> Vecteur3I;

typedef Vecteur4<float> Vecteur4D;


#endif
