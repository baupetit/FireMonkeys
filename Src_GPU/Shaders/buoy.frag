

uniform sampler3D texture_densite;
uniform sampler3D texture_vitesse;


uniform float     dt;
uniform float     buoy;


void main (){
    
    vec3 coord = gl_TexCoord[0].stp;
    vec3 densite = texture3D( texture_densite, coord) . rgb;
    vec3 vitesse = texture3D( texture_vitesse, coord) . rgb;
    vitesse.y = vitesse.y + densite.y * buoy * dt;
    gl_FragData[0] = vec4( vitesse, 1.0);
}

