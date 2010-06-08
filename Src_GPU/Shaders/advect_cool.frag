

uniform sampler3D densite_entree;
uniform sampler3D vitesse_entree;


uniform float     dt;
uniform vec3      dt0;
uniform float     taille_width;
uniform float     taille_height;
uniform float     taille_depth;
uniform float     consume;
uniform float     c0;
uniform float     fireToSmoke;

    
vec3 unite = vec3 ( 1.0 / taille_width, 1.0 / taille_height, 1.0 / taille_depth);



void main (){
    vec3 coord = gl_TexCoord[0].stp;
    
    vec3 old_pos = vec3 ( coord - ( texture3D( vitesse_entree, coord).rgb   *  dt0 ));
    
    //vec3 old_pos = vec3 ( correction *coord - correction * vec3(0.0, 0.010159,0.0));
    //vec3 decalage = vec3(0.0, 0.0049,0.0);
    //vec3 old_pos = vec3 ( coord - decalage);
    //( texture3D( vitesse_entree, coord).rgb ) * dt0;

    
   
    if (old_pos.x < 0.5 / taille_width)
        old_pos.x = 0.5 / taille_width;
    if (old_pos.x > 1.0 + 0.5 / taille_width)
        old_pos.x = 1.0 + 0.5 / taille_width;
    
    if (old_pos.y < 0.5 / taille_height)
        old_pos.y = 0.5 / taille_height;
    if (old_pos.y > 1.0 + 0.5 / taille_height)
        old_pos.y = 1.0 + 0.5 / taille_height;
    
    if (old_pos.z < 0.5 / taille_depth)
        old_pos.z = 0.5 / taille_depth;
    if (old_pos.z > 1.0 + 0.5 / taille_depth)
        old_pos.z = 1.0 + 0.5 / taille_depth;
        
    
   
    // on cast en int le déplacement
    ivec3 old_int_int = ivec3(taille_width  * old_pos.x,
                              taille_height * old_pos.y,
                              taille_depth  * old_pos.z);
                          
    ivec3 old_a_cote_int = old_int_int + ivec3(1, 1, 1);
    
    // on le recaste en float ( pour avoir acces aux bonnes cases de la texture )
    vec3 old_int = old_int_int * unite;
    vec3 old_a_cote = old_a_cote_int * unite;
    
    
    vec3 s1 = coord - old_int;
    vec3 s0 = ivec3(1, 1, 1) - s1;
    
    
    
    // Calcul de l'advection de la flamme, de la fumee, de la chaleur
    
    vec3 calcul = s0.x * ( s0.y *  ( 
                                    s0.z * texture3D(densite_entree, vec3(old_int.x, old_int.y, old_int.z)).rgb
                                    + s1.z * texture3D(densite_entree, vec3(old_int.x, old_int.y, old_a_cote.z)).rgb
                                    )
                           + s1.y * ( 
                                    s0.z * texture3D(densite_entree, vec3(old_int.x, old_a_cote.y, old_int.z)).rgb
                                    + s1.z * texture3D(densite_entree, vec3(old_int.x, old_a_cote.y, old_a_cote.z)).rgb
                                    )
                          )
                  +s1.x * ( s0.y *  ( 
                                    s0.z * texture3D(densite_entree, vec3(old_a_cote.x, old_int.y, old_int.z)).rgb
                                    + s1.z * texture3D(densite_entree, vec3(old_a_cote.x, old_int.y, old_a_cote.z)).rgb
                                    )
                           + s1.y * ( 
                                    s0.z * texture3D(densite_entree, vec3(old_a_cote.x, old_a_cote.y, old_int.z)).rgb
                                    + s1.z * texture3D(densite_entree, vec3(old_a_cote.x, old_a_cote.y, old_a_cote.z)).rgb
                                    )
                          ) ;
      
    

    // disssipation de la chaleur
    calcul.z = calcul.z * c0;    

    // destruction de la matière
    float temp = calcul.x ;
    calcul.x = max ( 0, calcul.x - consume*dt);
    calcul.y = calcul.y + fireToSmoke * (temp - calcul.x);


    // ecriture du resultat    
    gl_FragData[0] = vec4( calcul, 1.0);
}

