
uniform sampler3D feu;

void main (){
    
    vec4 speed;
	speed = vec4( texture3D( feu,gl_TexCoord[0].stp) );
    gl_FragColor = vec4(speed.x, speed.y, speed.z, 1.0);

    
}




