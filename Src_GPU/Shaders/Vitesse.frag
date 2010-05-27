uniform sampler3D SpeedField;



void main (){
    vec3 centre = vec3 (0.0, 1.0, 0.0);    
    vec3 speed = texture3D(SpeedField,gl_TexCoord[0].xyz).xyz;
    speed.y = 1.0;
    gl_FragColor = vec4 ( 1.0, 1.0, 0.0, 0.2);
}


