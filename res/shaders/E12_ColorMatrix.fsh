uniform mat4 u_ColorMatrix;

void main(){
	gl_FragColor = u_ColorMatrix*texture2D(CC_Texture0, cc_FragTexCoord1);
}
