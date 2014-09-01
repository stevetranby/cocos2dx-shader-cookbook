void main(){
	gl_FragColor = cc_FragColor*texture2D(CC_Texture0, cc_FragTexCoord1);
}
