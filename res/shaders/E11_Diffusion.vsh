uniform vec2 u_NoiseTexturePixelSize;
uniform float u_AnimationEnabled;
uniform float u_BlockSize;


attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying mediump vec4 cc_FragColor;
varying mediump vec2 cc_FragTexCoord1;
varying mediump vec2 cc_FragTexCoord2;
#else
varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;
varying vec2 cc_FragTexCoord2;
#endif


void main(){
    gl_Position = CC_PMatrix * a_position;
    cc_FragColor = clamp(a_color, 0.0, 1.0);
    cc_FragTexCoord1 = a_texCoord;

	vec2 randomAnimatedOffset = CC_Random01.xy * u_AnimationEnabled;
	cc_FragTexCoord2 = (cc_FragTexCoord2 + randomAnimatedOffset) / u_BlockSize;
}
