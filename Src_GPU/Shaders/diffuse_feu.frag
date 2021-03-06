

uniform sampler3D texture_entree;
uniform sampler3D texture_sortie;

uniform float     taille_width;
uniform float     taille_height;
uniform float     taille_depth;

uniform vec3      a;
uniform vec3      c;

#define DX  1.0/taille_width;
#define DY  1.0/taille_height;
#define DZ  1.0/taille_depth;

#define DECALAGE 1.0/taille_depth


vec3    voisinN        (0.0, DY, 0.0);
vec3    voisinE        (-DX, 0.0, 0.0);
vec3    voisinS        (0.0, -DY, 0.0);
vec3    voisinO        (DX, 0.0, 0.0);
vec3    voisinAV       (0.0, 0.0, DZ);
vec3    voisinAR       (0.0, 0.0, -DZ);


vec3 linearsolve(vec3 coordonnee){
    /*
    vec3 result;
    result = vec3( texture3D ( texture_entree, coordonnee ) .rgb )
             + a * ( texture3D ( texture_sortie, coordonnee + voisinN) .rgb )
                   + texture3D ( texture_sortie, coordonnee + voisinE) .rgb )
                   + texture3D ( texture_sortie, coordonnee + voisinS) .rgb )
                   + texture3D ( texture_sortie, coordonnee + voisinO) .rgb )
                   + texture3D ( texture_sortie, coordonnee + voisinAV) .rgb )
                   + texture3D ( texture_sortie, coordonnee + voisinAR) .rgb )
                   );
    result = vec3( texture3D ( texture_entree, coordonnee ) .rgb )
             + a * ( texture3D ( texture_entree, coordonnee + voisinN) .rgb )
                   + texture3D ( texture_entree, coordonnee + voisinE) .rgb )
                   + texture3D ( texture_entree, coordonnee + voisinS) .rgb )
                   + texture3D ( texture_entree, coordonnee + voisinO) .rgb )
                   + texture3D ( texture_entree, coordonnee + voisinAV) .rgb )
                   + texture3D ( texture_entree, coordonnee + voisinAR) .rgb )
                   );
                   
    //return result / c;
    */
    return vec3(1.0, 1.0, 0.0);
}






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

	gl_FragData[0] = vec4( linearsolve (coord + decalage0), 1.0);
	gl_FragData[1] = vec4( linearsolve (coord + decalage1), 1.0);
	gl_FragData[2] = vec4( linearsolve (coord + decalage2), 1.0);
	gl_FragData[3] = vec4( linearsolve (coord + decalage3), 1.0);
	gl_FragData[4] = vec4( linearsolve (coord + decalage4), 1.0);
	gl_FragData[5] = vec4( linearsolve (coord + decalage5), 1.0);
	gl_FragData[6] = vec4( linearsolve (coord + decalage6), 1.0);
	gl_FragData[7] = vec4( linearsolve (coord + decalage7), 1.0);
    
}

