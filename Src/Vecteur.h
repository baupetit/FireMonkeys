#ifndef VECTEUR_H
#define VECTEUR_H

#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

#include <iostream>
using namespace std;

class Vecteur2D{
public:
    float x;
    float y;
    
    Vecteur2D() : x(0), y(0){}
    Vecteur2D(float xx, float yy) : x(xx), y(yy){}
    Vecteur2D(const Vecteur2D& vec) : x(vec.x), y(vec.y){}
    Vecteur2D& operator= (Vecteur2D& vec){
        Vecteur2D *res = new Vecteur2D(vec.x, vec.y);
        return *res;
    }
};

class Vecteur3D : public Vecteur2D{
public:
    float z;
    
    Vecteur3D() : z(0){}
    Vecteur3D(float xx, float yy, float zz) : Vecteur2D(xx,yy), z(zz){}
    Vecteur3D(const Vecteur3D& vec) : Vecteur2D(vec.x,vec.y), z(vec.z){}
    
    Vecteur3D& operator= (const Vecteur3D& vec);
    
    void normaliser();
    
    friend Vecteur3D produitVectoriel(const Vecteur3D& v1, const Vecteur3D& v2);
    
    friend float produitScalaire(const Vecteur3D& v1, const Vecteur3D& v2);
    
    
    friend Vecteur3D operator+ (const Vecteur3D& v1, const Vecteur3D& v2);
    
    friend Vecteur3D operator- (const Vecteur3D& v1, const Vecteur3D& v2);
    
    Vecteur3D& operator+= (const Vecteur3D& v2);
    
        
    Vecteur3D& operator-= (const Vecteur3D& v2);
    
    friend Vecteur3D operator*(const Vecteur3D& v1, float coeff);
    
    friend Vecteur3D operator*(float coeff, const Vecteur3D& v1);
    
    friend Vecteur3D rotationAutourDunVecteur(const Vecteur3D& v1, const Vecteur3D& v2, float angle);
    
    void afficher();
    
    void rorationAutourAxeX(float angleRad);
    
    void rorationAutourAxeY(float angleRad);
    
    void rorationAutourAxeZ(float angleRad);
    
    friend float norme(Vecteur3D& v);
};


#endif
