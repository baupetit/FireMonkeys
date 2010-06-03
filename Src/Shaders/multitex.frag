uniform sampler3D Texture0;
uniform sampler3D Texture1;

void main()
{  
	vec4 texel0, texel1, color;
	
	texel0 = vec4( texture3D( Texture0,gl_TexCoord[0].stp));
	texel1 = vec4( texture3D( Texture1,gl_TexCoord[0].stp));
	
	if( texel0.a > texel1.a )
		color = texel0;
	else 
		color = texel1;
	
	gl_FragColor = color; 
}

