
uniform sampler3D Texture0;
uniform sampler3D Texture1;
uniform sampler3D Textureperlin;
uniform float temps1;
uniform float temps2;



void main(void)
{  

	vec4 texel0, texel1, color;
	vec3 coord = gl_TexCoord[0].stp ; 
	vec3 perl = vec3( texture3D( Textureperlin,gl_TexCoord[0].stp ));

	coord.x = coord.x + perl.x * cos(temps1)*sin(2*temps2);
	coord.y = coord.y + perl.y * sin(temps1)*sin(2*temps2);
	coord.z = coord.z + perl.z * cos(2*temps2);



	texel0 = vec4( texture3D( Texture0,coord)) ;
	texel1 = vec4( texture3D( Texture1,gl_TexCoord[0].stp)) ;
	
	if( texel0.a > texel1.a )
		color = texel0;
	else 
		color = texel1;
	
	gl_FragColor = color; 
}

