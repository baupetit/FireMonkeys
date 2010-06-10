

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
    
    
	color_fumee = vec4(texture3D( texture_entree,coord));
	
	coord.x = coord.x + perl.x * 0.01 * cos(temps1)*sin(2*temps2);
	coord.y = coord.y + perl.y * 0.03 * sin(temps1)*sin(2*temps2);
	coord.z = coord.z + perl.z * 0.01 * cos(2*temps2);	
	
	flamme_fumee_chaleur = vec4(texture3D( texture_entree,coord));

    vec3 couleurtext;
    
    	                     
    color *= 1000;	
    if (flamme_fumee_chaleur.x * 1000 > color_fumee.y){
        couleurtext = texture1D( ColorTexture,   0.4 + ( flamme_fumee_chaleur.z *100 )).rgb;
        color = vec4(couleurtext,flamme_fumee_chaleur.x*200 - flamme_fumee_chaleur.z*100);               
    }
    else {
        couleurtext = vec3(color_fumee.y, color_fumee.y, color_fumee.y);
        color = vec4(couleurtext,color_fumee.y*0.1);               
    }
	gl_FragColor = color;
}

