

uniform sampler3D texture_entree;

uniform sampler3D Textureperlin;
uniform float temps1;
uniform float temps2;
uniform sampler1D Temps;
uniform sampler1D ColorTexture;


void main(void)
{  

	vec4 color_flamme, color_fumee, color_chaleur, flamme_fumee_chaleur, color;
	
	vec3 perl = vec3( texture3D( Textureperlin,gl_TexCoord[0].stp ));
    vec3 coord = gl_TexCoord[0].stp ;
    
    
	coord.x = coord.x + perl.x * 0.05 * cos(temps1)*sin(2*temps2);
	coord.y = coord.y + perl.y * 0.02 * sin(temps1)*sin(2*temps2);
	coord.z = coord.z + perl.z * 0.05 * cos(2*temps2);
	
	
	
	flamme_fumee_chaleur = vec4(texture3D( texture_entree,coord));

    vec3 couleurtext = texture1D( ColorTexture, flamme_fumee_chaleur.z * 1100).rgb;

    color.r = flamme_fumee_chaleur.x;
    color.g = flamme_fumee_chaleur.x;
    color.b = flamme_fumee_chaleur.x;
    color.a = flamme_fumee_chaleur.x;
    	                     
    color *= 1000;	
    color = vec4(couleurtext,flamme_fumee_chaleur.x*1000);               
	gl_FragColor = color;
}

