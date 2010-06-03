uniform sampler2D textureDay;

void main(void)
{
  vec3 colorDay   = vec3( texture2D(textureDay  , gl_TexCoord[0].st) );

  vec3 color = colorDay;

  gl_FragColor = vec4(color, 1.0);
}
