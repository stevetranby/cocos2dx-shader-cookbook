#ifdef GL_ES
precision mediump float;
#endif

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;

uniform vec4 u_ColorFlash;

void main(){
	vec4 color = texture2D(CC_Texture0, cc_FragTexCoord1);
	
	// Composite the flash color over the texture color.
	// mix() is a builtin GLSL function that does linear interpolation.
	color.rgb = mix(color.rgb, u_ColorFlash.rgb*color.a, u_ColorFlash.a);
	
	gl_FragColor = cc_FragColor*color;
}
