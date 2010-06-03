

uniform sampler3D fire;
//uniform sampler3D fumee;
//-------------------


void main(void)
{  
   vec4 texel0;
   //-------------------
   texel0 = vec4( texture3D( fire,gl_TexCoord[0].stp));
   //texel1 = vec4( texture3D( fumee,gl_TexCoord[0].stp));
   //-------------------
   //resultColor = mix(texel0, texel1, texel0.a);
   gl_FragColor = vec4(texel0);
}

