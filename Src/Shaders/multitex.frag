
uniform sampler3D Texture0;
uniform sampler3D Texture1;
uniform sampler3D Textureperlin;
uniform float temps1;
uniform float temps2;
uniform float temps3;
uniform sampler1D Temps;



void main(void)
{  
	vec4 texel0, texel1, color;
	vec3 coord = gl_TexCoord[0].stp ;

	vec3 tps_perl = vec3( texture1D( Temps, temps1/30 ));

	vec3 perl = vec3( texture3D( Textureperlin, coord + mod(temps1,*tps_perl ));

	coord.x = coord.x + perl.x;// * cos(tps_perl.x)*sin(2*tps_perl.y);
	coord.y = coord.y + perl.y;// * sin(tps_perl.x)*sin(2*tps_perl.y);
	coord.z = coord.z + perl.z;// * cos(2*tps_perl.y);


	texel0 = vec4( texture3D( Texture0,coord)) ;
	texel1 = vec4( texture3D( Texture1,gl_TexCoord[0].stp)) ;
	
	if( texel0.a > texel1.a )
		color = texel0;
	else 
		color = texel1;
	
	gl_FragColor = color; 
}

