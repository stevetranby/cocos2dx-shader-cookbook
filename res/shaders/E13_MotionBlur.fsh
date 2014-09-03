uniform vec2 u_BlurVector;

varying vec2 cc_FragTexCoord1;

void main(){
	const int nSamples = 8;
	
	vec4 result = texture2D(CC_Texture0, cc_FragTexCoord1 + u_BlurVector * -0.5);
	for (int i = 1; i < nSamples; ++i) {
		// get offset in range [-0.5, 0.5]:
		vec2 offset = u_BlurVector * (float(i) / float(nSamples - 1) - 0.5);
	
		// sample & add to result:
		result += texture2D(CC_Texture0, cc_FragTexCoord1 + offset);
	}
	
	result /= float(nSamples);
	
	gl_FragColor = result;
}
