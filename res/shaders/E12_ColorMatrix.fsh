#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 u_ColorMatrix;

varying vec2 cc_FragTexCoord1;

void main(){
	gl_FragColor = u_ColorMatrix * texture2D(CC_Texture0, cc_FragTexCoord1);
}
