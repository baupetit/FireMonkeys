

uniform sampler3D texture_entree;
uniform sampler3D texture_bruit;


void main(void)
{  

	vec4 color_flamme, color_fumee, color_chaleur, flamme_fumee_chaleur, color;
	
	flamme_fumee_chaleur = vec4(texture3D( texture_entree,gl_TexCoord[0].stp));
	
	color_flamme  = vec4(flamme_fumee_chaleur.x, 0.0, 0.0, flamme_fumee_chaleur.x);
	color_fumee   = vec4(flamme_fumee_chaleur.y, flamme_fumee_chaleur.y, flamme_fumee_chaleur.y, flamme_fumee_chaleur.y);
	color_chaleur = vec4(flamme_fumee_chaleur.z, flamme_fumee_chaleur.z, flamme_fumee_chaleur.z, flamme_fumee_chaleur.z / 3.0);
	
	
	
	if( color_flamme.a > color_fumee.a )
	{
	    if ( color_fumee.a > color_chaleur.a )
		    color = color_flamme;
		else
		    color = color_chaleur;
	}	
	else 
	{
	    if ( color_flamme.a > color_chaleur.a )
		    color = color_fumee;
		else
		    color = color_chaleur;
	}
	//gl_FragColor = color_flamme;
	gl_FragColor = vec4(flamme_fumee_chaleur.x * 15.0, 0.0, 0.0, flamme_fumee_chaleur.x * 50.0 );
}

