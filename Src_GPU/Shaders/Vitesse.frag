#define TAILLE          10
#define TAILLE_CELL     (1.0/TAILLE)

uniform sampler3D TextureSpeedField_Prec;

void main (){
    
    vec4 speed;



	speed = vec4( texture3D( TextureSpeedField_Prec,gl_TexCoord[0].stp) );
	speed = vec4( speed.x + 0.0 , speed.y + 0.8, speed.z + 0.8,speed.w);

    gl_FragColor = vec4(1.0,1.0,0.0 , 1.0);

    
    /*
    
    if(gl_TexCoord[0].s>TAILLE_CELL){
        speed = vec4(texture3D(TextureSpeedField,gl_TexCoord[0].stp+vec3(TAILLE_CELL,TAILLE_CELL,TAILLE_CELL)));
        //speed = vec4(texture3D(TextureSpeedField,gl_TexCoord[0].stp));
    }else{
        speed = vec4(texture3D(TextureSpeedField,gl_TexCoord[0].stp));
    }
    
    */
    speed.x = 1.0;
    
    speed.y = 0.0;
    
    speed.z = 1.0;
    
    
    gl_FragColor = vec4(speed.x, speed.y, speed.z, 1.0);

    
}




