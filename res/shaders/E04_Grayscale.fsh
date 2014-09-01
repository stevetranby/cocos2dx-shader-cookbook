#ifdef GL_ES
precision mediump float;
#endif

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;

void main(){
	gl_FragColor = cc_FragColor * texture2D(CC_Texture0, cc_FragTexCoord1);
	
	float gray = (gl_FragColor.r + gl_FragColor.g + gl_FragColor.b)/3.0;
	gl_FragColor.rgb = vec3(gray);
}
