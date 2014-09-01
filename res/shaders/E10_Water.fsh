uniform sampler2D u_NoiseTexture;
uniform sampler2D u_CausticTexture;

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
	vec2 distortion = 2.0 * texture2D(u_NoiseTexture, cc_FragTexCoord2).xy - 1.0;
	vec2 distortionOffset = distortion * 0.15;
    gl_FragColor =  texture2D(CC_Texture0, cc_FragTexCoord1 + distortionOffset);
	gl_FragColor += 0.5 * texture2D(u_CausticTexture, cc_FragTexCoord1 - distortionOffset);
}
