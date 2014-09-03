uniform vec2 u_NoiseTexturePixelSize;
uniform vec2 cc_ViewSizeInPixels;

attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying mediump vec4 cc_FragColor;
varying mediump vec2 cc_FragTexCoord1;
#else
varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;
#endif

void main()
{
	gl_Position = CC_PMatrix * a_position;
	cc_FragColor = clamp(a_color, 0.0, 1.0);
	cc_FragTexCoord1 = a_texCoord;
}
