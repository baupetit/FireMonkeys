#include "Vecteur.h"


Vecteur3D& Vecteur3D::operator= (const Vecteur3D& vec){
        x=vec.x;
        y=vec.y;
        z=vec.z;
        return *this;
    }
    
void Vecteur3D::normaliser(){
        float norme = sqrt(x*x + y*y + z*z);
        x = x/norme;
        y = y/norme;
        z = z/norme;
    }
    
     Vecteur3D produitVectoriel(const Vecteur3D& v1, const Vecteur3D& v2){
        Vecteur3D res;
        res.x = v1.y * v2.z - v1.z * v2.y;
        res.y = v1.z * v2.x - v1.x * v2.z;
        res.z = v1.x * v2.y - v1.y * v2.x;
        return res;
    }
    
     float produitScalaire(const Vecteur3D& v1, const Vecteur3D& v2){
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    }
    
    
     Vecteur3D operator+ (const Vecteur3D& v1, const Vecteur3D& v2){
        Vecteur3D res(v1);
        res+=v2;
        return res;
    }
    
     Vecteur3D operator+ (const Vecteur3D& v1){
        Vecteur3D res(0,0,0);
        res+=v1;
        return res;
    }
    
     Vecteur3D operator- (const Vecteur3D& v1, const Vecteur3D& v2){
        Vecteur3D res(v1);
        res-=v2;
        return res;
    }
    
     Vecteur3D operator- (const Vecteur3D& v1){
        Vecteur3D res(0,0,0);
        res-=v1;
        return res;
    }
    
    Vecteur3D& Vecteur3D::operator+= (const Vecteur3D& v2){
        x = x + v2.x;
        y = y + v2.y;
        z = z + v2.z;
        return *this;
    }
    
        
    Vecteur3D& Vecteur3D::operator-= (const Vecteur3D& v2){
        x = x - v2.x;
        y = y - v2.y;
        z = z - v2.z;
        return *this;
    }
    
     Vecteur3D operator*(const Vecteur3D& v1, float coeff){
        Vecteur3D res(v1);
        res.x*=coeff;
        res.y*=coeff;
        res.z*=coeff;
        return res;
    } 
    
     Vecteur3D operator*(float coeff, const Vecteur3D& v1){
        return v1*coeff;
    }    
    
     Vecteur3D rotationAutourDunVecteur(const Vecteur3D& v1, const Vecteur3D& v2, float angle){
        Vecteur3D res;
        res = v1*cosf(angle)+(1-cosf(angle))*(produitScalaire(v1,v2))*v2+sinf(angle)*produitVectoriel(v2,v1);
        return res;
    }
    
    void Vecteur3D::afficher(){
        cout << "x : "<< x <<", y : "<<y<<", z : "<< z <<endl;
    }
    
    void Vecteur3D::rotationAutourAxeX(float angleRad){
        // x = x;
        float y2, z2;
        y2 = y * cosf(angleRad) - z * sinf(angleRad);
        z2 = y * sinf(angleRad) + z * cosf(angleRad);
        y = y2;
        z = z2;
    }
    
    void Vecteur3D::rotationAutourAxeY(float angleRad){
        float x2, z2;
        x2 = x * cosf(angleRad) - z * sinf(angleRad);
        z2 = x * sinf(angleRad) + z * cosf(angleRad);
        x = x2;
        z = z2;
    }
    
    void Vecteur3D::rotationAutourAxeZ(float angleRad){
        float x2, y2;
        x2 = x * cosf(angleRad) - y * sinf(angleRad);
        y2 = x * sinf(angleRad) + y * cosf(angleRad);
        x = x2;
        y = y2;
        // z = z;
    }
    
     float norme(Vecteur3D& v){
        return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    }
