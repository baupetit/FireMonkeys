

uniform sampler3D texture_vitesse;
uniform sampler3D texture_pression;

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
    vec3 vitesse = texture3D( texture_vitesse, coord) . rgb;
    
    vitesse.x = vitesse.x - 0.0001 * taille_width / 3.0 * 
                            ( texture3D ( texture_pression, coord + voisinO ).g
                              - texture3D ( texture_pression, coord + voisinE ).g 
                            );
                            
    vitesse.y = vitesse.y - 0.0001 * taille_height / 3.0 * 
                            ( texture3D ( texture_pression, coord + voisinN ).g 
                              - texture3D ( texture_pression, coord + voisinS ).g 
                            );
                            
    vitesse.z = vitesse.z - 0.0001 * taille_depth / 3.0 * 
                            ( texture3D ( texture_pression, coord + voisinAV ).g 
                              - texture3D ( texture_pression, coord + voisinAR ).g 
                            );  
    
    gl_FragData[0] = vec4( vitesse , 1.0);
}


