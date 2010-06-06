

uniform sampler3D texture_sources;
uniform sampler3D texture_densite;


uniform float     dt;


void main (){
    

    vec3 coord = gl_TexCoord[0].stp;
    vec3 densite = texture3D( texture_densite, coord) . rgb;
    vec3 sources = texture3D( texture_sources, coord) . rgb;
    
    gl_FragData[0] = vec4( densite + sources * dt , 1.0);
}

