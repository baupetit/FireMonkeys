

uniform sampler3D texture_vitesse;
uniform sampler3D texture_pression;

uniform float     echantillonage;
uniform float     taille_width;
uniform float     taille_height;
uniform float     taille_depth;

#define DX  1.0/(taille_width)
#define DY  1.0/(taille_height)
#define DZ  1.0/(taille_depth)


vec3    voisinN        =vec3(0.0, DY, 0.0);
vec3    voisinE        =vec3(-DX, 0.0, 0.0);
vec3    voisinS        =vec3(0.0, -DY, 0.0);
vec3    voisinO        =vec3(DX, 0.0, 0.0);
vec3    voisinAV       =vec3(0.0, 0.0, DZ);
vec3    voisinAR       =vec3(0.0, 0.0, -DZ);


void main (){
    
    vec3 coord = gl_TexCoord[0].stp;
    
    float div =  0.5 + (1.0/2.0)  * (1.0 / echantillonage)
                              *
                              (   texture3D( texture_vitesse, coord + voisinO).r
                                - texture3D( texture_vitesse, coord + voisinE).r
                                + texture3D( texture_vitesse, coord + voisinN).g
                                - texture3D( texture_vitesse, coord + voisinS).g
                                + texture3D( texture_vitesse, coord + voisinAV).b
                                - texture3D( texture_vitesse, coord + voisinAR).b
                              );
    
    gl_FragData[0] = vec4( div ,0.0, 0.0, 1.0);
}

