#include "Shader.h"
#include "loadprogram.h"
#include "Texture3D.h"

Shader::Shader(string fichier_vert_shader, string fichier_frag_shader){


    cout << "Chargement du Shader program : ";
    cout << fichier_vert_shader;
    cout << " , ";
    cout << fichier_frag_shader;
    cout << endl;
    
    // Chargement du programme
    program = LoadProgram(fichier_vert_shader.c_str(), fichier_frag_shader.c_str());
    cout << "Nouveau shader, id : " << program << endl;
    
    cout << "Shader program chargé" << endl;
    
}

Shader::~Shader(){
}


void Shader::Bind_Program(){	
	glUseProgram(program);
}
    
void Shader::Unbind_Program(){	
    glUseProgram(0);
}



void Shader::lierLevel(string nom_texture, GLuint level){
        GLuint	location;
        location = glGetUniformLocation ( program, nom_texture.c_str());
        glUniform1i(location, level);
}


void Shader::lierVecteur(string nom_vecteur, Vecteur3D& vect){
    GLuint	location = glGetUniformLocation ( program, nom_vecteur.c_str());
    glUniform3f(location, vect.x, vect.y, vect.z);
}


void Shader::lierFloat(string nom_float, float flouttounet){
    GLuint	location = glGetUniformLocation ( program, nom_float.c_str());
    glUniform1f(location, flouttounet);
}


