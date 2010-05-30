#define TAILLE          10
#define TAILLE_CELL     (1.0/TAILLE)

uniform sampler3D TextureSpeedField_Prec;

void main (){
    
    vec4 speed;


	speed = vec4( texture3D( TextureSpeedField_Prec,gl_TexCoord[0].stp) );
	speed = vec4( speed.x + 0.0 , speed.y + 0.8, speed.z + 0.8,speed.w);

    gl_FragColor = vec4(1.0,1.0,0.0 , 1.0);
    
}




