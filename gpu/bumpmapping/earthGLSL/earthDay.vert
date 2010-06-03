
void main()
{
  // connect first texture coordinates of the vertex
  gl_TexCoord[0] = gl_MultiTexCoord0;

  gl_Position = ftransform();		
}
