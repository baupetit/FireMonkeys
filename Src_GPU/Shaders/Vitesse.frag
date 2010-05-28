






#define TAILLE          30
#define TAILLE_CELL     (1.0/TAILLE)

uniform sampler3D TextureSpeedField;



void main (){
    
    vec4 speed = vec4(texture3D(TextureSpeedField,gl_TexCoord[0].stp));
    gl_FragColor = vec4(speed.x, speed.y, speed.z, 1.0);

    
    
}




