#include "Shader.h"
#include "loadprogram.h"


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
