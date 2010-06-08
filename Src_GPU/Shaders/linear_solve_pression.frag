

uniform sampler3D texture_entree;
uniform sampler3D texture_sortie;

uniform float     taille_width;
uniform float     taille_height;
uniform float     taille_depth;

uniform float      a;
uniform float      c;

#define DX  1.0/(taille_width)
#define DY  1.0/(taille_height)
#define DZ  1.0/(taille_depth)

#define DECALAGE 1.0/taille_depth

vec3    voisinN        =vec3(0.0, DY, 0.0);
vec3    voisinE        =vec3(-DX, 0.0, 0.0);
vec3    voisinS        =vec3(0.0, -DY, 0.0);
vec3    voisinO        =vec3(DX, 0.0, 0.0);
vec3    voisinAV       =vec3(0.0, 0.0, DZ);
vec3    voisinAR       =vec3(0.0, 0.0, -DZ);


vec2 linearsolve(vec3 coordonnee){
    
    
    
    vec2 result;
    
    if (   (coordonnee.s <= DX) 
        || (coordonnee.s >= 1-DX)  
        || coordonnee.t <= DY  
        || coordonnee.t >= 1-DY  
        || coordonnee.p <= DZ  
        || coordonnee.p >= 1-DZ  ) {
        result.g = 0.0;        
        
    }else{

        result.g = float( texture3D ( texture_entree, coordonnee ) .r - 0.5)
                 + a * ( ( texture3D ( texture_sortie, coordonnee + voisinN) .g ) -0.5
                       + ( texture3D ( texture_sortie, coordonnee + voisinE) .g ) -0.5
                       + ( texture3D ( texture_sortie, coordonnee + voisinS) .g ) -0.5
                       + ( texture3D ( texture_sortie, coordonnee + voisinO) .g ) -0.5
                       + ( texture3D ( texture_sortie, coordonnee + voisinAV) .g ) -0.5
                       + ( texture3D ( texture_sortie, coordonnee + voisinAR) .g ) -0.5
                       );
        result.g = result.g / c ; 
        result.g = result.g + 0.5;
    }
    result.r = float( texture3D ( texture_entree, coordonnee ) .r );
    return result;
}


void main (){
    vec3 coord = gl_TexCoord[0].stp;
	gl_FragData[0] = vec4( linearsolve (coord),0.0, 0.0);
}

