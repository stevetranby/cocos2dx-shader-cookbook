#ifdef GL_ES
precision mediump float;
#endif

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;

void main()
{
//    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	// Read the sprite's texture's color for the current pixel.
	vec4 textureColor = texture2D(CC_Texture0, cc_FragTexCoord1);
	
	// Tint the texture color by the sprite's color.
	gl_FragColor = cc_FragColor * textureColor;
}
