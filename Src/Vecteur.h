#ifndef VECTEUR_H
#define VECTEUR_H

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
    Vecteur3D& operator= (Vecteur3D& vec){
        Vecteur3D *res = new Vecteur3D(vec.x, vec.y, vec.z);
        return *res;
    }
};


#endif
