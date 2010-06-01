uniform sampler3D SpeedField;



void main (){
    
    gl_FragColor = vec4(texture3D(SpeedField,gl_TexCoord[0].xyz).xyz, 1.0);

    
    
}


