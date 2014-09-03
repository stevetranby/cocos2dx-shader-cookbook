uniform sampler2D u_NoiseTexture;

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;

void main(){
	vec2 distortionScroll = vec2(-3.0 * CC_Time[0], 0.0);
	vec2 distortion = texture2D(u_NoiseTexture, vec2(1.5, 0.5)*cc_FragTexCoord1 + distortionScroll).xy;
	distortion.x = pow(distortion.x, 0.5);
	
	const float xDistort = 0.2;
	vec2 distortionOffset = cc_FragTexCoord1.x * mix(vec2(0.2, -xDistort), vec2(0.0, xDistort), distortion);
	gl_FragColor = cc_FragColor * texture2D(CC_Texture0, cc_FragTexCoord1 + distortionOffset);
}
