

uniform sampler3D texture_vitesse;
uniform sampler3D texture_densite;


uniform float     dt;
uniform float     buoy;


void main (){
    
    vec3 coord    =  gl_TexCoord[0].stp;
    vec3 vitesse  =  texture3D( texture_vitesse, coord) . rgb;
    vec3 densite  =  texture3D( texture_densite, coord) . rgb;
    vitesse.g = vitesse.y + densite. z * buoy * dt;
    gl_FragData[0] = vec4( vitesse, 1.0);
}

