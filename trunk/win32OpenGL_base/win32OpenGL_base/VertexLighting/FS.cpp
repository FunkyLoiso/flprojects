uniform sampler2D Texture0;

void main(void)
{
	vec4 txtColor = texture2D(Texture0, gl_TexCoord[0].st);
   gl_FragColor =  gl_FrontLightModelProduct.sceneColor + gl_Color * txtColor + gl_SecondaryColor;
}