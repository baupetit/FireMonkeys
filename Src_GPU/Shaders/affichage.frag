

uniform sampler3D texture_entree;
uniform sampler3D texture_bruit;



int   couleur_size = 81;





vec3 corps_noir [81] =
         vec3[81](
         vec3(0.0014, 0.0000, 0.0065),
         vec3(0.0022, 0.0001, 0.0105),
         vec3(0.0042, 0.0001, 0.0201),
         vec3(0.0076, 0.0002, 0.0362),
         vec3(0.0143, 0.0004, 0.0679),
         vec3(0.0232, 0.0006, 0.1102),
         vec3(0.0435, 0.0012, 0.2074),
         vec3(0.0776, 0.0022, 0.3713),
         vec3(0.1344, 0.0040, 0.6456),
         vec3(0.2148, 0.0073, 1.0391),
         vec3(0.2839, 0.0116, 1.3856),
         vec3(0.3285, 0.0168, 1.6230),
         vec3(0.3483, 0.0230, 1.7471),
         vec3(0.3481, 0.0298, 1.7826),
         vec3(0.3362, 0.0380, 1.7721),
         vec3(0.3187, 0.0480, 1.7441),
         vec3(0.2908, 0.0600, 1.6692),
         vec3(0.2511, 0.0739, 1.5281),
         vec3(0.1954, 0.0910, 1.2876),
         vec3(0.1421, 0.1126, 1.0419),
         vec3(0.0956, 0.1390, 0.8130),
         vec3(0.0580, 0.1693, 0.6162),
         vec3(0.0320, 0.2080, 0.4652),
         vec3(0.0147, 0.2586, 0.3533),
         vec3(0.0049, 0.3230, 0.2720),
         vec3(0.0024, 0.4073, 0.2123),
         vec3(0.0093, 0.5030, 0.1582),
         vec3(0.0291, 0.6082, 0.1117),
         vec3(0.0633, 0.7100, 0.0782),
         vec3(0.1096, 0.7932, 0.0573),
         vec3(0.1655, 0.8620, 0.0422),
         vec3(0.2257, 0.9149, 0.0298),
         vec3(0.2904, 0.9540, 0.0203),
         vec3(0.3597, 0.9803, 0.0134),
         vec3(0.4334, 0.9950, 0.0087),
         vec3(0.5121, 1.0000, 0.0057),
         vec3(0.5945, 0.9950, 0.0039),
         vec3(0.6784, 0.9786, 0.0027),
         vec3(0.7621, 0.9520, 0.0021),
         vec3(0.8425, 0.9154, 0.0018),
         vec3(0.9163, 0.8700, 0.0017),
         vec3(0.9786, 0.8163, 0.0014),
         vec3(1.0263, 0.7570, 0.0011),
         vec3(1.0567, 0.6949, 0.0010),
         vec3(1.0622, 0.6310, 0.0008),
         vec3(1.0456, 0.5668, 0.0006),
         vec3(1.0026, 0.5030, 0.0003),
         vec3(0.9384, 0.4412, 0.0002),
         vec3(0.8544, 0.3810, 0.0002),
         vec3(0.7514, 0.3210, 0.0001),
         vec3(0.6424, 0.2650, 0.0000),
         vec3(0.5419, 0.2170, 0.0000),
         vec3(0.4479, 0.1750, 0.0000),
         vec3(0.3608, 0.1382, 0.0000),
         vec3(0.2835, 0.1070, 0.0000),
         vec3(0.2187, 0.0816, 0.0000),
         vec3(0.1649, 0.0610, 0.0000),
         vec3(0.1212, 0.0446, 0.0000),
         vec3(0.0874, 0.0320, 0.0000),
         vec3(0.0636, 0.0232, 0.0000),
         vec3(0.0468, 0.0170, 0.0000),
         vec3(0.0329, 0.0119, 0.0000),
         vec3(0.0227, 0.0082, 0.0000),
         vec3(0.0158, 0.0057, 0.0000),
         vec3(0.0114, 0.0041, 0.0000),
         vec3(0.0081, 0.0029, 0.0000),
         vec3(0.0058, 0.0021, 0.0000),
         vec3(0.0041, 0.0015, 0.0000),
         vec3(0.0029, 0.0010, 0.0000),
         vec3(0.0020, 0.0007, 0.0000),
         vec3(0.0014, 0.0005, 0.0000),
         vec3(0.0010, 0.0004, 0.0000),
         vec3(0.0007, 0.0002, 0.0000),
         vec3(0.0005, 0.0002, 0.0000),
         vec3(0.0003, 0.0001, 0.0000),
         vec3(0.0002, 0.0001, 0.0000),
         vec3(0.0002, 0.0001, 0.0000),
         vec3(0.0001, 0.0000, 0.0000),
         vec3(0.0001, 0.0000, 0.0000),
         vec3(0.0001, 0.0000, 0.0000),
         vec3(0.0000, 1.0000, 0.0000)
         );







vec3 getRGB( float temperature )
{
    vec3 res;
	int index = int(150000000.0*temperature);
	
	if (index >= couleur_size)
	    index = 81;
	if (index <= 0)
	    index = 0;
	
	res = corps_noir[index];
	return res;
}











void main(void)
{  

	vec4 color_flamme, color_fumee, color_chaleur, flamme_fumee_chaleur, color;
	
	flamme_fumee_chaleur = vec4(texture3D( texture_entree,gl_TexCoord[0].stp));
	/*
	if( flamme_fumee_chaleur.x * 1000000 > flamme_fumee_chaleur.y*0.0002 )
	{
	    color_flamme  = vec4( 0.9 - flamme_fumee_chaleur.z * 8000.0, 
	                          0.5 - flamme_fumee_chaleur.z * 8000.0, 
	                          flamme_fumee_chaleur.z * 8000,
	                          flamme_fumee_chaleur.x * 1000.0 - flamme_fumee_chaleur.z * 5000);
	    color = color_flamme;
	}	
	else 
	{
    
	    color_fumee   = vec4(0.2, 
	                         0.2, 
	                         0.2, 
	                         flamme_fumee_chaleur.y*0.002);
	                     
	                     
	    color = color_fumee;
	}
    */
    
    
    
	color_flamme  = vec4(     0.9 - flamme_fumee_chaleur.z * 5000.0, 
	                          0.5 - flamme_fumee_chaleur.z * 5000.0, 
	                          flamme_fumee_chaleur.z * 5000,
	                          (1.0 - flamme_fumee_chaleur.z * 5000) * flamme_fumee_chaleur.x*4000);
	color = color_flamme;
    
    
    
    
    
    
    
    
    
    
	                     
	gl_FragColor = color;
}

