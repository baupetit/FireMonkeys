
uniform sampler3D feu;




#define DECALAGE 1.0/128.0


vec3 decalage0 = vec3 ( 0.0, 0.0, DECALAGE * 0.0);
vec3 decalage1 = vec3 ( 0.0, 0.0, DECALAGE * 1.0);
vec3 decalage2 = vec3 ( 0.0, 0.0, DECALAGE * 2.0);
vec3 decalage3 = vec3 ( 0.0, 0.0, DECALAGE * 3.0);
vec3 decalage4 = vec3 ( 0.0, 0.0, DECALAGE * 4.0);
vec3 decalage5 = vec3 ( 0.0, 0.0, DECALAGE * 5.0);
vec3 decalage6 = vec3 ( 0.0, 0.0, DECALAGE * 6.0);
vec3 decalage7 = vec3 ( 0.0, 0.0, DECALAGE * 7.0);


void main (){
    
    
    vec3 coord = gl_TexCoord[0].stp;
    
    
	gl_FragData[0] = vec4( texture3D( feu, coord + decalage0));
	gl_FragData[1] = vec4( texture3D( feu, coord + decalage1));
	gl_FragData[2] = vec4( texture3D( feu, coord + decalage2));
	gl_FragData[3] = vec4( texture3D( feu, coord + decalage3));
	gl_FragData[4] = vec4( texture3D( feu, coord + decalage4));
	gl_FragData[5] = vec4( texture3D( feu, coord + decalage5));
	gl_FragData[6] = vec4( texture3D( feu, coord + decalage6));
	gl_FragData[7] = vec4( texture3D( feu, coord + decalage7));
	


    
}



