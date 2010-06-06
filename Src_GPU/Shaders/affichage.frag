

uniform sampler3D texture_entree;
uniform sampler3D texture_bruit;



void main(void)
{  

	vec4 color_flamme, color_fumee, color_chaleur, flamme_fumee_chaleur, color;
	
	flamme_fumee_chaleur = vec4(texture3D( texture_entree,gl_TexCoord[0].stp));
	
	color_flamme  = vec4(flamme_fumee_chaleur.x * 10000.0,
	                     0.0, 
	                     0.0, 
	                     flamme_fumee_chaleur.x * 5000.0);
	                     
	color_fumee   = vec4(0.2, 
	                     0.2, 
	                     0.2, 
	                     flamme_fumee_chaleur.y * 1.0);
	                     
	color_chaleur = vec4(0.9, 
	                     0.8,//flamme_fumee_chaleur.z, 
	                     0.9,//flamme_fumee_chaleur.z, 
	                     flamme_fumee_chaleur.z * 10.0);
	
	
	
	if( color_flamme.a > (color_fumee.a * 1.0) )
	{
	    color = color_flamme;
	}	
	else 
	{
	    color = color_fumee;
	}

	gl_FragColor = color_flamme;
}

