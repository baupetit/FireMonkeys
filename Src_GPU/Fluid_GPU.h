#ifndef FLUID_GPU_H
#define FLUID_GPU_H

#include <GL/glew.h>
#include <GL/glut.h>


/** Taille de la grille **/
#define TAILLE_GRILLE 4

class Vecteur2D{
public:
    float x;
    float y;
    
    Vecteur2D(){}
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
    
    Vecteur3D(){}
    Vecteur3D(float xx, float yy, float zz) : Vecteur2D(xx,yy), z(zz){}
    Vecteur3D(const Vecteur3D& vec) : Vecteur2D(vec.x,vec.y), z(vec.z){}
    Vecteur3D& operator= (Vecteur3D& vec){
        Vecteur3D *res = new Vecteur3D(vec.x, vec.y, vec.z);
        return *res;
    }
};


class Fluid_GPU{
private:
    GLuint _speed_program;
    
    /** Texture : Champs de viteses */
    GLuint _speedField;
    
    
public:
    Fluid_GPU();
    ~Fluid_GPU();
    void initialiserSpeedField();
    void resolutionSpeedField();
    void displaySpeedField();
    void display_3DTexture(int details, int textureid);
};

#endif
